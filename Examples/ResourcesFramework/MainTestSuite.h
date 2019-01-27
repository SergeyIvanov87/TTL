#ifndef MAIN_TEST_SUITE_H
#define MAIN_TEST_SUITE_H
#include "BaseTestSuite.h"
#include <Framework/ResourcesFramework/ResourcesFramework.hpp>
#include "ResourceFrameworkSpecs.h"

class MainSuite : public TestSuiteFixtureBase
{
    MainSuite()
    {
        addTest(&MainSuite::loadAssets);
    }

    void loadAssets()
    {
        SSTracerImpl ss('\t');
        Tracer<SSTracerImpl> tracer(ss);
        ResourcesFramework::initializeResourceHolder("ASSETS", "dumps", tracer);
        std::cout << tracer.getManagedTracer().str();

        auto p = ResourcesFramework::Instance()->getResourcePtr<ResourceA>("ResourceAResourceA_1");
        TEST_ASSERT_MSG(p, "Fail - no ResourceA type loaded"
    }
};
#endif //MAIN_TEST_SUITE_H
