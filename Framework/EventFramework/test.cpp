#ifdef TEST
#include <string>
#include <iostream>

#include "Interfaces/IControllable.hpp"
#include "../../../FrameworkSpecializations/EventFrameworkSpecialization/EventFrameworkSpecializationExport.h"

struct Test : public IControllable<Test, MouseEvent, KeyboardEvent>
{
    Errors::ErrorDescription processSpecificEvent(const MouseEvent &event, ControlEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << event.toString() << std::endl;
        return Errors::ErrorDescription();
    }
    Errors::ErrorDescription processSpecificEvent(const KeyboardEvent &event, ControlEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << event.toString() << std::endl;
        return Errors::ErrorDescription();
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
#endif
