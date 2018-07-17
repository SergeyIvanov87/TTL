#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H
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
#include "Framework/EventFramework/Interfaces/IBaseEvent.h"
    enum class KeyboardSpecialButton
    {
        F1 = GLUT_KEY_F1,
        F2 = GLUT_KEY_F2,
        F3 = GLUT_KEY_F3,
        F4 = GLUT_KEY_F4,
        F5 = GLUT_KEY_F5,
        F6 = GLUT_KEY_F6,
        F7 = GLUT_KEY_F7,
        F8 = GLUT_KEY_F8,
        F9 = GLUT_KEY_F9,
        F10 = GLUT_KEY_F10,
        F11 = GLUT_KEY_F11,
        F12 = GLUT_KEY_F12,
        LEFT_ARROW = GLUT_KEY_LEFT,
        UP_ARROW = GLUT_KEY_UP,
        RIGHT_ARROW = GLUT_KEY_RIGHT,
        DOWN_ARROW = GLUT_KEY_DOWN,
        PAGE_UP = GLUT_KEY_PAGE_UP,
        PAGE_DOWN = GLUT_KEY_PAGE_DOWN,
        HOME_KEY = GLUT_KEY_HOME,
        END_KEY = GLUT_KEY_END,
        INSERT_KEY = GLUT_KEY_INSERT,

        MAX_KEY_VALUE
    };

    enum class KeyState
    {
        KEY_STATE_DOWN,
        KEY_STATE_UP,

        MAX_KEY_STATE
    };

    struct KeyboardKey
    {
        friend bool operator< (const KeyboardKey &lhs, const KeyboardKey &rhs);
        typedef unsigned int KeyboardCommonKey;
        typedef union KeyboardKeyHolder
        {
            KeyboardCommonKey key;
            KeyboardSpecialButton specialKey;
            friend bool operator< (const KeyboardKeyHolder &lhs, const KeyboardKeyHolder &rhs);

            KeyboardKeyHolder() = default;
            KeyboardKeyHolder( unsigned char key_, bool isSpecial)
            {
                if(isSpecial)
                {
                    specialKey = (KeyboardSpecialButton)key_;
                }
                else
                {
                    key = key_;
                }
            }
        } KeyboardKeyHolder;

        bool isSpecialKey;
        KeyboardKeyHolder keyValue;

        KeyboardKey() = default;
        KeyboardKey(unsigned char key, bool isSpecial):
            isSpecialKey(isSpecial),
            keyValue(key, isSpecial)
        {}
    };

    inline bool operator< (const KeyboardKey &lhs, const KeyboardKey &rhs)
    {
        return lhs.keyValue < rhs.keyValue;
    }
    inline bool operator< (const KeyboardKey::KeyboardKeyHolder &lhs, const KeyboardKey::KeyboardKeyHolder &rhs)
    {
        return lhs.key < rhs.key;
    }

class KeyboardEvent : public IBaseEvent<KeyboardEvent,
                                        KeyboardKey, KeyModifier, KeyState>
{
public:
    using BaseType = IBaseEvent<KeyboardEvent,
                                KeyboardKey, KeyModifier, KeyState>;

    //common ctor
    KeyboardEvent(GLfloat _x, GLfloat _y,
                  unsigned char key,
                  bool isSpecial, int keyMod, KeyState state) :
        BaseType(KeyboardKey(key, isSpecial),
        static_cast<KeyModifier>(keyMod),
        state),
        x(_x),
        y(_y)
    {
    }

    static constexpr ControlEventID getControlEventID()
    {
        return ControlEventID::KEYBOARD_EVENT;
    }

    static constexpr const char * getEventTypeDescriptionImpl()
    {
        return getObserverEventTypeString(getControlEventID());
    }


    GLfloat getPointerX() const {return x;}
    GLfloat getPointerY() const {return y;}


    //Interface Impls
    static constexpr KeyState
        getEventIdStateDefaultImpl()
            { return KeyState::KEY_STATE_DOWN; };

    static constexpr const char *
        EventId2StringImpl(KeyboardKey eventId);

    static KeyboardKey
        String2EventIdImpl(const std::string &eventIdStr, const char sep = '+');

    static constexpr const char*
        EventIdState2StringImpl(KeyState state);

    static KeyState
        String2EventIdStateImpl(const std::string &state);

    std::string
        toStringImpl() const;

private:
    GLfloat x, y;
};

#endif //KEYBOARD_EVENTS_H
