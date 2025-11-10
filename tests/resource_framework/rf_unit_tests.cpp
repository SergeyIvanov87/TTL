#include <gtest/gtest.h>
#include "fixture.hpp"

TEST_F(RFAssetsPathCreationFixture, FrameworkInitialization) {
    auto tracerCallback = [] (const auto &sstream) {
        std::cout << sstream.str() << std::endl;
    };

    Streamed<std::stringstream> ss('\t', tracerCallback);
    Tracer<Streamed<std::stringstream>> tracer(ss);
    std::unique_ptr<ResourcesFramework> fw;
    ASSERT_NO_THROW(fw = std::make_unique<ResourcesFramework>("ASSETS", "dumps"));
    ASSERT_NO_THROW(fw->initResourceLoader(tracer));
    ASSERT_NO_THROW(fw->deinitResourceLoader());
}

template<Resources::BaseResourceConcept R>
void foo(R *r) {
    std::cout << "gotcha!" << std::endl;
}

TEST_F(RFFixture, ResourceLoading) {
    auto p = fw->getResourcePtr<ResourceA>("ResourceAResourceA_1");
    EXPECT_TRUE(!p.expired()) << "ResourceAResourceA_1 must be loadable";
    foo(p.lock().get());

    p = fw->getResourcePtr<ResourceA>("ResourceAXXYZZResourceA_1");
    EXPECT_TRUE(p.expired()) << "ResourceAXXYZZResourceA_1 must not be loadable";
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
