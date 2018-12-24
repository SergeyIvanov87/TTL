#ifndef EVENTID_MODIFIER_SPECIFIC_H
#define EVENTID_MODIFIER_SPECIFIC_H
#include <map>
#include <cassert>

#define TO_STRING(x) #x

enum NonEventModifier
{
};

enum KeyModifier
{
    NONE_MOD_KEY = 0,
    SHIFT_MOD_KEY = GLUT_ACTIVE_SHIFT,
    CTRL_MOD_KEY = GLUT_ACTIVE_CTRL,
    SHIFT_CTL_MOD_KEY = SHIFT_MOD_KEY | CTRL_MOD_KEY,
    ALT_MOD_KEY = GLUT_ACTIVE_ALT,
    SHIFT_ALT_MOD_KEY = SHIFT_MOD_KEY | ALT_MOD_KEY,
    SHIFT_CTRL_ALT_MOD_KEY = SHIFT_ALT_MOD_KEY | CTRL_MOD_KEY,

    MAX_KEY_MOD
};

inline KeyModifier String2KeyboardModifier(const std::string &keyMod)
{
    static const std::map<std::string, KeyModifier> data
                            {
                              {TO_STRING(NONE_MOD_KEY), KeyModifier::NONE_MOD_KEY},
                              {TO_STRING(SHIFT_MOD_KEY), KeyModifier::SHIFT_MOD_KEY},
                              {TO_STRING(CTRL_MOD_KEY), KeyModifier::CTRL_MOD_KEY},

                              {TO_STRING(SHIFT_CTL_MOD_KEY), KeyModifier::SHIFT_CTL_MOD_KEY},
                              {TO_STRING(ALT_MOD_KEY), KeyModifier::ALT_MOD_KEY},
                              {TO_STRING(SHIFT_ALT_MOD_KEY), KeyModifier::SHIFT_ALT_MOD_KEY},
                              {TO_STRING(SHIFT_CTRL_ALT_MOD_KEY), KeyModifier::SHIFT_CTRL_ALT_MOD_KEY}
                            };
    KeyModifier ret = KeyModifier::NONE_MOD_KEY;
    auto it = data.find(keyMod);
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

inline constexpr const char *KeyboardModifier2String(KeyModifier mod)
{
    switch(mod)
    {
        case KeyModifier::NONE_MOD_KEY:
            return "";
        case KeyModifier::SHIFT_MOD_KEY:
            return TO_STRING(SHIFT_MOD_KEY);
        case KeyModifier::CTRL_MOD_KEY:
            return TO_STRING(CTRL_MOD_KEY);
        case KeyModifier::SHIFT_CTL_MOD_KEY:
            return TO_STRING(SHIFT_CTL_MOD_KEY);
        case KeyModifier::ALT_MOD_KEY:
            return TO_STRING(ALT_MOD_KEY);
        case KeyModifier::SHIFT_ALT_MOD_KEY:
            return TO_STRING(SHIFT_ALT_MOD_KEY);
        case KeyModifier::SHIFT_CTRL_ALT_MOD_KEY:
            return TO_STRING(SHIFT_CTRL_ALT_MOD_KEY);
        default:
            assert(false);
    }
    return "";
}

#endif //EVENTID_MODIFIER_SPECIFIC_H
