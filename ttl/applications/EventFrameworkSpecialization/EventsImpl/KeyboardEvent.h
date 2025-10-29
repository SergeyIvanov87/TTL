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
#include "KeyboardCommands.h"
#include "../EventIDsDeclaration.h"
#include "EventIdModifierSpecific.h"
#include <ttl/include/EventFramework/Interfaces/IBaseEvent.hpp>

struct KeyState : public IEventField<KeyState>
{
    friend class IEventField<KeyState>;
    static constexpr const char *invalidKeyModifierStr = "INVALID_KEY_STATE";
    enum ids
    {
        KEY_STATE_DOWN,
        KEY_STATE_UP,

        MAX_KEY_STATE
    };

    ids m_id;

    constexpr KeyState(size_t id = KeyState::MAX_KEY_STATE) : m_id(KeyState::ids(id)) {}

private:
    ids valueImpl() const noexcept
    {
        return m_id;
    }

    const char* toCStringImpl() const
    {
        switch(m_id)
        {
            case KeyState::KEY_STATE_DOWN:
                return TO_STRING(KEY_STATE_DOWN);
            case KeyState::KEY_STATE_UP:
                return TO_STRING(KEY_STATE_UP);
            default:
                assert(false);
                break;
        }
        return invalidKeyModifierStr;

    }

    static KeyState createFromStringImpl(const std::string &state)
    {
         static const std::map<std::string, KeyState::ids> data
                            { {TO_STRING(KEY_STATE_DOWN), KeyState::KEY_STATE_DOWN},
                              {TO_STRING(KEY_STATE_UP), KeyState::KEY_STATE_UP}};
        KeyState ret(KeyState::MAX_KEY_STATE);
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

    std::pair<bool, std::string> validateImpl() const
    {
        std::pair<bool, std::string> ret {true, ""};
        const char *str = toCStringImpl();
        if( str == invalidKeyModifierStr)
        {
            ret.first = false;
            ret.second = str;
        }
        return ret;
    }
};


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
struct KeyboardKey : public IEventField<KeyboardKey>
{
    friend class IEventField<KeyboardKey>;
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

private:
    KeyboardKeyHolder valueImpl() const noexcept
    {
        return keyValue;
    }

