#ifndef TEST_MAIN_SUITE_H
#define TEST_MAIN_SUITE_H
#include <cppunit/Test.h>
#include <vector>
class TestSuiteFixtureBase: public CppUnit::Suite
{
public:
    using TestFunction = CppUnit::Suite::Func;
    TestSuiteFixtureBase() = default;
    virtual ~TestSuiteFixtureBase() = default;

    bool chargeTestSuite()
    {
        for(auto test : m_testsList)
        {
            TEST_ADD(test);
        }
    }

    void addTest(TestFunction test)
    {
        m_testsList.push_back(test);
    }
protected:
    virtual void setup()     {} // setup resources...
    virtual void tear_down() {} // remove resources...

private:
    std::vector<TestFunction> m_testsList;
};



template<class ...Tests>
TestSuiteFixtureBase createTests()
{
    TestSuiteFixtureBase main;
    using expander = int[];
    (void)expander { 0, main.add(std::make_unique<Tests>()->chargeTestSuite())..., 0}
    main.chargeTestSuite();
    return main;
}
#endif
