#include "ResourceFrameworkSpecs.h"
#include "Framework/ResourcesFramework/ResourcesFramework.hpp"

constexpr const char system_info_dummy_path[] = "DUMMY_PATH";
constexpr const char resourceA_path_str[] = "rA";

int main(int argc, char **argv)
{
    ResourcesFramework::initializeResourceHolder("ASSETS", "dumps");
    return 0;
}
