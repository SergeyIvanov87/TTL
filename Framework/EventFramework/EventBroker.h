#ifndef EVENT_BROKER_H
#define EVENT_BROKER_H


template<class ...EventDirectors>
class EventBroker
{
public:
    using ProducersListType = std::tuple<typename EventDirectors::ProducerType...>;
    using EventDirectorsListType = std::tuple<EventDirectors...>;

    template<class Event, class Producer>
    void push_event(Event &&event, Producer &producer);

    template<class Event, class Producer>
    void push_deferred_event(Event &&event, Producer &producer);

    template<class Producer, class Consumer>
    void registerConsumer(Consumer consumerCandidate);
private:
    EventDirectorsListType m_directors;
};
#endif //EVENT_BROKER_H
