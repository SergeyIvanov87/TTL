#ifndef EVENT_BROKER_H
#define EVENT_BROKER_H

template<class ...EventDirectors>
class EventBroker
{
public:
    ~EventBroker();
    using ProducersListType = std::tuple<typename EventDirectors::ProducerType...>;
    using EventDirectorsListType = std::tuple<EventDirectors...>;

    //Sync delivering
    template<class Event>
    void push_sync_event(Event &&event);

    template<class Event, class Producer>
    void push_sync_event(Event &&event, Producer &producer);

    template<class Event, class Producer>
    void push_sync_event(Event &&event, const Producer &producer);

    //Async delivering
    template<class Event>
    void push_async_event(Event &&event);

    template<class Event, class Producer>
    void push_async_event(Event &&event, Producer &producer);

    template<class Event, class Producer>
    void push_async_event(Event &&event, const Producer &producer);

    //Registration
    template<class Producer, class Consumer>
    void register_sync_consumer(Consumer consumerCandidate);

    template<class Producer, class Consumer>
    void register_async_consumer(Consumer consumerCandidate);

    void stop();
private:
    EventDirectorsListType m_directors;
    std::mutex m_asyncQueueMutex;
    std::condition_variable m_asyncQueueCond;

    std::vector<std::packaged_task<void(void)>> m_asyncEventProcessingQueue;
    std::thread m_worker;
    std::atomic<bool> m_stop = false;

    template<class Task>
    void postponeEvent(Task &&task);


    void eventProcessorImpl();
};
#endif //EVENT_BROKER_H
