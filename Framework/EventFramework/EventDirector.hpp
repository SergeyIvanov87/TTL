#ifndef EVENT_DIRECTOR_HPP
#define EVENT_DIRECTOR_HPP
#include "EventDirector.h"

template<class Consumer>
template<class Producer, class Event>
void Wrapper<Consumer>::produceEventForAllConsumers(Producer &producer, Event &event)
{
    for( auto c : m_sameTypeConsumers)
    {
        c->onProcessEventDispatcher(producer, event);
    }
}


#define T_ARGS_DECL         class Producer, class ...Consumers
#define T_ARGS_DEF          Producer, Consumers...


template<T_ARGS_DECL>
template<class Event>
void EventDirector<T_ARGS_DEF>::produceEvent(Producer &producer, Event &&event)
{
    std::apply(
    [this, &producer, &event]
    (auto &...x)
    {
        bool dispatchingResult[]
        {
                 (x.produceEventForAllConsumers(producer, event), true)...
        };

    }, m_consumers);
};

template<T_ARGS_DECL>
template<class Consumer>
void EventDirector<T_ARGS_DEF>::registerConsumer(Consumer consumerCandidate)
{
    std::get<Wrapper<std::remove_pointer_t<Consumer>>>(m_consumers).m_sameTypeConsumers.insert(consumerCandidate);
}

#undef T_ARGS_DEF
#undef T_ARGS_DECL
#endif
