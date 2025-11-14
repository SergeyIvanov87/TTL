#ifndef TEST_EVENT_FRAMEWORK_FIXTURE
#define TEST_EVENT_FRAMEWORK_FIXTURE

#include <gtest/gtest.h>

#include <ttl/include/EventFramework/Interfaces/IEventConfigurator.hpp>
#include "CustomConfigurator.h"
#include "CustomEventSubscriber.h"

struct EFConfigurationFixture : public testing::Test {
    Configurator conf;
    virtual void SetUp() override {
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
    }

    virtual void TearDown() override {
        conf.configurationMap.clear();
    }
};

struct EFEventSubscriberFixture : public EFConfigurationFixture {
    EventSubscriber subscriber;

    virtual void SetUp() override {
        EFConfigurationFixture::SetUp();

        //logger
        Streamed<std::stringstream> ss;
        Tracer<Streamed<std::stringstream>> tracer(ss);

        //subscribe to events from config
        subscriber.loadControlEvents(conf, tracer);
        std::cout << ss.str() << std::endl;
    }

    void TearDown() override {
        EFConfigurationFixture::TearDown();
    }
};
#endif // TEST_EVENT_FRAMEWORK_FIXTURE
