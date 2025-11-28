#ifndef EVENT_DIRECTOR_H
#define EVENT_DIRECTOR_H

struct EmptyProducer {};


template<class Consumer>
struct ConsumersSetWrapper
{
    template<class Producer, class Event>
    void produceEventForAllConsumers(Producer &producer, Event &event);
    std::set<std::add_pointer_t<Consumer>> m_sameTypeConsumers;
};

template<class Producer>
struct SyncProducerWrapper
{
    using ProducerType = Producer;
};

template<class Producer>
struct AsyncProducerWrapper
{
    using ProducerType = Producer;
};

template<class Producer, class SyncType = std::true_type, class Orphaned = std::false_type>
struct ProducerWrapperSelector
{
    using ProducerWrapperType = typename std::conditional<std::is_same_v<SyncType, std::true_type>,
                                                                SyncProducerWrapper<Producer>,
                                                                AsyncProducerWrapper<Producer>>::type;
};

template<class Producer, class ...Consumers>
struct IEventDirector
{
    using ConsumersListType = std::tuple<Consumers...>;
    using ConsumersTupleType = std::tuple<ConsumersSetWrapper<Consumers>...>;
    using ProducerType = Producer;

    enum
    {
        ConsumersCount =  std::tuple_size<ConsumersListType>::value
    };

    template<class Consumer>
    void registerConsumer(Consumer consumerCandidate);

protected:
    ConsumersTupleType m_consumers;
};

//thread unsafe - direct call
template<class Producer, class ...Consumers>
class SyncEventDirector: public IEventDirector<Producer, Consumers...>
{
public:
    using ProducerType = SyncProducerWrapper<Producer>;
    using ThisType = SyncEventDirector<Producer, Consumers...>;
    using Base = IEventDirector<Producer, Consumers...>;

    template<class Event>
    void produceEvent(Producer &producer, Event &&event);
};

//supportdeferred processing in another thread
template<class Producer, class ...Consumers>
class AsyncEventDirector : public IEventDirector<Producer, Consumers...>
{
public:
    using ProducerType = AsyncProducerWrapper<Producer>;
    using ThisType = AsyncEventDirector<Producer, Consumers...>;
    using Base = IEventDirector<Producer, Consumers...>;
    using QueueProcessingType = std::vector<std::packaged_task<void(void)>/*, Base::ConsumersCount + 1*/>; //+1 - is EventOwner queue element, see implementations
    //using QueueProcessingType = std::array<std::packaged_task<void(void)>/*, Base::ConsumersCount + 1*/>; //+1 - is EventOwner queue element, see implementations

    using AsyncTask = std::packaged_task<void(void)>;
    template<class Event>
    AsyncTask produceEvent(Producer &producer, Event &&event);
};

#endif
