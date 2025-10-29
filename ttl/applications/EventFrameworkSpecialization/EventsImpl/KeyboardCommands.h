#ifndef KEYBOARD_COMMANDS_H
#define KEYBOARD_COMMANDS_H

#include <ttl/include/EventFramework/Interfaces/IEventFields.h>

struct KeyboardEventCMD : public IEventTriggerCommand<KeyboardEventCMD>
{
    enum ids
    {
        EMPTY,
        MOVE_FORWARD,
        MOVE_BACKWARD,
        STRAFE_LEFT,
        STRAFE_RIGHT,

        MAX_EVENT_TYPE
    };
    ids m_id;

    KeyboardEventCMD(size_t id = ids::EMPTY): m_id(static_cast<ids>(id)) {}
    static KeyboardEventCMD createFromStringImpl(const std::string &commandStr)
    {
        static const std::map<std::string, KeyboardEventCMD::ids> data
                        {
                            {TO_STRING(EMPTY), KeyboardEventCMD::EMPTY},
                            {TO_STRING(MOVE_FORWARD), KeyboardEventCMD::MOVE_FORWARD},
                            {TO_STRING(MOVE_BACKWARD), KeyboardEventCMD::MOVE_BACKWARD},
                            {TO_STRING(STRAFE_LEFT), KeyboardEventCMD::STRAFE_LEFT},
                            {TO_STRING(STRAFE_RIGHT), KeyboardEventCMD::STRAFE_RIGHT},
                        };

        KeyboardEventCMD ret;
        auto it = data.find(commandStr);
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

    inline constexpr const char *toCStringImpl() const noexcept
    {
        using namespace Utils;
        switch(m_id)
        {
            case KeyboardEventCMD::EMPTY:
                return TO_STRING(EMPTY);
            case KeyboardEventCMD::MOVE_FORWARD:
                return TO_STRING(MOVE_FORWARD);
            case KeyboardEventCMD::MOVE_BACKWARD:
                return TO_STRING(MOVE_BACKWARD);
            case KeyboardEventCMD::STRAFE_LEFT:
                return TO_STRING(STRAFE_LEFT);
            case KeyboardEventCMD::STRAFE_RIGHT:
                return TO_STRING(STRAFE_RIGHT);
            default:
                assert(false);
        }
        return TO_STRING(EMPTY);
    }

    ids valueImpl() const noexcept
    {
        return m_id;
    }
};

#endif //KEYBOARD_COMMANDS_H
