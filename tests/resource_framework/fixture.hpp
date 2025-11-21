#ifndef RESOURCE_FRAMEWORK_FIXTURE
#define RESOURCE_FRAMEWORK_FIXTURE

#include <filesystem>
#include <fstream>
#include <map>
#include <ranges>

#include <gtest/gtest.h>
#include <ttl/include/ResourcesFramework/ResourcesFramework.hpp>
#include <ttl/include/Utils/LogTracer.h>
#include "ResourceFrameworkSpecs.h"

struct RFAssetsPathCreationFixture : public testing::Test {
    std::filesystem::path assetsPath{"ASSETS"};
    virtual void SetUp() override {
        EXPECT_NO_THROW(std::filesystem::create_directories(assetsPath));
        EXPECT_NO_THROW(std::filesystem::create_directories("dumps"));

        auto resourceAStoragePath = assetsPath / ResourcesTraits<ResourceA>::getResourcePath();
        EXPECT_NO_THROW(std::filesystem::create_directory(resourceAStoragePath));
        auto resourceBStoragePath = assetsPath / ResourcesTraits<ResourceB>::getResourcePath();
        EXPECT_NO_THROW(std::filesystem::create_directory(resourceBStoragePath));
    }

    virtual void TearDown() override {
        std::filesystem::remove_all(assetsPath);
        std::filesystem::remove_all("dumps");
    }
};

template<std::ranges::input_range R>
std::list<std::ofstream> createFilesFromPaths(R r) {
    std::list<std::ofstream> files;
    for (const auto &p : r | std::views::values) {
        files.push_back(std::ofstream{p, std::ios::binary});
        files.back() << "something not empty" << std::endl;
    }
    return files;
}

template<std::ranges::input_range R>
void deleteFilesForPaths(R r) {
    for (const auto &p : r | std::views::values) {
        std::filesystem::remove(p);
    }
}

struct RFFixture: RFAssetsPathCreationFixture {
    using Base = RFAssetsPathCreationFixture;
    std::map<std::string, std::filesystem::path> resourceAFilePaths;
    std::map<std::string, std::filesystem::path> resourceBFilePaths;
    std::unique_ptr<ResourcesFramework> fw;

    void SetUp() override {
        Base::SetUp();

        resourceAFilePaths["ResourceAResourceA_1"] = assetsPath / ResourcesTraits<ResourceA>::getResourcePath() / "ResourceA";
        resourceBFilePaths["ResourceAResourceB_1"] = assetsPath / ResourcesTraits<ResourceB>::getResourcePath() / "ResourceB";

        createFilesFromPaths(resourceAFilePaths);
        createFilesFromPaths(resourceBFilePaths);

        Streamed ss('\t');
        Tracer<Streamed<std::stringstream>> tracer(ss);
        fw = std::make_unique<ResourcesFramework>("ASSETS", "dumps");
        fw->initResourceLoader(tracer);
        std::cout << tracer.getManagedTracer().str();

    }

    void TearDown() override {
        deleteFilesForPaths(resourceAFilePaths);
        deleteFilesForPaths(resourceBFilePaths);
        fw.reset();
        Base::TearDown();

    }
};
#endif //RESOURCE_FRAMEWORK_FIXTURE
