#include <utility>
#include <algorithm>
#include "KeyboardEvent.h"
#include "Framework/Utils/Utils.h"
#include "Framework/EventFramework/Interfaces/IBaseEvent.hpp"

    //helpers conversion functions
inline KeyboardSpecialButton String2KeyboardSpecialButton(const std::string &string)
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

inline constexpr const char *KeyboardSpecialButton2String(KeyboardSpecialButton sButton)
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

constexpr const char *CommonKeyboardKey2String(KeyboardKey::KeyboardCommonKey key)
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

KeyboardKey::KeyboardCommonKey String2CommonKeyboardKey(const std::string &keyStr)
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


constexpr const char *KeyboardEvent::EventId2StringImpl(KeyboardKey eventId)
{
    if(eventId.isSpecialKey)
    {
        return KeyboardSpecialButton2String(eventId.keyValue.specialKey);
    }
    return CommonKeyboardKey2String(eventId.keyValue.key);
}

KeyboardKey KeyboardEvent::String2EventIdImpl(const std::string &eventIdStr, const char sep
/* = '+'*/)
{
    KeyboardKey ret;
    const char separator = sep;
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
constexpr const char* KeyboardEvent::EventIdState2StringImpl(KeyState state)
{
    switch(state)
    {
        case KeyState::KEY_STATE_DOWN:
            return TO_STRING(KEY_STATE_DOWN);
        case KeyState::KEY_STATE_UP:
            return TO_STRING(KEY_STATE_UP);
        default:
            assert(false);
            break;
    }
    return "";
}
KeyState KeyboardEvent::String2EventIdStateImpl(const std::string &state)
{
    static const std::map<std::string, KeyState> data
                            { {TO_STRING(KEY_STATE_DOWN), KeyState::KEY_STATE_DOWN},
                              {TO_STRING(KEY_STATE_UP), KeyState::KEY_STATE_UP}};
    KeyState ret;
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

KeyModifier KeyboardEvent::String2KeyModifierImpl(const std::string &keyMod)
{
    return String2KeyboardModifier(keyMod);
}

KeyboardEventCMD KeyboardEvent::String2ControlEventCommandsImpl(const std::string &commandStr)
{
    static const std::map<std::string, KeyboardEventCMD> data
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
        ret = std::get<1>(*it);
    }
    else
    {
        assert(false);
    }
    return ret;
}

constexpr const char *KeyboardEvent::ControlEventCommands2StringImpl(KeyboardEventCMD command)
{
    using namespace Utils;
        switch(command)
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

std::string KeyboardEvent::toStringImpl() const
{
    std::string result("[");
    result = result + EventId2StringImpl(getEventTypeCtrlId()) + ", " +
                KeyboardModifier2String(getEventTypeCtrlIdModifier()) + ", " +
                EventIdState2StringImpl(getEventTypeCtrlIdState()) + "] x=" +
                std::to_string(x) + ", y=" + std::to_string(y);
    return result;
}
    /*
KeyboardEvent::KeyboardEvent(int button, bool isSpecial, int modifiers, KeyState state, GLfloat _x, GLfloat _y)
{
    isSpecialKey = isSpecial;
    if(isSpecial)
    {
        mKey.specialKey = (KeyboardSpecialButton)button;
    }
    else
    {
        mKey.key = button;
    }

    keyModifier = (KeyModifier)modifiers;
    mKeyState = state;

    x = _x;
    y = _y;
}*/
