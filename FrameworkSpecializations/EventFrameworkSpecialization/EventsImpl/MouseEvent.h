#ifndef MOUSE_EVENTS_H
#define MOUSE_EVENTS_H
#include <GL/glew.h>
#ifdef FREEGLUT
    #include <GL/freeglut.h>
#else
    #include <GL/glut.h>
#endif

#include <GL/gl.h>
#include <memory>
#include "MouseCommands.h"
#include "../EventIDsDeclaration.h"
#include "EventIdModifierSpecific.h"
#include "Framework/EventFramework/Interfaces/IBaseEvent.hpp"


struct MouseButton : public IEventField<MouseButton>
{
    friend class IEventField<MouseButton>;
    enum ids
    {
        MB_BUTTON_NONE = -1,
        MB_LEFT = GLUT_LEFT_BUTTON,
        MB_MIDDLE = GLUT_MIDDLE_BUTTON,
        MB_RIGHT = GLUT_RIGHT_BUTTON,
        MOUSE_MOVE,
        MB_BUTTON_MAX
    };

    ids m_id;
    MouseButton(size_t id = MouseButton::MB_BUTTON_NONE) : m_id(MouseButton::ids(id)) {}

private:
    constexpr const char *toCStringImpl() const noexcept
    {
        switch(m_id)
        {
            case MouseButton::MB_BUTTON_NONE:
                return "";
            case MouseButton::MB_LEFT:
                return TO_STRING(MB_LEFT);
            case MouseButton::MB_RIGHT:
                return TO_STRING(MB_RIGHT);
            case MouseButton::MB_MIDDLE:
                return TO_STRING(MB_MIDDLE);
            case MouseButton::MOUSE_MOVE:
                return TO_STRING(MOUSE_MOVE);
            default:
                assert(false);
                break;
        }
        return "";
    }

    ids valueImpl() const noexcept
    {
        return m_id;
    }

    static MouseButton createFromStringImpl(const std::string &eventIdStr)
    {
        MouseButton ret;
        static const std::map<std::string, MouseButton::ids> data {
                                      {TO_STRING(MB_LEFT), MouseButton::MB_LEFT},
                                      {TO_STRING(MB_RIGHT), MouseButton::MB_RIGHT},
                                      {TO_STRING(MB_MIDDLE), MouseButton::MB_MIDDLE},
                                      {TO_STRING(MOUSE_MOVE), MouseButton::MOUSE_MOVE} };
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
};

struct MouseButtonState : public IEventField<MouseButtonState>
{
    friend class IEventField<MouseButtonState>;

    enum ids
    {
        MB_STATE_NONE = -1,
        MB_DOWN = GLUT_DOWN,
        MB_UP = GLUT_UP,
        MB_STATE_MAX,
    };

    ids m_id;
    constexpr MouseButtonState(size_t id = MouseButtonState::MB_STATE_NONE) : m_id(MouseButtonState::ids(id)) {}

private:
    ids valueImpl() const noexcept
    {
        return m_id;
    }

    constexpr const char* toCStringImpl() const noexcept
    {
        switch(m_id)
        {
            case MouseButtonState::MB_STATE_NONE:
                return "";
            case MouseButtonState::MB_DOWN:
                return TO_STRING(MB_DOWN);
            case MouseButtonState::MB_UP:
                return TO_STRING(MB_UP);
            default:
                assert(false);
                break;
        }
        return "";
    }

    static MouseButtonState String2EventIdStateImpl(const std::string &state)
    {
        static const std::map<std::string, MouseButtonState::ids> data
                            { {TO_STRING(MB_DOWN), MouseButtonState::MB_DOWN},
                              {TO_STRING(MB_UP), MouseButtonState::MB_UP}};

        MouseButtonState ret(MouseButtonState::MB_STATE_NONE);
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
};


//Aggregated Event Definition
class MouseEvent : public IBaseEvent<MouseEvent,
                                     MouseButton, KeyModifier, MouseButtonState, MouseEventCMD>
{
public:
    using BaseType = IBaseEvent<MouseEvent,
                                MouseButton, KeyModifier, MouseButtonState, MouseEventCMD>;

    //total ctor
    MouseEvent(GLfloat _x, GLfloat _y, MouseButton mouseButton, MouseButtonState mouseButtonState) :
        BaseType(mouseButton, KeyModifier::NONE_MOD_KEY, mouseButtonState),
        x(_x), y(_y)
    {
    }
    MouseEvent(GLfloat _x, GLfloat _y, MouseButton mouseButton, KeyModifier keyMod, MouseButtonState mouseButtonState) :
        BaseType(mouseButton,
                keyMod,
                mouseButtonState),
        x(_x), y(_y)
    {
    }
    ~MouseEvent() = default;

    GLfloat getPointerX() const {return x;}
    GLfloat getPointerY() const {return y;}

    //Interface Impl
    static constexpr ControlEventID getControlEventID()
    {
        return ControlEventID::MOUSE_EVENT;
    }

    static constexpr const char * getEventTypeDescriptionImpl()
    {
        return TO_STRING(MOUSE_EVENT);
    }

    std::string toStringImpl() const
    {
        std::string result("[");
        result = result + getEventTypeCtrlId().toCString() + ", " +
                getEventTypeCtrlIdModifier().toCString() + ", " +
                getEventTypeCtrlIdState().toCString() + "] x=" +
                std::to_string(x) + ", y=" + std::to_string(y);
        return result;
    }
private:
    GLfloat x, y;
};


#endif
