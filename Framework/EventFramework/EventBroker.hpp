#ifndef EVENT_BROKER_HPP
#define EVENT_BROKER_HPP
#include "EventBroker.h"
#include "Framework/Utils/CTimeUtils.h"
#define T_ARGS_DECL         class ...EventDirectors
#define T_ARGS_DEF          EventDirectors...

template<T_ARGS_DECL>
template<class Event, class Producer>
void EventBroker<T_ARGS_DEF>::push_event(Event &&event, Producer &producer)
{
    constexpr size_t producerIndex = CTimeUtils::Index<Producer, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);
    directorContainedProducer.produceEvent(producer, std::forward<Event>(event));
}

template<T_ARGS_DECL>
template<class Event, class Producer>
void EventBroker<T_ARGS_DEF>::push_deferred_event(Event &&event, Producer &producer)
{

    constexpr size_t producerIndex = CTimeUtils::Index<Producer, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);
    auto ret = directorContainedProducer.produceEventDeferred(producer, std::forward<Event>(event));

//TODO:TEST
    ret[0]();
}

template<T_ARGS_DECL>
template<class Producer, class Consumer>
void EventBroker<T_ARGS_DEF>::registerConsumer(Consumer consumerCandidate)
{
    constexpr size_t producerIndex = CTimeUtils::Index<Producer, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);

    directorContainedProducer.registerConsumer(consumerCandidate);
}
#undef T_ARGS_DECL
#undef T_ARGS_DEF
#endif //EVENT_BROKER_HPP
