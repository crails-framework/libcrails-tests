#include <crails/tests/request.hpp>
#include <crails/tests/runner.hpp>
#include <crails/session_store/no_session_store.hpp>
#include <crails/router.hpp>
#include <crails/controller.hpp>
#include <crails/request_handlers/action.hpp>
#include "test_server.hpp"
#undef NODEBUG
#include <cassert>

using namespace std;

void Crails::Tests::Runner::setup()
{
}

void Crails::Tests::Runner::shutdown()
{
}

class TestController : public Crails::Controller
{
public:
  TestController(Crails::Context& context) : Crails::Controller(context)
  {
  }

  boost::asio::awaitable<void> coroutine_path()
  {
    render(TEXT, std::string_view("Coucou Coroutine"));
    co_return ;
  }

  void session_path()
  {
    session["myvar"] = 42;
    render(TEXT, std::string_view("Coucou Session"));
  }
};

struct TestHelper : public Crails::Tests::Helper
{
  TestHelper() : Helper("TestHelper")
  {
    describe("request tests", [this]()
    {
      it("normal coroutine", [this]()
      {
        Crails::Tests::Request test_request(boost::beast::http::verb::get, "/coroutine-path");

        test_request.run();
        assert(test_request.response_status == 200);
        assert(test_request.response_body == "Coucou Coroutine");
      });

      it("normal coroutine bis", [this]()
      {
        Crails::Tests::Request test_request(boost::beast::http::verb::get, "/coroutine-path");

        test_request.run();
        assert(test_request.response_status == 200);
        assert(test_request.response_body == "Coucou Coroutine");
      });

      it("normal 404", [this]()
      {
        Crails::Tests::Request not_found_test_request(boost::beast::http::verb::post, "/coroutine-path");
        bool exception_caught = false;
        try
        {
          not_found_test_request.run();
        }
        catch (const std::exception& err)
        {
          exception_caught = true;
        }
        assert(exception_caught);
      });
    });
  }
};

int main()
{
  using namespace Crails;
  using namespace std;

  {
    auto parser = new TestParser();
    auto handler = new ActionRequestHandler();

    SingletonInstantiator<TestServer> server;
    SingletonInstantiator<Crails::NoSessionStore::Factory> store;
    SingletonInstantiator<Crails::Router> router;
    
    server->test_setup(
      vector<Crails::RequestHandler*>{handler},
      vector<Crails::RequestParser*>{parser}
    );

    router->match_action("GET", "/coroutine-path", TestController, coroutine_path);
    router->match_action("GET", "/session-path", TestController, session_path);

    // Test normal coroutine request
    Crails::Tests::Request test_request(boost::beast::http::verb::get, "/coroutine-path");
    test_request.run();

    assert(parser->was_called);
    assert(test_request.response_status == 200);
    assert(test_request.response_body == "Coucou Coroutine");

    // Test not found request
    Crails::Tests::Request not_found_test_request(boost::beast::http::verb::post, "/coroutine-path");
    bool exception_caught = false;
    try
    {
      not_found_test_request.run();
    }
    catch (const std::exception& err)
    {
      exception_caught = true;
    }
    assert(exception_caught);

    // Test session
    Crails::Tests::Request session_request(boost::beast::http::verb::get, "/session-path");
    session_request.run();
    assert(session_request.response_status == 200);
    assert(session_request.session["myvar"].defaults_to<int>(0) == 42);

    // Helper test
    {
      TestHelper test_helper;

      test_helper.run();
      assert(test_helper.passed_count == 3);
    }
  }

  return 0;
}

