#include <string>
#include <iostream>
#include <map>
#include <chrono>

//Broker
#include <ttl/include/EventFramework/EventDirector.hpp>
#include <ttl/include/EventFramework/EventBroker.hpp>

#include "ttl/include/EventFramework/EventFramework.h"

//specific events
#include "FrameworkSpecializations/EventFrameworkSpecialization/EventsImpl/MouseEvent.h"
#include "FrameworkSpecializations/EventFrameworkSpecialization/EventsImpl/KeyboardEvent.h"

#include <ttl/include/EventFramework/Interfaces/IEventConsumer.hpp>
//#include <ttl/include/EventFramework/Interfaces/IController.hpp>

#include "CustomEvent.h"



//g++ -std=c++17 event_broker_test.cpp ../../FrameworkSpecializations/EventFrameworkSpecialization/EventsImpl/KeyboardEvent.cpp -I/home/user/microcontroller/git_hub/TTL/ -lpthread

//Just Stub configurator...
struct Configurator :
    public IEventConfigurator<Configurator>
{

    //Interface Impl
    template <class ConfigurationValueType>
    ConfigurationValueType getValueFromConfigurationImpl(const std::string &configurationName, const std::string &valueName) const
    {
        assert(configurationName == "ControllableEvents");

        auto it = configurationMap.find(valueName);
        if(it == configurationMap.end())
        {
            static const std::string empty;
            return empty;
        }
        return it->second;
    }

    template <class ConfigurationValueType>
    std::unordered_set<ConfigurationValueType> getMultipleValuesFromConfigurationImpl(const std::string &configurationName, const std::string &valueName) const
    {
        assert(configurationName == "ControllableEvents");

        std::unordered_set<std::string> ret;
        auto range = configurationMap.equal_range(valueName);
        for(auto it = range.first; it != range.second; ++it)
        {
            ret.insert(it->second);
        }
        return ret;
    }

    //Store configuration parameters here
    std::multimap<std::string, std::string> configurationMap;
};

bool globalTestFlag_eventReceived = false;

//Simpple Producer class
struct EventProducerSimple : public IController<EventProducerSimple>
{
    bool eventDelivered = false;
};

struct EventAnotherProducerSimple : public IController<EventAnotherProducerSimple>
{
    bool eventDelivered = false;
};


//Subscriber class
struct EventSubscriber :
        public IEventConsumer
                            <
/*Events for monitoring ---> */EventSubscriber, MouseEvent, KeyboardEvent, TestEvent
                            >
{
    //Specific processing event methods, based on event type
    template<class ...Producer>
    urc::ResultDescription processSpecificEvent(const MouseEvent &event, MouseEventCMD type, Producer &&...producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;

        //set flag for received event
        globalTestFlag_eventReceived = true;
        return urc::ResultDescription();
    }
    template<class ...Producer>
    urc::ResultDescription processSpecificEvent(const KeyboardEvent &event, KeyboardEventCMD type, Producer &&...producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;

        //set flag for received event
        globalTestFlag_eventReceived = true;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestEvent &event, CustomEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;

        //set flag for received event
        globalTestFlag_eventReceived = true;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestEvent &event, CustomEventCMD type, EventProducerSimple &producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;

        //set flag for received event
        globalTestFlag_eventReceived = true;

        //notify producer
        producer.eventDelivered = true;
        return urc::ResultDescription();
    }

    urc::ResultDescription processSpecificEvent(const TestEvent &event, CustomEventCMD type, EventAnotherProducerSimple &producer)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;

        //set flag for received event
        globalTestFlag_eventReceived = true;

        //notify producer
        producer.eventDelivered = true;
        return urc::ResultDescription();
    }
};
struct EventSubscriberNext
{
    //Specific processing event methods, based on event type
    template<class Producer>
    urc::ResultDescription onProcessEventDispatcher(Producer &&producer, const TestEvent &event)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        return urc::ResultDescription();
    }

    urc::ResultDescription onProcessEventDispatcher(const TestEvent &event)
    {
        std::cout << __PRETTY_FUNCTION__ << ", thread_id: " << std::this_thread::get_id() << ", " << event.toString() << std::endl;
        return urc::ResultDescription();
    }
};

