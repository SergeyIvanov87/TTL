#ifndef I_NOTCONTROLLABLE_HPP
#define I_NOTCONTROLLABLE_HPP

#include "INotControllable.h"
#include "../ControllerEvent.h"

template <class ...AllEventTypes>
urc::ResultDescription
    INotControllable::onProcessEventDispatcher(CommonControllerEvent<AllEventTypes...> &event, bool notFilteredEvent/* = true*/)
{
        return urc::ResultDescription();
}

#endif //I_NOTCONTROLLABLE_HPP
