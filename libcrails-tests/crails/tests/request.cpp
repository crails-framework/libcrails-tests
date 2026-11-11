#include "request.hpp"
#include "helper.hpp"
#include <crails/router.hpp>

using namespace std;
using namespace Crails;

Tests::Request::Request(boost::beast::http::verb method, const std::string& uri)
{
  request.method(method);
  request.target(uri);
}

void Tests::Request::run()
{
  auto& test_server = Server::singleton::require();
  auto& io_context  = test_server.get_io_context();
  const Router* router = Router::singleton::get();

  if (router)
  {
    auto stub_connection = make_shared<Crails::Connection>(test_server, request);
    auto stub_request    = make_shared<Crails::Context>(test_server, *stub_connection);
    auto future_status   = stub_request->get_future();
    auto deadline        = chrono::steady_clock::now() + max_duration;

    stub_connection->set_muted(true);
    stub_request->params.as_data().merge(params);
    stub_request->params.get_session().merge(session);
    stub_request->run();

    session.as_data().destroy();
    session.as_data().merge(stub_request->params.get_session());

    if (io_context.stopped())
      io_context.restart();
    while (future_status.wait_for(chrono::milliseconds(50)) == future_status::timeout)
    {
      io_context.poll();
      if (chrono::steady_clock::now() > deadline)
        throw std::runtime_error("Tests::Request: timed out");
    }

    response = stub_request->response.get_raw_response();
    response_body = response.body();
    response_status = response.result_int();

    if (stub_request->response.get_status_code() == HttpStatus::not_found)
    {
      throw RouteNotFound(std::string(boost::beast::http::to_string(request.method())) + '#' + std::string(request.target()));
    }
    else if (!stub_request->handled)
      throw NotHandled();
  }
  else
    throw RouterNotInitialized();
}
