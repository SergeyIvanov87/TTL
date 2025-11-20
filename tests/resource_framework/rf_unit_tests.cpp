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
void tryConcept(R *r) {
    EXPECT_TRUE(r != nullptr);
}

TEST_F(RFFixture, ResourceLoading) {
    auto w = fw->getNonOwnResourcePtr<ResourceA>("ResourceAResourceA_1");
    EXPECT_TRUE(!w.expired()) << "ResourceAResourceA_1 must be loadable";
    tryConcept(w.lock().get());

    ResourceA::OwnPtrConst s;
    ASSERT_NO_THROW(s = fw->getResourcePtr<ResourceA>("ResourceAResourceA_1"));
    EXPECT_EQ(s.get(), w.lock().get());

    ASSERT_THROW(w = fw->getNonOwnResourcePtr<ResourceA>("ResourceAXXYZZResourceA_1"), urc::MissingResourceError);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
