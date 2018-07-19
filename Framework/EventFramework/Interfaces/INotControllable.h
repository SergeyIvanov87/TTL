#ifndef I_NOTCONTROLLABLE
#define I_NOTCONTROLLABLE
#include "Framework/Utils/UnifiedResultCodes.h"
#include "FrameworkSpecializations/EventFrameworkSpecialization/EventFrameworkSpecializationExport.h"
template<class ...AllEventTypes>
class CommonControllerEvent;

class INotControllable
{
public:
    template<class ...AllEventTypes>
    urc::ResultDescription onProcessEventDispatcher(CommonControllerEvent<AllEventTypes...> &event, bool notFilteredEvent)
    {
        return urc::ResultDescription();
    };
};
#endif
