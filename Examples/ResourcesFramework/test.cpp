#include <Framework/ResourcesFramework/ResourcesFramework.hpp>
#include "ResourceFrameworkSpecs.h"

int main(int argc, char **argv)
{

    SSTracerImpl ss('\t');
    Tracer<SSTracerImpl> tracer(ss);
    ResourcesFramework::initializeResourceHolder("ASSETS", "dumps", tracer);
    std::cout << tracer.getManagedTracer().str();

    auto p = ResourcesFramework::Instance()->getResourcePtr<ResourceA>("ResourceA_1");
    assert(p);
    return 0;
}
