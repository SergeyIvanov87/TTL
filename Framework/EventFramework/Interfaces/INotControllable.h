#ifndef I_NOTCONTROLLABLE
#define I_NOTCONTROLLABLE
#include "Framework/Utils/UnifiedResultCodes.h"
#include "FrameworkSpecializations/EventFrameworkSpecialization/EventFrameworkSpecializationExport.h"

class INotControllable
{
public:
    urc::ResultDescription onProcessEventDispatcher(ObserverEvent &event, bool notFilteredEvent)
    {
        return urc::ResultDescription();
    };
};
#endif
