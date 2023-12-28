#ifndef  CRAILS_TESTS_REQUEST_HPP
# define CRAILS_TESTS_REQUEST_HPP

# include <crails/router.hpp>
# include <crails/params.hpp>

# define EXPECT_STATUS(request, status) \
  EXPECT(static_cast<int>(request.response.result()), ==, static_cast<int>(status))

namespace Crails
{
  namespace Tests
  {
    class Request
    {
      struct RouterNotInitialized : public std::exception
      {
        const char* what() const throw() { return "Router not initialized"; }
      };

      struct RouteNotFound : public std::exception
      {
        RouteNotFound(const std::string& route) : message("Route not found: " + route) {}
        const char* what() const throw() { return message.c_str(); }
        const std::string message;
      };

      struct NotHandled : public std::exception
      {
        const char* what() const throw() { return "Request not handled"; }
      };

    public:
      Request(boost::beast::http::verb method, const std::string& uri);

      void run();

      DataTree     params;
      DataTree     session;
      HttpRequest  request;
      HttpResponse response;
    };
  }
}

#endif
