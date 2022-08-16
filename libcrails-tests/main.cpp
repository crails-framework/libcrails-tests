#include "runner.hpp"
#include <crails/server.hpp>
#include <crails/logger.hpp>
#include <crails/environment.hpp>

using namespace std;
using namespace Crails;
using namespace Crails::Tests;

Server* test_server = nullptr;

int main(int, char**)
{
  Server server;
  Runner runner;
  int    exit_status = 1;

  test_server = &server;
  Crails::environment = Crails::Test;
  runner.setup();
  if (runner.execute())
    exit_status = 0;
  runner.shutdown();
  return exit_status;
}
