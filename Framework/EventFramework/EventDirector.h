#ifndef EVENT_DIRECTOR_H
#define EVENT_DIRECTOR_H
#include <tuple>
#include <set>
#include <array>

template<class Consumer>
struct Wrapper
{
    template<class Producer, class Event>
    void produceEventForAllConsumers(Producer &producer, Event &event);
    std::set<std::add_pointer_t<Consumer>> m_sameTypeConsumers;
};

template<class Producer, class ...Consumers>
class EventDirector
{
public:
    using ConsumersListType = std::tuple<Consumers...>;
    using ConsumersTupleType = std::tuple<Wrapper<Consumers>...>;
    using ProducerType = Producer;

    enum
    {
        ConsumersCount =  std::tuple_size<ConsumersListType>::value
    };

    template<class Event>
    void produceEvent(Producer &producer, Event &&event);

    template<class Event>
    std::array<std::function<void(void)>,
               ConsumersCount>
        produceEventDeferred(Producer &producer, Event &&event);

    template<class Consumer>
    void registerConsumer(Consumer consumerCandidate);
private:
    ConsumersTupleType m_consumers;
};
#endif