            //helpers conversion functions
    inline static KeyboardSpecialButton String2KeyboardSpecialButton(const std::string &string)
    {
        if(string == TO_STRING(F1))
        {
            return (KeyboardSpecialButton::F1);
        }
        else if(string == TO_STRING(F2))
        {
            return (KeyboardSpecialButton::F2);
        }
        else if(string == TO_STRING(F3))
        {
            return (KeyboardSpecialButton::F3);
        }
        else if(string == TO_STRING(F4))
        {
            return (KeyboardSpecialButton::F4);
        }
        else if(string == TO_STRING(F5))
        {
            return (KeyboardSpecialButton::F5);
        }
        else if(string == TO_STRING(F6))
        {
            return (KeyboardSpecialButton::F6);
        }
        else if(string == TO_STRING(F7))
        {
            return (KeyboardSpecialButton::F7);
        }
        else if(string == TO_STRING(F8))
        {
            return (KeyboardSpecialButton::F8);
        }
        else if(string == TO_STRING(F9))
        {
            return (KeyboardSpecialButton::F9);
        }
        else if(string == TO_STRING(F10))
        {
            return (KeyboardSpecialButton::F10);
        }
        else if(string == TO_STRING(F11))
        {
            return (KeyboardSpecialButton::F11);
        }
        else if(string == TO_STRING(F12))
        {
            return (KeyboardSpecialButton::F12);
        }
        else if(string == TO_STRING(LEFT_ARROW))
        {
            return (KeyboardSpecialButton::LEFT_ARROW);
        }
        else if(string == TO_STRING(UP_ARROW))
        {
            return (KeyboardSpecialButton::UP_ARROW);
        }
        else if(string == TO_STRING(RIGHT_ARROW))
        {
            return (KeyboardSpecialButton::RIGHT_ARROW);
        }
        else if(string == TO_STRING(DOWN_ARROW))
        {
            return (KeyboardSpecialButton::DOWN_ARROW);
        }
        else if(string == TO_STRING(PAGE_UP))
        {
            return (KeyboardSpecialButton::PAGE_UP);
        }
        else if(string == TO_STRING(PAGE_DOWN))
        {
            return (KeyboardSpecialButton::PAGE_DOWN);
        }
        else if(string == TO_STRING(HOME_KEY))
        {
            return (KeyboardSpecialButton::HOME_KEY);
        }
        else if(string == TO_STRING(END_KEY))
        {
            return (KeyboardSpecialButton::END_KEY);
        }
        else if(string == TO_STRING(INSERT_KEY))
        {
            return (KeyboardSpecialButton::INSERT_KEY);
        }
        else
        {
            assert(false);
        }

        return KeyboardSpecialButton::MAX_KEY_VALUE;//static_cast<int>(KeyboardSpecialButton::EMPTY);
    }

inline static constexpr const char *KeyboardSpecialButton2String(KeyboardSpecialButton sButton)
{
    switch(sButton)
    {
        case KeyboardSpecialButton::F1:
            return TO_STRING(F1);
        case KeyboardSpecialButton::F2:
            return TO_STRING(F2);
        case KeyboardSpecialButton::F3:
            return TO_STRING(F3);
        case KeyboardSpecialButton::F4:
            return TO_STRING(F4);
        case KeyboardSpecialButton::F5:
            return TO_STRING(F5);
        case KeyboardSpecialButton::F6:
            return TO_STRING(F6);
        case KeyboardSpecialButton::F7:
            return TO_STRING(F7);
        case KeyboardSpecialButton::F8:
            return TO_STRING(F8);
        case KeyboardSpecialButton::F9:
            return TO_STRING(F9);
        case KeyboardSpecialButton::F10:
            return TO_STRING(F10);
        case KeyboardSpecialButton::F11:
            return TO_STRING(F11);
        case KeyboardSpecialButton::F12:
            return TO_STRING(F12);
        case KeyboardSpecialButton::LEFT_ARROW:
            return TO_STRING(LEFT_ARROW);
        case KeyboardSpecialButton::UP_ARROW:
            return TO_STRING(UP_ARROW);
        case KeyboardSpecialButton::RIGHT_ARROW:
            return TO_STRING(RIGHT_ARROW);
        case KeyboardSpecialButton::DOWN_ARROW:
            return TO_STRING(DOWN_ARROW);
        case KeyboardSpecialButton::PAGE_UP:
            return TO_STRING(PAGE_UP);
        case KeyboardSpecialButton::PAGE_DOWN:
            return TO_STRING(PAGE_DOWN);
        case KeyboardSpecialButton::HOME_KEY:
            return TO_STRING(HOME_KEY);
        case KeyboardSpecialButton::END_KEY:
            return TO_STRING(END_KEY);
        case KeyboardSpecialButton::INSERT_KEY:
            return TO_STRING(INSERT_KEY);
        default:
            assert(false);
            break;
    }
    return "";
}

inline static constexpr const char *CommonKeyboardKey2String(KeyboardKey::KeyboardCommonKey key)
{
    constexpr std::array<const char *, std::numeric_limits<unsigned char>::max() + 1> charArray
    {"", "", "", "", "", "", "", "", "", "",
  /*10*/  "", "", "", "", "", "", "", "", "", "",
  /*20*/  "", "", "", "", "", "", "", "", "", "",
  /*30*/  "", "", "SPACE", "!", "\"", "#", "$", "%", "&", "'",
  /*40*/  "(", ")", "*", "+", ",", "-", ".", "/", "0", "1",
  /*50*/  "2", "3", "4", "5", "6", "7", "8", "9", ":", ";",
  /*60*/  "<", "=", ">", "?", "@", "A", "B", "C", "D", "E",
  /*70*/ "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
  /*80*/ "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y",
  /*90*/ "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c",
  /*100*/ "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
  /*110*/ "n", "o", "p", "q", "r", "s", "t", "u", "v", "w",
  /*120*/ "x", "y", "z", "{", "|", "}", "~", "DEL", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", ""};
    if(key > std::numeric_limits<unsigned char>::max())
    {
        return "";
    }
    return charArray[key];
}

static inline KeyboardKey::KeyboardCommonKey String2CommonKeyboardKey(const std::string &keyStr)
{
    constexpr std::array<const char *, std::numeric_limits<unsigned char>::max() + 1> charArray
    {     "", "", "", "", "", "", "", "", "", "",
  /*10*/  "", "", "", "", "", "", "", "", "", "",
  /*20*/  "", "", "", "", "", "", "", "", "", "",
  /*30*/  "", "", "SPACE", "!", "\"", "#", "$", "%", "&", "'",
  /*40*/  "(", ")", "*", "+", ",", "-", ".", "/", "0", "1",
  /*50*/  "2", "3", "4", "5", "6", "7", "8", "9", ":", ";",
  /*60*/  "<", "=", ">", "?", "@", "A", "B", "C", "D", "E",
  /*70*/  "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",
  /*80*/  "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y",
  /*90*/  "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c",
  /*100*/ "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
  /*110*/ "n", "o", "p", "q", "r", "s", "t", "u", "v", "w",
  /*120*/ "x", "y", "z", "{", "|", "}", "~", "DEL", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", "", "", "", "", "",
    "", "", "", "", "", ""};
    auto it = std::find(charArray.begin(), charArray.end(), keyStr);
    if(it == charArray.end())
    {
        return 0;
    }
    return std::distance(charArray.begin(), it);
}


    inline constexpr const char *toCStringImpl() const noexcept
    {
        if(isSpecialKey)
        {
            return KeyboardSpecialButton2String(keyValue.specialKey);
        }
        return CommonKeyboardKey2String(keyValue.key);
    }

    static inline KeyboardKey createFromStringImpl(const std::string &eventIdStr)
    {
        const char sep = '+';
        KeyboardKey ret;
        std::list<std::string> list;
        if (!Utils::tokenizeString(eventIdStr, sep, list))
        {
            return ret;
        }

        if(list.size() == 1)
        {
            ret.isSpecialKey = false;
            ret.keyValue.key = String2CommonKeyboardKey(eventIdStr);
            return ret;
        }else if(list.size() == 2)
        {
            //TODO!!!!
            ret.isSpecialKey = true;
            ret.keyValue.key = String2CommonKeyboardKey(*list.begin());
            ret.keyValue.specialKey = String2KeyboardSpecialButton(*std::next(list.begin()));
        }
        return ret;
    }
};

inline bool operator< (const KeyboardKey::KeyboardKeyHolder &lhs, const KeyboardKey::KeyboardKeyHolder &rhs)
{
    return lhs.key < rhs.key;
}

//Aggregated Event Definition
class KeyboardEvent : public IBaseEvent<KeyboardEvent,
                                        KeyboardKey, KeyModifier, KeyState, KeyboardEventCMD>
{
public:
    using BaseType = IBaseEvent<KeyboardEvent,
                                KeyboardKey, KeyModifier, KeyState, KeyboardEventCMD>;

    //common ctor
    KeyboardEvent(GLfloat _x, GLfloat _y,
                  unsigned char key,
                  bool isSpecial, int keyMod, KeyState state) :
        BaseType(KeyboardKey(key, isSpecial), keyMod, state),
                x(_x), y(_y)
    {
    }

    static constexpr ControlEventID getControlEventID()
    {
        return ControlEventID::KEYBOARD_EVENT;
    }

    static constexpr const char *getEventTypeDescriptionImpl()
    {
        return TO_STRING(KEYBOARD_EVENT);
    }

    GLfloat getPointerX() const {return x;}
    GLfloat getPointerY() const {return y;}

    //Interface Impls
    inline static KeyboardEventCMD String2ControlEventCommandsImpl(const std::string &commandStr);

    inline static constexpr const char *ControlEventCommands2StringImpl(KeyboardEventCMD command);

    inline std::string
        toStringImpl() const
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

//#include "KeyboardEvent.cpp"
#endif //KEYBOARD_EVENTS_H
