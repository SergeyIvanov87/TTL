#ifndef EVENT_BROKER_HPP
#define EVENT_BROKER_HPP
#include <vector>
#include <thread>
#include <mutex> //TODO
#include <future>
#include <condition_variable>
#include <ttl/Utils/CTimeUtils.h>
#include "EventDirector.hpp"
#include "EventBroker.h"

#define T_ARGS_DECL         class ...EventDirectors
#define T_ARGS_DEF          EventDirectors...
template<T_ARGS_DECL>
EventBroker<T_ARGS_DEF>::~EventBroker()
{
    stop();
    if(m_worker.joinable())
    {
        m_worker.join();
    }
}

template<T_ARGS_DECL>
template<class Event>
void EventBroker<T_ARGS_DEF>::push_sync_event(Event &&event)
{
    static EmptyProducer stub;
    push_sync_event(std::forward<Event>(event), stub);
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
void EventBroker<T_ARGS_DEF>::push_sync_event(Event &&event, const Producer &producer)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);
    directorContainedProducer.produceEvent(producer, std::forward<Event>(event));
}


template<T_ARGS_DECL>
template<class Event>
void EventBroker<T_ARGS_DEF>::push_async_event(Event &&event)
{
    static EmptyProducer stub;
    push_async_event(std::forward<Event>(event), stub);
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
template<class Event, class Producer>
void EventBroker<T_ARGS_DEF>::push_async_event(Event &&event, const Producer &producer)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer, std::false_type>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);
    auto &&ret = directorContainedProducer.produceEvent(producer, std::forward<Event>(event));
    postponeEvent(std::move(ret));
}

template<T_ARGS_DECL>
template<class Producer, class Consumer>
void EventBroker<T_ARGS_DEF>::register_sync_consumer(Consumer consumerCandidate)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);

    directorContainedProducer.registerConsumer(consumerCandidate);
}

template<T_ARGS_DECL>
template<class Producer, class Consumer>
void EventBroker<T_ARGS_DEF>::register_async_consumer(Consumer consumerCandidate)
{
    using ProducerWrapperType = typename ProducerWrapperSelector<Producer, std::false_type>::ProducerWrapperType;
    constexpr size_t producerIndex = CTimeUtils::Index<ProducerWrapperType, ProducersListType>::value;
    auto &directorContainedProducer = std::get<producerIndex>(m_directors);

    directorContainedProducer.registerConsumer(consumerCandidate);
}

template<T_ARGS_DECL>
void EventBroker<T_ARGS_DEF>::stop()
{
    m_stop.store(true);
    m_asyncQueueCond.notify_all();
}
template<T_ARGS_DECL>
template<class Task>
void EventBroker<T_ARGS_DEF>::postponeEvent(Task &&task)
{
    {
        std::unique_lock<std::mutex> l(m_asyncQueueMutex);
        m_asyncEventProcessingQueue.push_back(std::forward<Task>(task));
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
    while(!m_stop.load()) //todo
            {
                decltype(m_asyncEventProcessingQueue) processingQueue;
                {
                    std::unique_lock<std::mutex> l(m_asyncQueueMutex);
                    m_asyncQueueCond.wait(l, [this](){ return !m_asyncEventProcessingQueue.empty() or m_stop.load(); });
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
