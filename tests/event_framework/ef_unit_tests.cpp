#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include <chrono>

//Broker
#include <ttl/include/EventFramework/EventDirector.hpp>
#include <ttl/include/EventFramework/EventBroker.hpp>

#include <ttl/include/EventFramework/EventFramework.h>

#include "fixture.h"

TEST_F(EFConfigurationFixture, loading) {
}

using EventFramework = EventFrameworkFactory<MouseEvent, KeyboardEvent, TestEvent>;
TEST_F(EFEventSubscriberFixture, receiveSimpleEvent) {
    auto event = EventFramework::createControllerEvent<MouseEvent>(
                            0.0f, 0.0f, //{coordnates X,Y}
                            (int)MouseButton::MOUSE_MOVE,
                            0,  //key modifier
                            (int)MouseButtonState::MB_STATE_NONE);

    size_t prevConsumedEventCounter = subscriber.getEventCounter<MouseEvent>();
    subscriber.onProcessEventDispatcher(*event.get());
    EXPECT_EQ(subscriber.getEventCounter<MouseEvent>(), prevConsumedEventCounter + 1);
}

TEST_F(EFEventSubscriberFixture, checkEventRegistration) {
    auto event2Deliver = EventFramework::createControllerEvent<KeyboardEvent>(
                            0.0f, 0.0f, //{coordnates X,Y}
                            119, //key code
                            false, 0, //modifiers
                            KeyState::KEY_STATE_DOWN);

    size_t prevConsumedEventCounter = subscriber.getEventCounter<KeyboardEvent>();
    subscriber.onProcessEventDispatcher(*event2Deliver);
    EXPECT_EQ(subscriber.getEventCounter<KeyboardEvent>(), prevConsumedEventCounter + 1);

    event2Deliver = EventFramework::createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f,
                                115, false, 0,
                                KeyState::KEY_STATE_UP);

    prevConsumedEventCounter = subscriber.getEventCounter<KeyboardEvent>();
    subscriber.onProcessEventDispatcher(*event2Deliver);
    EXPECT_EQ(subscriber.getEventCounter<KeyboardEvent>(), prevConsumedEventCounter + 1);

    auto event2Skip = EventFramework::createControllerEvent<KeyboardEvent>(
                                0.0f, 0.0f,
                                666, false, 0,
                                KeyState::KEY_STATE_UP);

    prevConsumedEventCounter = subscriber.getEventCounter<KeyboardEvent>();
    subscriber.onProcessEventDispatcher(*event2Skip);
    EXPECT_NE(subscriber.getEventCounter<KeyboardEvent>(), prevConsumedEventCounter + 1);
    EXPECT_EQ(subscriber.getEventCounter<KeyboardEvent>(), prevConsumedEventCounter);
}


TEST_F(EFEventSubscriberFixture, checkEventSenderInterface) {
    auto event = EventFramework::createControllerEvent<TestEvent>(
                                TestEventID::TEID_1,
                                TestEventModifier::TEIM_NONE,
                                TestEvenState::TEIS_1);

    size_t prevConsumedEventCounter = subscriber.getEventCounter<TestEvent>();
    subscriber.onProcessEventDispatcher(*event.get());
    EXPECT_EQ(subscriber.getEventCounter<TestEvent>(), prevConsumedEventCounter + 1);

    EventProducerSimple producer;
    EXPECT_EQ(producer.eventDelivered, 0);
    size_t prevProducedEventCounter = producer.eventDelivered;
    event = EventFramework::createControllerEvent<TestEvent>(
                            TestEventID::TEID_1,
                            TestEventModifier::TEIM_NONE,
                            TestEvenState::TEIS_1);

    prevConsumedEventCounter = subscriber.getEventCounter<TestEvent>();
    subscriber.onProcessEventDispatcher(producer, *event.get());
    EXPECT_EQ(subscriber.getEventCounter<TestEvent>(), prevConsumedEventCounter + 1);
    EXPECT_EQ(producer.eventDelivered, prevProducedEventCounter + 1);
}


TEST_F(EFEventSubscriberFixture, checkEventBroker) {
    EventBroker<SyncEventDirector<EventProducerSimple, EventSubscriber>,
                AsyncEventDirector<EventProducerSimple, EventSubscriber>,
                AsyncEventDirector<EventAnotherProducerSimple, EventSubscriber>> broker;

    broker.register_sync_consumer<EventProducerSimple>(&subscriber);
    broker.register_async_consumer<EventProducerSimple>(&subscriber);


    EventProducerSimple producer;
    assert(!producer.eventDelivered);
    auto event = EventFramework::createControllerEvent<TestEvent>(
                            TestEventID::TEID_1,
                            TestEventModifier::TEIM_NONE,
                            TestEvenState::TEIS_1);

    //Sync case
    size_t prevConsumedEventCounter = subscriber.getEventCounter<TestEvent>();
    size_t prevProducedEventCounter = producer.eventDelivered;
    broker.push_sync_event(*event.get(), producer);
    EXPECT_EQ(subscriber.getEventCounter<TestEvent>(), prevConsumedEventCounter + 1); //OK, configured
    EXPECT_EQ(producer.eventDelivered, prevProducedEventCounter + 1);

    //Async case
    prevConsumedEventCounter = subscriber.getEventCounter<TestEvent>();
    prevProducedEventCounter = producer.eventDelivered;
    broker.push_async_event(std::move(*event.get()), producer);


    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);

    EXPECT_EQ(subscriber.getEventCounter<TestEvent>(), prevConsumedEventCounter + 1); //OK, configured
    EXPECT_EQ(producer.eventDelivered, prevProducedEventCounter + 1);


    EventAnotherProducerSimple producerAnother;
    broker.register_async_consumer<EventAnotherProducerSimple>(&subscriber);
    prevProducedEventCounter = producerAnother.eventDelivered;
    event = EventFramework::createControllerEvent<TestEvent>(
                            TestEventID::TEID_1,
                            TestEventModifier::TEIM_NONE,
                            TestEvenState::TEIS_1);

    prevConsumedEventCounter = subscriber.getEventCounter<TestEvent>();
    broker.push_async_event(std::move(*event.get()), producerAnother);

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(2s);

    EXPECT_EQ(subscriber.getEventCounter<TestEvent>(), prevConsumedEventCounter + 1); //OK, configured
    EXPECT_EQ(producerAnother.eventDelivered, prevProducedEventCounter + 1);
}
