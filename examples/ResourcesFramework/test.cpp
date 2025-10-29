#if 0
#include "MainTestSuite.h"

int main(int argc, char **argv)
{
    auto mainTest = createTests<MainSuite>()
    ts.add(unique_ptr<Test::Suite>(new TestSuite1));
    ts.add(unique_ptr<Test::Suite>(new TestSuite2));
    ts.add(unique_ptr<Test::Suite>(new TestSuite3));

    Test::TextOutput output(Test::TextOutput::Verbose);
    return ts.run(output);
}
    return 0;
}

#else

int main(int argc, char ** argv)
{
    return 0;
}
#endif
