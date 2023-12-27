#include "request.hpp"
#include "helper.hpp"

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
  const Router* router = Router::singleton::get();

  if (router)
  {
    auto stub_connection = make_shared<Crails::Connection>(test_server, request);
    auto stub_request    = make_shared<Crails::Context>(test_server, *stub_connection);

    stub_request->params.as_data().merge(params);
    stub_request->params.get_session().merge(session);
    stub_request->run();
    session.as_data().destroy();
    session.as_data().merge(stub_request->params.get_session());
    response = stub_request->response.get_raw_response();
    if (!stub_request->handled)
      throw RouteNotFound(std::string(boost::beast::http::to_string(request.method())) + '#' + std::string(request.target()));
  }
  else
    throw RouterNotInitialized();
}
