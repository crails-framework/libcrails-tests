#include "runner.hpp"
#include <crails/logger.hpp>
#include <crails/environment.hpp>
#include <crails/utils/backtrace.hpp>

using namespace std;
using namespace Crails;
using namespace Crails::Tests;

int main(int, char**)
{
  Runner runner;
  int    exit_status = 1;

  Crails::environment = Crails::Test;
  try
  {
    runner.setup();
    if (runner.execute())
      exit_status = 0;
    runner.shutdown();
  }
  catch (const boost_ext::runtime_error& error)
  {
    cerr << "Caught exception: " << boost_ext::trace(error) << endl;
  }
  return exit_status;
}
