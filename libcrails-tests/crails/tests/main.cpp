#include "runner.hpp"
#include <crails/logger.hpp>
#include <crails/environment.hpp>

using namespace std;
using namespace Crails;
using namespace Crails::Tests;

int main(int, char**)
{
  Runner runner;
  int    exit_status = 1;

  Crails::environment = Crails::Test;
  runner.setup();
  if (runner.execute())
    exit_status = 0;
  runner.shutdown();
  return exit_status;
}
