#include <string>
#include <iostream>

#include "Framework/EventFramework/Interfaces/IControllable.hpp"
#include "FrameworkSpecializations/EventFrameworkSpecialization/EventFrameworkSpecializationExport.h"

//g++ -std=c++17 test.cpp ../../FrameworkSpecializations/EventFrameworkSpecialization/EventsImpl/KeyboardEvent.cpp -I/home/user/microcontroller/git_hub/TTL/
struct Test : public IControllable<Test, MouseEvent, KeyboardEvent>
{
    urc::ResultDescription processSpecificEvent(const MouseEvent &event, ControlEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << event.toString() << std::endl;
        return urc::ResultDescription();
    }
    urc::ResultDescription processSpecificEvent(const KeyboardEvent &event, ControlEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << event.toString() << std::endl;
        return urc::ResultDescription();
    }
};

int main(int argc, char ** argv)
{
    Test t;
    auto event = createControllerEvent<MouseEvent>(
                                0.0f, 0.0f,
                                (1),
                                (1),
                                (1));
    t.onProcessEventDispatcher(*event.get());

    return 0;
}
