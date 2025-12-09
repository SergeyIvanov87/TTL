#ifndef MOUSE_COMMANDS_H
#define MOUSE_COMMANDS_H
#include <ttl/EventFramework/Interfaces/IEventFields.h>

struct MouseEventCMD: public IEventTriggerCommand<MouseEventCMD>
{
    enum ids
    {
        EMPTY,
        LOOK,
        MOUSE_BUTTON_EVENT,
        MAX_EVENT_TYPE
    };
    ids m_id;

    MouseEventCMD(size_t id = ids::EMPTY): m_id(static_cast<ids>(id)) {}
    static MouseEventCMD createFromStringImpl(const std::string &commandStr)
    {
        static const std::map<std::string, MouseEventCMD::ids> data
                        {
                            {TO_STRING(EMPTY), MouseEventCMD::EMPTY},
                            {TO_STRING(LOOK), MouseEventCMD::LOOK},
                            {TO_STRING(MOUSE_BUTTON_EVENT), MouseEventCMD::MOUSE_BUTTON_EVENT}
                        };

        MouseEventCMD ret;
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

    constexpr const char *toCStringImpl() const noexcept
    {
        using namespace Utils;
        switch(m_id)
        {
            case MouseEventCMD::EMPTY:
                return TO_STRING(EMPTY);
            case MouseEventCMD::LOOK:
                return TO_STRING(LOOK);
            case MouseEventCMD::MOUSE_BUTTON_EVENT:
                return TO_STRING(MOUSE_BUTTON_EVENT);
            default:
                assert(false);
        }
        return TO_STRING(EMPTY);
    }

    ids value() const noexcept
    {
        return m_id;
    }
};

#endif //MOUSE_COMMANDS_H
