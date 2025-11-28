#ifndef TEST_EF_CUSTOM_EVENT_SUBSCRIBER_H
#define TEST_EF_CUSTOM_EVENT_SUBSCRIBER_H


//specific events
#include <ttl/applications/EventFrameworkSpecialization/EventsImpl/MouseEvent.h>
#include <ttl/applications/EventFrameworkSpecialization/EventsImpl/KeyboardEvent.h>

#include <ttl/EventFramework/Interfaces/IControllable.hpp>

#include "CustomEvent.h"

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
/*Events for monitoring ---> */EventSubscriber, MouseEvent, KeyboardEvent, TestEvent
                            >
{
    template<class EventType>
    struct EventCounterHolder {
        using Event = EventType;
        size_t eventCounter = 0;
    };

    std::tuple<EventCounterHolder<MouseEvent>, EventCounterHolder<KeyboardEvent>, EventCounterHolder<TestEvent>> multiEventCounter;

    template<class EventType>
    size_t& getEventCounter() {
        return std::get<EventCounterHolder<EventType>>(multiEventCounter).eventCounter;
    }

    //Specific processing event methods, based on event type
    template<class ...Producer>
    urc::ResultDescription processSpecificEvent(const MouseEvent &event, MouseEventCMD type, Producer &&...producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<MouseEvent>()++;
        return urc::ResultDescription();
    }
    template<class ...Producer>
    urc::ResultDescription processSpecificEvent(const KeyboardEvent &event, KeyboardEventCMD type, Producer &&...producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        getEventCounter<KeyboardEvent>()++;
        return urc::ResultDescription();
    }
    //template<class Producer>
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
};

#endif // TEST_EF_CUSTOM_EVENT_SUBSCRIBER_H
