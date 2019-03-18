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
template<class Event>
std::array<std::function<void(void)>,
              EventDirector<T_ARGS_DEF>::ConsumersCount>
    EventDirector<T_ARGS_DEF>::produceEventDeferred(Producer &producer, Event &&event)
{
    std::array<std::function<void(void)>,
               ConsumersCount> ret;

CTimeUtils::for_each_in_tuple(m_consumers,
            [this, &producer, &event, &ret](size_t index, auto &dst)
        {
            //compare requested id and existed id
            typedef typename std::remove_reference<decltype(dst)>::type NonRefType;
            ret[index] = std::bind(&NonRefType::template produceEventForAllConsumers<Producer, Event>, &dst, std::ref(producer), std::ref(event));

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
    return ret;
}
template<T_ARGS_DECL>
template<class Consumer>
void EventDirector<T_ARGS_DEF>::registerConsumer(Consumer consumerCandidate)
{
    std::get<Wrapper<std::remove_pointer_t<Consumer>>>(m_consumers).m_sameTypeConsumers.insert(consumerCandidate);
}

#undef T_ARGS_DEF
#undef T_ARGS_DECL
#endif
