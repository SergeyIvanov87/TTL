#ifdef TEST

#include "LoadedResourcesHolderSpecific.h"
#include "Framework/ResourcesFramework/LoadedResourcesHolder.hpp"

using namespace std;

int main(int argc, char** argv)
{
    using namespace Resources;
    getResourceObjectCPtr<Graphic::Material> ("dummy");
    return 0;
}

#endif