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
#include "../EventIDsDeclaration.h"
#include "EventIdModifierSpecific.h"
#include "Framework/Base/EventFramework/Interfaces/IBaseEvent.hpp"


    enum class MouseButton
    {
        MB_BUTTON_NONE = -1,
        MB_LEFT = GLUT_LEFT_BUTTON,
        MB_MIDDLE = GLUT_MIDDLE_BUTTON,
        MB_RIGHT = GLUT_RIGHT_BUTTON,
        /*MOUSE_MOVE_DOWN = 0X00E0,
        MOUSE_MOVE_UP = 0X00F0,
        MOUSE_MOVE_LEFT = 0X0E00,
        MOUSE_MOVE_RIGHT = 0X0F00,
        */
        MOUSE_MOVE,
        MB_BUTTON_MAX
    };

    enum class MouseButtonState
    {
        MB_STATE_NONE = -1,
        MB_DOWN = GLUT_DOWN,
        MB_UP = GLUT_UP,
        MB_STATE_MAX,
    };

class MouseEvent : public IBaseEvent<MouseEvent,
                                     MouseButton, KeyModifier, MouseButtonState>
{
public:
    using BaseType = IBaseEvent<MouseEvent,
                                MouseButton, KeyModifier, MouseButtonState>;

    //total ctor
    //1)
    MouseEvent(GLfloat _x, GLfloat _y, int mouseButton, int mouseButtonState) :
        BaseType(static_cast<MouseButton>(mouseButton),
                KeyModifier::NONE_MOD_KEY,
                static_cast<MouseButtonState>(mouseButtonState)),
        x(_x), y(_y)
    {
    }
    //1)*
    MouseEvent(GLfloat _x, GLfloat _y, MouseButton mouseButton, MouseButtonState mouseButtonState) :
        BaseType(mouseButton, KeyModifier::NONE_MOD_KEY, mouseButtonState),
        x(_x), y(_y)
    {
    }
    //2)
    MouseEvent(GLfloat _x, GLfloat _y, int mouseButton, int keyMod, int mouseButtonState) :
        BaseType(static_cast<MouseButton>(mouseButton),
                static_cast<KeyModifier>(keyMod),
                static_cast<MouseButtonState>(mouseButtonState)),
        x(_x), y(_y)
    {
    }
    //2)*
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
        return getObserverEventTypeString(getControlEventID());
    }

    static constexpr const char *EventId2StringImpl(MouseButton eventId)
    {
        switch(eventId)
        {
            case MouseButton::MB_BUTTON_NONE:
                return "";
            case MouseButton::MB_LEFT:
                return TO_STRING(MB_LEFT);
            case MouseButton::MB_RIGHT:
                return TO_STRING(MB_RIGHT);
            case MouseButton::MB_MIDDLE:
                return TO_STRING(MB_MIDDLE);
                /*
            case MouseButton::MOUSE_MOVE_DOWN:
                return TO_STRING(MOUSE_MOVE_DOWN);
            case MouseButton::MOUSE_MOVE_UP:
                return TO_STRING(MOUSE_MOVE_UP);
            case MouseButton::MOUSE_MOVE_LEFT:
                return TO_STRING(MOUSE_MOVE_LEFT);
            case MouseButton::MOUSE_MOVE_RIGHT:
                return TO_STRING(MOUSE_MOVE_RIGHT);
                */
            case MouseButton::MOUSE_MOVE:
                return TO_STRING(MOUSE_MOVE);
            default:
                assert(false);
                break;
        }
        return "";
    }
    static constexpr MouseButtonState getEventIdStateDefaultImpl() {return MouseButtonState::MB_STATE_NONE; };
    static MouseButton String2EventIdImpl(const std::string &eventIdStr)
    {
        MouseButton ret = MouseButton::MB_BUTTON_NONE;
        static const std::map<std::string, MouseButton> data
                                    { {TO_STRING(MB_LEFT), MouseButton::MB_LEFT},
                                      {TO_STRING(MB_RIGHT), MouseButton::MB_RIGHT},
                                      {TO_STRING(MB_MIDDLE), MouseButton::MB_MIDDLE},
                                     /* {TO_STRING(MOUSE_MOVE_DOWN), MouseButton::MOUSE_MOVE_DOWN},
                                      {TO_STRING(MOUSE_MOVE_UP), MouseButton::MOUSE_MOVE_UP},
                                      {TO_STRING(MOUSE_MOVE_LEFT), MouseButton::MOUSE_MOVE_LEFT},
                                      {TO_STRING(MOUSE_MOVE_RIGHT), MouseButton::MOUSE_MOVE_RIGHT},*/
                                      {TO_STRING(MOUSE_MOVE), MouseButton::MOUSE_MOVE} };
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

    static constexpr const char* EventIdState2StringImpl(MouseButtonState state)
    {
        switch(state)
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
        static const std::map<std::string, MouseButtonState> data
                            { {TO_STRING(MB_DOWN), MouseButtonState::MB_DOWN},
                              {TO_STRING(MB_UP), MouseButtonState::MB_UP}};

        MouseButtonState ret;
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

    std::string toStringImpl() const
    {
        std::string result("[");
        result = result + EventId2StringImpl(getEventTypeCtrlId()) + ", " +
                KeyModifier2String(getEventTypeCtrlIdModifier()) + ", " +
                EventIdState2StringImpl(getEventTypeCtrlIdState()) + "] x=" +
                std::to_string(x) + ", y=" + std::to_string(y);
        return result;
    }
private:
    GLfloat x, y;
};


#endif
