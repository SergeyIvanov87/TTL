#ifndef EVENT_BROKER_HPP
#define EVENT_BROKER_HPP
#include <vector>
#include <thread>
#include <mutex> //TODO
#include <future>
#include <condition_variable>
#include "EventBroker.h"
#include "Framework/Utils/CTimeUtils.h"
#define T_ARGS_DECL         class ...EventDirectors
#define T_ARGS_DEF          EventDirectors...
template<T_ARGS_DECL>
EventBroker<T_ARGS_DEF>::~EventBroker()
{
    if(m_worker.joinable())
    {
        m_worker.join();
    }
}
template<T_ARGS_DECL>
template<class Event, class Producer>
void EventBroker<T_ARGS_DEF>::push_sync_event(Event &&event, Producer &producer)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);
    directorContainedProducer.produceEvent(producer, std::forward<Event>(event));
}

template<T_ARGS_DECL>
template<class Event, class Producer>
void EventBroker<T_ARGS_DEF>::push_async_event(Event &&event, Producer &producer)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer, std::false_type>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);
    auto &&ret = directorContainedProducer.produceEvent(producer, std::forward<Event>(event));
    postponeEvent(std::move(ret));
}

template<T_ARGS_DECL>
template<class Producer, class Consumer>
void EventBroker<T_ARGS_DEF>::registerSyncConsumer(Consumer consumerCandidate)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);

    directorContainedProducer.registerConsumer(consumerCandidate);
}

template<T_ARGS_DECL>
template<class Producer, class Consumer>
void EventBroker<T_ARGS_DEF>::registerAsyncConsumer(Consumer consumerCandidate)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer, std::false_type>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);

    directorContainedProducer.registerConsumer(consumerCandidate);
}

template<T_ARGS_DECL>
template<class EventsProcessingList>
void EventBroker<T_ARGS_DEF>::postponeEvent(EventsProcessingList &&events)
{
    {
        std::unique_lock<std::mutex> l(m_asyncQueueMutex);
        std::move(events.begin(), events.end(), std::back_inserter(m_asyncEventProcessingQueue));
        m_asyncQueueCond.notify_one();
    }

    if(!m_worker.joinable())
    {
        m_worker = std::thread(&EventBroker<T_ARGS_DEF>::eventProcessorImpl, this);
    }


}

template<T_ARGS_DECL>
void EventBroker<T_ARGS_DEF>::eventProcessorImpl()
{
  while(true) //todo
            {
                decltype(m_asyncEventProcessingQueue) processingQueue;
                {
                    std::unique_lock<std::mutex> l(m_asyncQueueMutex);
                    m_asyncQueueCond.wait(l, [this](){ return !m_asyncEventProcessingQueue.empty(); });
                    processingQueue.swap(m_asyncEventProcessingQueue);
                }

                for(auto &processor : processingQueue)
                {
                    processor();
                }
            }
        }
#undef T_ARGS_DECL
#undef T_ARGS_DEF
#endif //EVENT_BROKER_HPP
