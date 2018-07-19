#ifndef TEST_CUSTOM_EVENT_H
#define TEST_CUSTOM_EVENT_H


#include "Framework/EventFramework/Interfaces/IBaseEvent.hpp"
#include "Framework/Utils/Utils.h"
#include "Framework/Utils/StringUtils.h"

//my Test Event
enum TestEventID
{
    TEID_1,
    TEID_2
};
enum TestEventModifier
{
    TEIM_1,
    TEIM_2
};
enum TestEvenState
{
    TEIS_1,
    TEIS_2
};

struct TestEvent : public IBaseEvent<TestEvent, TestEventID, TestEventModifier, TestEvenState>
{
    TestEvent (TestEventID id, TestEventModifier mod, TestEvenState state) :
        IBaseEvent<TestEvent, TestEventID, TestEventModifier, TestEvenState>(id, mod, state)
    {}

    //Interface methods
    static constexpr ControlEventID getControlEventID()
    {
        return ControlEventID::TEST_EVENT;
    }


    static constexpr const char * getEventTypeDescriptionImpl()
    {
        return "TestEvent";
    };

    static TestEventID String2EventIdImpl(const std::string &eventIdStr)
    {
        using namespace Utils;
        TestEventID ret = TestEventID::TEID_1;
        static const std::map<std::string, TestEventID> data
                                    { {TO_STRING(TEID_1), TestEventID::TEID_1},
                                      {TO_STRING(TEID_2), TestEventID::TEID_2}
                                    };
        auto it = data.find(eventIdStr);
        if(it != data.end())
        {
            ret = std::get<1>(*it);
        }
        else
        {
            assert(false);
        }
        return ret;
    }

    static constexpr const char* EventId2StringImpl(TestEventID eventId)
    {
        using namespace Utils;
        switch(eventId)
        {
            case TestEventID::TEID_1:
                return TO_STRING(TEID_1);
            case TestEventID::TEID_2:
                return TO_STRING(TEID_2);
            default:
                assert(false);
        }
        return TO_STRING(TEID_1);
    }

    static constexpr TestEvenState getEventIdStateDefaultImpl()
    {
        return TestEvenState::TEIS_1;
    }

    static TestEvenState String2EventIdStateImpl(const std::string &state)
    {

        static const std::map<std::string, TestEvenState> data
                            { {TO_STRING(TEIS_1), TestEvenState::TEIS_1},
                              {TO_STRING(TEIS_2), TestEvenState::TEIS_2}};

        TestEvenState ret;
        auto it = data.find(state);
        if(it != data.end())
        {
            ret = std::get<1>(*it);
        }
        else
        {
            assert(false);
        }
        return ret;
    }

    static constexpr const char* EventIdState2StringImpl(EventIdState state)
    {
        using namespace Utils;
        switch(state)
        {
            case TestEvenState::TEIS_1:
                return TO_STRING(TEIS_1);
            case TestEvenState::TEIS_2:
                return TO_STRING(TEIS_2);
            default:
                assert(false);
        }
        return TO_STRING(TEIS_1);
    }

    std::string toStringImpl() const
    {
        return makeString(getEventTypeDescriptionImpl(),
                          "id=", EventId2StringImpl(getEventTypeCtrlId()),
                          "mod=", getEventTypeCtrlIdModifier(),
                          "st=",  EventIdState2StringImpl(getEventTypeCtrlIdState()));
    }

};


#endif //TEST_CUSTOM_EVENT_H

