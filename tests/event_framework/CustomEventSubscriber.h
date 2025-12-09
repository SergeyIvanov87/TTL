#ifndef TEST_EF_CUSTOM_EVENT_SUBSCRIBER_H
#define TEST_EF_CUSTOM_EVENT_SUBSCRIBER_H


#include <ttl/EventFramework/Interfaces/IControllable.hpp>

#include "CustomEvent.h"
#include "CustomSecondEvent.h"

//Simple Producer class
struct EventProducerSimple : public IController<EventProducerSimple>
{
    size_t eventDelivered = 0;
};

struct EventAnotherProducerSimple : public IController<EventAnotherProducerSimple>
{
    size_t eventDelivered = 0;
};


//Subscriber class
struct EventSubscriber :
        public IControllable
                            <
/*Events for monitoring ---> */EventSubscriber, TestEvent, TestSecondEvent
                            >
{
    template<class EventType>
    struct EventCounterHolder {
        using Event = EventType;
        size_t eventCounter = 0;
    };

    std::tuple<EventCounterHolder<TestEvent>, EventCounterHolder<TestSecondEvent>> multiEventCounter;

    template<class EventType>
    size_t& getEventCounter() {
        return std::get<EventCounterHolder<EventType>>(multiEventCounter).eventCounter;
    }

    //Specific processing event methods, based on event type
    urc::ResultDescription processSpecificEvent(const TestEvent &event, CustomEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<TestEvent>()++;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestEvent &event, CustomEventCMD type, EventProducerSimple &producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<TestEvent>()++;

        //notify producer
        producer.eventDelivered++;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestEvent &event, CustomEventCMD type, EventAnotherProducerSimple &producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<TestEvent>()++;

        //notify producer
        producer.eventDelivered++;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestSecondEvent &event, CustomEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<TestSecondEvent>()++;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestSecondEvent &event, CustomEventCMD type, EventProducerSimple &producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<TestSecondEvent>()++;

        //notify producer
        producer.eventDelivered++;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestSecondEvent &event, CustomEventCMD type, EventAnotherProducerSimple &producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<TestSecondEvent>()++;

        //notify producer
        producer.eventDelivered++;
        return urc::ResultDescription();
    }
};

#endif // TEST_EF_CUSTOM_EVENT_SUBSCRIBER_H
