#include <string>
#include <iostream>
#include <map>
#include <chrono>

//Broker
#include <ttl/include/EventFramework/EventDirector.hpp>
#include <ttl/include/EventFramework/EventBroker.hpp>

#include <ttl/include/EventFramework/EventFramework.h>

//specific events
#include <ttl/applications/EventFrameworkSpecialization/EventsImpl/MouseEvent.h>
#include <ttl/applications/EventFrameworkSpecialization/EventsImpl/KeyboardEvent.h>

#include <ttl/include/EventFramework/Interfaces/IControllable.hpp>
//#include <ttl/include/EventFramework/Interfaces/IController.hpp>

#include "CustomEvent.h"



//g++ -std=c++17 test.cpp ../../FrameworkSpecializations/EventFrameworkSpecialization/EventsImpl/KeyboardEvent.cpp -I/home/user/microcontroller/git_hub/TTL/ -lpthread

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
        public IControllable
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
    //template<class Producer>
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
    Streamed<std::stringstream> ss;
    Tracer<Streamed<std::stringstream>> tracer(ss);

    //subscribe to events from config
    t.loadControlEvents(conf, tracer);
    std::cout << ss.str() << std::endl;

    //Make test
    {
        auto event = EventFramework::createControllerEvent<MouseEvent>(
                                0.0f, 0.0f, //{coordnates X,Y}
                                (int)MouseButton::MOUSE_MOVE,
                                0,  //key modifier
                                (int)MouseButtonState::MB_STATE_NONE);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
    }

    {
        auto event = EventFramework::createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f, //{coordnates X,Y}
                                119, //key code
                                false, 0, //modifiers
                                KeyState::KEY_STATE_DOWN);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
    }

    {
        auto event = EventFramework::createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f,
                                115, false, 0,
                                KeyState::KEY_STATE_UP);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
    }

    {
        auto event = EventFramework::createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f,
                                666, false, 0,
                                KeyState::KEY_STATE_UP);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(!globalTestFlag_eventReceived); //Not delivered unknown event
    }

    {
        auto event = EventFramework::createControllerEvent<TestEvent>(
                                TestEventID::TEID_1,
                                TestEventModifier::TEIM_NONE,
                                TestEvenState::TEIS_1);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
    }

    {
        EventProducerSimple producer;
        assert(!producer.eventDelivered);
        auto event = EventFramework::createControllerEvent<TestEvent>(
                                TestEventID::TEID_1,
                                TestEventModifier::TEIM_NONE,
                                TestEvenState::TEIS_1);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(producer, *event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
        assert(producer.eventDelivered);
    }


    {
        //Test EventBroker -- simple
        EventBroker<SyncEventDirector<EventProducerSimple, EventSubscriber>,
                    AsyncEventDirector<EventProducerSimple, EventSubscriber>,
                    AsyncEventDirector<EventAnotherProducerSimple, EventSubscriber>> broker;

        broker.register_sync_consumer<EventProducerSimple>(&t);
        broker.register_async_consumer<EventProducerSimple>(&t);


        EventProducerSimple producer;
        assert(!producer.eventDelivered);
        auto event = EventFramework::createControllerEvent<TestEvent>(
                                TestEventID::TEID_1,
                                TestEventModifier::TEIM_NONE,
                                TestEvenState::TEIS_1);

        //Sync case
        globalTestFlag_eventReceived = false;
        producer.eventDelivered = false;
        broker.push_sync_event(*event.get(), producer);
        assert(globalTestFlag_eventReceived); //OK, configured
        assert(producer.eventDelivered);

        //Async case
        globalTestFlag_eventReceived = false;
        producer.eventDelivered = false;
        broker.push_async_event(std::move(*event.get()), producer);


        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        assert(globalTestFlag_eventReceived); //OK, configured
        assert(producer.eventDelivered);


        EventAnotherProducerSimple producerAnother;
        broker.register_async_consumer<EventAnotherProducerSimple>(&t);
        assert(!producerAnother.eventDelivered);
        event = EventFramework::createControllerEvent<TestEvent>(
                                TestEventID::TEID_1,
                                TestEventModifier::TEIM_NONE,
                                TestEvenState::TEIS_1);

        globalTestFlag_eventReceived = false;
        broker.push_async_event(std::move(*event.get()), producerAnother);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        assert(globalTestFlag_eventReceived); //OK, configured
        assert(producerAnother.eventDelivered);

    }
    return 0;
}
