#ifndef I_NOTCONTROLLABLE
#define I_NOTCONTROLLABLE
#include "../Utils/ErrorCodes.h"
#include "../../../FrameworkSpecializations/EventFrameworkSpecialization/EventFrameworkSpecializationExport.h"

class INotControllable
{
public:
    Errors::ErrorDescription onProcessEventDispatcher(ObserverEvent &event, bool notFilteredEvent)
    {
        return Errors::ErrorDescription();
    };
};
#endif
