#ifndef CONTROLLER_EVENT_HPP
#define CONTROLLER_EVENT_HPP

#include "ControllerEvent.h"


#define TEMPLATE_ARGS_LIST_DECL  class ...Events
#define TEMPLATE_ARGS_LIST_DEF   Events...

template<TEMPLATE_ARGS_LIST_DECL>
ControlEventID CommonControllerEvent<TEMPLATE_ARGS_LIST_DEF>::getEventType() const
{
    return std::visit([](auto &arg) -> ControlEventID
    {
        using T = std::decay_t<decltype(arg)>;
        return T::element_type::getControlEventID();
    }, m_eventHolder);
    //return static_cast<ControlEventID>(m_eventHolder.index());
}

#undef TEMPLATE_ARGS_LIST_DECL
#undef TEMPLATE_ARGS_LIST_DEF
#endif //CONTROLLER_EVENT_HPP