int main(int argc, char ** argv)
{
    Configurator conf;
    conf.configurationMap.insert({"ControllableEvents", "KEYBOARD_EVENT,MOUSE_EVENT,TestEvent"});
    conf.configurationMap.insert({"KEYBOARD_EVENT", "MOVE_FORWARD,MOVE_BACKWARD,STRAFE_LEFT,STRAFE_RIGHT"});
    conf.configurationMap.insert({"MOVE_FORWARD", "w"});
    conf.configurationMap.insert({"MOVE_BACKWARD", "s"});
    conf.configurationMap.insert({"STRAFE_LEFT", "a"});
    conf.configurationMap.insert({"STRAFE_RIGHT", "d"});
    conf.configurationMap.insert({"MOUSE_EVENT", "LOOK"});
    conf.configurationMap.insert({"LOOK", "MOUSE_MOVE"});
    conf.configurationMap.insert({"TestEvent", "TEID_1_CMD"});
    conf.configurationMap.insert({"TEID_1_CMD", "TEID_1"});

    using EventFramework = EventFrameworkFactory<MouseEvent, KeyboardEvent, TestEvent>;
    EventSubscriber t;

    //logger
    Streamed ss;
    Tracer<Streamed> tracer(ss);

    //subscribe to events from config
    t.loadControlEvents(conf, tracer);
    std::cout << ss.str() << std::endl;

    //Make test
    {
        //Test EventBroker -- simple
        EventBroker</*SyncEventDirector<EventProducerSimple, EventSubscriber>,
                    AsyncEventDirector<EventProducerSimple, EventSubscriber>,
                    AsyncEventDirector<EventAnotherProducerSimple, EventSubscriber>,*/
                    SyncEventDirector<EmptyProducer, EventSubscriber,EventSubscriberNext>,
                    SyncEventDirector<EventProducerSimple, EventSubscriberNext>> broker;

        //broker.register_sync_consumer<EventProducerSimple>(&t);
        //broker.register_async_consumer<EventProducerSimple>(&t);
        broker.register_sync_consumer<EmptyProducer>(&t);
        EventSubscriberNext next;
        broker.register_sync_consumer<EmptyProducer>(&next);
        broker.register_sync_consumer<EventProducerSimple>(&next);

        TestEvent event(TestEventID::TEID_1, TestEventModifier::TEIM_NONE, TestEvenState::TEIS_1);

        //event by reference case, orphaned
        globalTestFlag_eventReceived = false;
        broker.push_sync_event(event);

        EventProducerSimple producer;
        broker.push_sync_event(event, producer);
        assert(globalTestFlag_eventReceived); //OK, configured
    return 0;
    /*


        EventProducerSimple producer;
        assert(!producer.eventDelivered);

        //event by reference case
        globalTestFlag_eventReceived = false;
        producer.eventDelivered = false;
        broker.push_sync_event(event, producer);
        assert(globalTestFlag_eventReceived); //OK, configured
        assert(producer.eventDelivered);

        //move event
        globalTestFlag_eventReceived = false;
        producer.eventDelivered = false;
        broker.push_sync_event(std::move(event),producer);
        assert(globalTestFlag_eventReceived); //OK, configured
        assert(producer.eventDelivered);

        //Async case - TestEvent
        TestEvent asyncEvent(TestEventID::TEID_1, TestEventModifier::TEIM_NONE, TestEvenState::TEIS_1);
        globalTestFlag_eventReceived = false;
        producer.eventDelivered = false;
        broker.push_async_event(std::move(event), producer);

        //Async case - Mouse
        MouseEvent asyncMouseEvent(
                                0.0f, 0.0f, //{coordnates X,Y}
                                (int)MouseButton::MOUSE_MOVE,
                                0,  //key modifier
                                (int)MouseButtonState::MB_STATE_NONE);

        globalTestFlag_eventReceived = false;
        producer.eventDelivered = false;
        broker.push_async_event(std::move(asyncMouseEvent), producer);


        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        assert(globalTestFlag_eventReceived); //OK, configured
        assert(producer.eventDelivered);
        * */
    }
    return 0;
}
