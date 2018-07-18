#include <string>
#include <iostream>
#include <map>

#include "FrameworkSpecializations/EventFrameworkSpecialization/EventFrameworkSpecializationExport.h"
#include "Framework/EventFramework/Interfaces/IControllable.hpp"
#include "Framework/EventFramework/Interfaces/IEventConfigurator.hpp"

//g++ -std=c++17 test.cpp ../../FrameworkSpecializations/EventFrameworkSpecialization/EventsImpl/KeyboardEvent.cpp -I/home/user/microcontroller/git_hub/TTL/


//Just Stub configurator...
struct Configurator :
    public IEventConfigurator<Configurator>
{

    //Interface Impl
    template <class ConfigurationValueType>
    const ConfigurationValueType &getValueFromConfigurationImpl(const std::string &configurationName, const std::string &valueName) const
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

//Subscriber class
struct EventSubscriber :
        public IControllable
                            <
/*Events for monitoring ---> */EventSubscriber, MouseEvent, KeyboardEvent
                            >
{
    //Specific processing event methods, based on event type
    urc::ResultDescription processSpecificEvent(const MouseEvent &event, ControlEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << event.toString() << std::endl;

        //set flag for received event
        globalTestFlag_eventReceived = true;
        return urc::ResultDescription();
    }
    urc::ResultDescription processSpecificEvent(const KeyboardEvent &event, ControlEventCMD type)
    {
        std::cout << __PRETTY_FUNCTION__ << event.toString() << std::endl;

        //set flag for received event
        globalTestFlag_eventReceived = true;
        return urc::ResultDescription();
    }
};

int main(int argc, char ** argv)
{
    Configurator conf;
    conf.configurationMap.insert({"ControllableEvents", "KEYBOARD_EVENT,MOUSE_EVENT"});
    conf.configurationMap.insert({"KEYBOARD_EVENT", "MOVE_FORWARD,MOVE_BACKWARD,STRAFE_LEFT,STRAFE_RIGHT"});
    conf.configurationMap.insert({"MOVE_FORWARD", "w"});
    conf.configurationMap.insert({"MOVE_BACKWARD", "s"});
    conf.configurationMap.insert({"STRAFE_LEFT", "a"});
    conf.configurationMap.insert({"STRAFE_RIGHT", "d"});
    conf.configurationMap.insert({"MOUSE_EVENT", "LOOK"});
    conf.configurationMap.insert({"LOOK", "MOUSE_MOVE"});

    EventSubscriber t;

    //logger
    SSTracerImpl ss;
    Tracer<SSTracerImpl> tracer(ss);

    //subscribe to events from config
    t.loadControlEvents(conf, tracer);
    std::cout << ss.str() << std::endl;

    //Make test
    {
        auto event = createControllerEvent<MouseEvent>(
                                0.0f, 0.0f,
                                (int)MouseButton::MOUSE_MOVE,
                                0,
                                (int)MouseButtonState::MB_STATE_NONE);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
    }

    {
        auto event = createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f,
                                119, false, 0,
                                KeyState::KEY_STATE_DOWN);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
    }

    {
        auto event = createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f,
                                115, false, 0,
                                KeyState::KEY_STATE_UP);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(globalTestFlag_eventReceived); //OK, configured
    }

    {
        auto event = createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f,
                                666, false, 0,
                                KeyState::KEY_STATE_UP);

        globalTestFlag_eventReceived = false;
        t.onProcessEventDispatcher(*event.get());
        assert(!globalTestFlag_eventReceived); //Not delivered unknow event
    }
    return 0;
}
