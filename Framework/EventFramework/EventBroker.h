#if 0
#ifndef EVENT_BROKER_H
#define EVENT_BROKER_H
#include <tuple>


template<class Producer, class ...Consumers>
class EventDirector
{
public:

    using ConsumersTupleType = std::tuple<std::set<Consumers>...>;
    using ProducerType = Producer;
};


template<class ...EventDirectors>
class EventBroker
{
    std::tuple<Producer, EventDirectors...>
};
#endif //EVENT_BROKER_H

#endif
