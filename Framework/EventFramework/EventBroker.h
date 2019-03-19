#ifndef EVENT_BROKER_H
#define EVENT_BROKER_H
template<class ...EventDirectors>
class EventBroker
{
public:
    ~EventBroker();
    using ProducersListType = std::tuple<typename EventDirectors::ProducerType...>;
    using EventDirectorsListType = std::tuple<EventDirectors...>;

    template<class Event, class Producer>
    void push_sync_event(Event &&event, Producer &producer);

    template<class Event, class Producer>
    void push_async_event(Event &&event, Producer &producer);

    template<class Producer, class Consumer>
    void registerSyncConsumer(Consumer consumerCandidate);

    template<class Producer, class Consumer>
    void registerAsyncConsumer(Consumer consumerCandidate);
private:
    EventDirectorsListType m_directors;
    std::mutex m_asyncQueueMutex;
    std::condition_variable m_asyncQueueCond;
    std::vector<std::packaged_task<void(void)>> m_asyncEventProcessingQueue;
    std::thread m_worker;

    template<class EventsProcessingList>
    void postponeEvent(EventsProcessingList &&events);


    void eventProcessorImpl();
};
#endif //EVENT_BROKER_H
