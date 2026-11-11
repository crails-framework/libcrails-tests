#include <crails/tests/helper.hpp>
#include <crails/tests/runner.hpp>
#undef NODEBUG
#include <cassert>

using namespace std;

struct MyTestHelper : public Crails::Tests::Helper
{
  MyTestHelper() : Helper("MyTestHelper")
  {
    describe("Group", [this]()
    {
      it("passing test", [this]()
      {
        EXPECT(true, ==, true);
      });

      xit("pending test", [this]()
      {
        EXPECT(false, ==, true);
      });

      it("failing test", [this]()
      {
        EXPECT(true, ==, false);
      });
    });
  }
};

void Crails::Tests::Runner::setup()
{
}

void Crails::Tests::Runner::shutdown()
{
}

int main()
{
  MyTestHelper helper;

  helper.run();
  assert(helper.passed_count == 1);
  assert(helper.failed_count == 1);
  assert(helper.pending_count == 1);
  return 0;
}
