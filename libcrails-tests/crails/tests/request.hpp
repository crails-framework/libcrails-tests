#ifndef  CRAILS_TESTS_REQUEST_HPP
# define CRAILS_TESTS_REQUEST_HPP

# include <crails/router.hpp>
# include <crails/params.hpp>

# define EXPECT_STATUS(context, status) \
  EXPECT(context.response.get_status_code(), ==, status)

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

    public:
      Request(boost::beast::http::verb method, const std::string& uri);

      void run();

      HttpRequest  request;
      HttpResponse response;
    };
  }
}

#endif
