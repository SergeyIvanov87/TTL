#ifndef TEST_CUSTOM_SECOND_EVENT_H
#define TEST_CUSTOM_SECOND_EVENT_H

#include "CustomCommands.h"

#include <ttl/EventFramework/Interfaces/IBaseEvent.hpp>
#include <ttl/Utils/Utils.h>
#include <ttl/Utils/StringUtils.h>

struct TestSecondEventID : IEventField<TestSecondEventID>
{
    friend class IEventField<TestSecondEventID>;
    enum ids
    {
        TEID_1,
        TEID_2
    };
    ids m_id;

    constexpr TestSecondEventID(size_t id = TestSecondEventID::TEID_1) : m_id(TestSecondEventID::ids(id)) {}

private:
    static TestSecondEventID createFromStringImpl(const std::string &eventIdStr)
    {
        using namespace Utils;
        TestSecondEventID ret(TestSecondEventID::TEID_1);
        static const std::map<std::string, TestSecondEventID::ids> data
                                    {
                                        {TO_STRING(TEID_1), TestSecondEventID::TEID_1},
                                        {TO_STRING(TEID_2), TestSecondEventID::TEID_2}
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
            case TestSecondEventID::TEID_1:
                return TO_STRING(TEID_1);
            case TestSecondEventID::TEID_2:
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

struct TestSecondEventModifier : public IEventField<TestSecondEventModifier>
{
    friend class IEventField<TestSecondEventModifier>;
    enum ids
    {
        TEIM_NONE,
        TEIM_1,
        TEIM_2
    };

    ids m_id;

    constexpr TestSecondEventModifier(size_t id = TestSecondEventModifier::TEIM_NONE) : m_id(TestSecondEventModifier::ids(id)) {}

private:
    static TestSecondEventModifier createFromStringImpl(const std::string &keyMod)
    {
         static const std::map<std::string, TestSecondEventModifier::ids> data
                            { {TO_STRING(TEIM_NONE), TestSecondEventModifier::TEIM_NONE},
                              {TO_STRING(TEIM_1), TestSecondEventModifier::TEIM_1},
                              {TO_STRING(TEIM_2), TestSecondEventModifier::TEIM_2},
                            };


        TestSecondEventModifier ret(TestSecondEventModifier::TEIM_NONE);
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
            case TestSecondEventModifier::TEIM_NONE:
                return TO_STRING(TEIM_NONE);
            case TestSecondEventModifier::TEIM_1:
                return TO_STRING(TEIM_1);
            case TestSecondEventModifier::TEIM_2:
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

struct TestSecondEvenState: public IEventField<TestSecondEvenState>
{
    friend class IEventField<TestSecondEvenState>;
    enum ids
    {
        TEIS_1,
        TEIS_2
    };
    ids m_id;

    constexpr TestSecondEvenState(size_t id = TestSecondEvenState::TEIS_1) : m_id(TestSecondEvenState::ids(id)) {}

private:
    static TestSecondEvenState createFromStringImpl(const std::string &state)
    {
        static const std::map<std::string, TestSecondEvenState::ids> data
                            { {TO_STRING(TEIS_1), TestSecondEvenState::TEIS_1},
                              {TO_STRING(TEIS_2), TestSecondEvenState::TEIS_2}};

        TestSecondEvenState ret(TestSecondEvenState::TEIS_1);
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
            case TestSecondEvenState::TEIS_1:
                return TO_STRING(TEIS_1);
            case TestSecondEvenState::TEIS_2:
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

struct TestSecondEvent : public IBaseEvent<TestSecondEvent, TestSecondEventID, TestSecondEventModifier, TestSecondEvenState, CustomEventCMD>
{
    TestSecondEvent (TestSecondEventID id, TestSecondEventModifier mod, TestSecondEvenState state) :
        IBaseEvent<TestSecondEvent, TestSecondEventID, TestSecondEventModifier, TestSecondEvenState, CustomEventCMD>(id, mod, state)
    {}

    //Interface methods
    /* -S- TODO It requires an interface different than IBaseEvent*/
    static constexpr ControlEventID getControlEventID()
    {
        return ControlEventID::TEST_EVENT;
    }


    static constexpr const char * getEventTypeDescriptionImpl()
    {
        return "TestSecondEvent";
    };

    static constexpr TestSecondEventModifier getEventModifierDefaultImpl()
    {
        return TestSecondEventModifier(TestSecondEventModifier::TEIM_NONE);
    }

    static constexpr TestSecondEvenState getEventIdStateDefaultImpl()
    {
        TestSecondEvenState ret(TestSecondEvenState::TEIS_1);
        return ret;
    }

    std::string toStringImpl() const
    {
        return makeString(getEventTypeDescriptionImpl(),
                          "{ id=", getEventTypeCtrlId().toCString(),
                          ", mod=", getEventTypeCtrlIdModifier().toCString(),
                          ", st=",  getEventTypeCtrlIdState().toCString(), "}");
    }

};


#endif //TEST_CUSTOM_SECOND_EVENT_H
