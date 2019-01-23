#ifndef TEST_CUSTOM_EVENT_H
#define TEST_CUSTOM_EVENT_H

#include "CustomCommands.h"

#include "Framework/EventFramework/Interfaces/IBaseEvent.hpp"
#include "Framework/Utils/Utils.h"
#include "Framework/Utils/StringUtils.h"

//my Test Event
struct TestEventID : IEventField<TestEventID>
{
    friend class IEventField<TestEventID>;
    enum ids
    {
        TEID_1,
        TEID_2
    };
    ids m_id;

    constexpr TestEventID(size_t id = TestEventID::TEID_1) : m_id(TestEventID::ids(id)) {}

private:
    static TestEventID createFromStringImpl(const std::string &eventIdStr)
    {
        using namespace Utils;
        TestEventID ret(TestEventID::TEID_1);
        static const std::map<std::string, TestEventID::ids> data
                                    {
                                        {TO_STRING(TEID_1), TestEventID::TEID_1},
                                        {TO_STRING(TEID_2), TestEventID::TEID_2}
                                    };
        auto it = data.find(eventIdStr);
        if(it != data.end())
        {
            ret.m_id = std::get<1>(*it);
        }
        else
        {
            assert(false);
        }
        return ret;
    }

    constexpr const char* toCStringImpl() const
    {
        using namespace Utils;
        switch(m_id)
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

    std::pair<bool, std::string> validateImpl() const
    {
        return {true, ""};
    }

    ids valueImpl() const noexcept
    {
        return m_id;
    }
};

struct TestEventModifier : public IEventField<TestEventModifier>
{
    friend class IEventField<TestEventModifier>;
    enum ids
    {
        TEIM_NONE,
        TEIM_1,
        TEIM_2
    };

    ids m_id;

    constexpr TestEventModifier(size_t id = TestEventModifier::TEIM_NONE) : m_id(TestEventModifier::ids(id)) {}

private:
    static TestEventModifier createFromStringImpl(const std::string &keyMod)
    {
         static const std::map<std::string, TestEventModifier::ids> data
                            { {TO_STRING(TEIM_NONE), TestEventModifier::TEIM_NONE},
                              {TO_STRING(TEIM_1), TestEventModifier::TEIM_1},
                              {TO_STRING(TEIM_2), TestEventModifier::TEIM_2},
                            };


        TestEventModifier ret(TestEventModifier::TEIM_NONE);
        auto it = data.find(keyMod);
        if(it != data.end())
        {
            ret.m_id = std::get<1>(*it);
        }
        else
        {
            assert(false);
        }
        return ret;
    }

    constexpr const char* toCStringImpl() const
    {
        using namespace Utils;
        switch(m_id)
        {
            case TestEventModifier::TEIM_NONE:
                return TO_STRING(TEIM_NONE);
            case TestEventModifier::TEIM_1:
                return TO_STRING(TEIM_1);
            case TestEventModifier::TEIM_2:
                return TO_STRING(TEIM_2);
            default:
                assert(false);
        }
        return TO_STRING(TEIM_NONE);
    }

    ids valueImpl() const noexcept
    {
        return m_id;
    }
};

struct TestEvenState: public IEventField<TestEvenState>
{
    friend class IEventField<TestEvenState>;
    enum ids
    {
        TEIS_1,
        TEIS_2
    };
    ids m_id;

    constexpr TestEvenState(size_t id = TestEvenState::TEIS_1) : m_id(TestEvenState::ids(id)) {}

private:
    static TestEvenState createFromStringImpl(const std::string &state)
    {
        static const std::map<std::string, TestEvenState::ids> data
                            { {TO_STRING(TEIS_1), TestEvenState::TEIS_1},
                              {TO_STRING(TEIS_2), TestEvenState::TEIS_2}};

        TestEvenState ret(TestEvenState::TEIS_1);
        auto it = data.find(state);
        if(it != data.end())
        {
            ret.m_id = std::get<1>(*it);
        }
        else
        {
            assert(false);
        }
        return ret;
    }

    constexpr const char* toCStringImpl() const
    {
        using namespace Utils;
        switch(m_id)
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

        ids valueImpl() const noexcept
    {
        return m_id;
    }
};

struct TestEvent : public IBaseEvent<TestEvent, TestEventID, TestEventModifier, TestEvenState, CustomEventCMD>
{
    TestEvent (TestEventID id, TestEventModifier mod, TestEvenState state) :
        IBaseEvent<TestEvent, TestEventID, TestEventModifier, TestEvenState, CustomEventCMD>(id, mod, state)
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

    static constexpr TestEventModifier getEventModifierDefaultImpl()
    {
        return TestEventModifier(TestEventModifier::TEIM_NONE);
    }

    static constexpr TestEvenState getEventIdStateDefaultImpl()
    {
        TestEvenState ret(TestEvenState::TEIS_1);
        return ret;
    }

    std::string toStringImpl() const
    {
        return makeString(getEventTypeDescriptionImpl(),
                          "id=", getEventTypeCtrlId().toCString(),
                          "mod=", getEventTypeCtrlIdModifier().toCString(),
                          "st=",  getEventTypeCtrlIdState().toCString());
    }

};


#endif //TEST_CUSTOM_EVENT_H
