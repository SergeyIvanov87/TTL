#ifndef EVENTID_MODIFIER_SPECIFIC_H
#define EVENTID_MODIFIER_SPECIFIC_H
#include <map>
#include <cassert>

#define TO_STRING(x) #x

enum NonEventModifier
{
};

struct KeyModifier: public IEventField<KeyModifier>
{
    friend class IEventField<KeyModifier>;
    static constexpr const char *invalidKeyModifierStr = "INVALID_KEY_MODIFIER";
    enum ids
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

    ids m_id;

    //construct from raw, will use methid validate() later
    constexpr KeyModifier(size_t id = KeyModifier::NONE_MOD_KEY) : m_id(KeyModifier::ids(id)) {}

private:
    ids valueImpl() const noexcept
    {
        return m_id;
    }
    static KeyModifier createFromStringImpl(const std::string &keyMod)
    {
        static const std::map<std::string, KeyModifier::ids> data
                            {
                              {TO_STRING(NONE_MOD_KEY), KeyModifier::NONE_MOD_KEY},
                              {TO_STRING(SHIFT_MOD_KEY), KeyModifier::SHIFT_MOD_KEY},
                              {TO_STRING(CTRL_MOD_KEY), KeyModifier::CTRL_MOD_KEY},

                              {TO_STRING(SHIFT_CTL_MOD_KEY), KeyModifier::SHIFT_CTL_MOD_KEY},
                              {TO_STRING(ALT_MOD_KEY), KeyModifier::ALT_MOD_KEY},
                              {TO_STRING(SHIFT_ALT_MOD_KEY), KeyModifier::SHIFT_ALT_MOD_KEY},
                              {TO_STRING(SHIFT_CTRL_ALT_MOD_KEY), KeyModifier::SHIFT_CTRL_ALT_MOD_KEY}
                            };
        KeyModifier ret;
        ret.m_id = KeyModifier::NONE_MOD_KEY;
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

    constexpr const char *toCStringImpl() const noexcept
    {
        switch(m_id)
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
        return invalidKeyModifierStr;
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


#endif //EVENTID_MODIFIER_SPECIFIC_H
