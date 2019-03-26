#ifndef EVENT_DIRECTOR_HPP
#define EVENT_DIRECTOR_HPP
#include <tuple>
#include <set>
#include <array>
#include <vector>
#include <functional>
#include <future>
#include <type_traits>
#include "EventDirector.h"
#include "Framework/Utils/CTimeUtils.h"

template<class Consumer>
template<class Producer, class Event>
void ConsumersSetWrapper<Consumer>::produceEventForAllConsumers(Producer &producer, Event &event)
{
    for( auto c : m_sameTypeConsumers)
    {
        if constexpr(std::is_same_v<Producer,EmptyProducer>)
        {
            c->onProcessEventDispatcher(event);
        }
        else
        {
            c->onProcessEventDispatcher(producer, event);
        }
    }
}

#define T_ARGS_DECL         class Producer, class ...Consumers
#define T_ARGS_DEF          Producer, Consumers...


template<T_ARGS_DECL>
template<class Consumer>
void IEventDirector<T_ARGS_DEF>::registerConsumer(Consumer consumerCandidate)
{
    std::get<ConsumersSetWrapper<std::remove_pointer_t<Consumer>>>(m_consumers).m_sameTypeConsumers.insert(consumerCandidate);
}


template<T_ARGS_DECL>
template<class Event>
void SyncEventDirector<T_ARGS_DEF>::produceEvent(Producer &producer, Event &&event)
{
    std::apply(
    [this, &producer, &event]
    (auto &...x)
    {
        bool dispatchingResult[]
        {
                 (x.produceEventForAllConsumers(producer, event), true)...
        };

    }, this->m_consumers);
};

template<class Event>
void eventOwner(Event &&event)
{
    (void)event;
}

template<T_ARGS_DECL>
template<class Event>
typename AsyncEventDirector<T_ARGS_DEF>::AsyncTask
    AsyncEventDirector<T_ARGS_DEF>::produceEvent(Producer &producer, Event &&event)
{

    /* own event a object - to avoid danging reference.
     * This callback should be called at chain finish,
     * to allow actual event processing callback finish its execution
     */
   /* ret[0] = [holdedEvent = std::forward<Event>(event)]() -> void
    {
        (void)holdedEvent;
    };
*/


/*
ret.push_back([holdedEvent = std::move(event)]() mutable
    {
        eventOwner(std::move(holdedEvent));
    });
*/
    using Processors = std::array<std::function<void(Event &)>, Base::ConsumersCount + 1>;
    Processors processors;
    CTimeUtils::for_each_in_tuple(this->m_consumers,
            [this, &producer, &event, &processors](size_t index, auto &dst)
        {
            //compare requested id and existed id
            typedef typename std::remove_reference<decltype(dst)>::type NonRefType;
            processors[index] = (std::bind(&NonRefType::template produceEventForAllConsumers<Producer, Event>, &dst, std::ref(producer), std::placeholders::_1));

        });
        /*
    std::apply(
    [this, &producer, &event, &ret]
    (auto &...x)
    {
        //typedef typename std::remove_reference<decltype(x)>::type NonRefType;
        size_t index = 0;
        bool dispatchingResult[]
        {
                 (ret[index++] = std::bind(
                                        typename std::remove_reference<decltype(x)>::type::template produceEventForAllConsumers<Producer, Event>, &x, std::ref(producer), std::ref(event)))...

        };

    }, m_consumers);*/

    return AsyncTask(std::bind([](Event &ev, Processors &pr)
    {
        for(auto &p : pr)
        {
            p(ev);
        }
    }, std::move(event), std::move(processors)));
}


#undef T_ARGS_DEF
#undef T_ARGS_DECL
#endif
