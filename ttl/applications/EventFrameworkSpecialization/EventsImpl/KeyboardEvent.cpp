#if 0
#include <utility>
#include <algorithm>
#include "KeyboardEvent.h"
#include <ttl/include/Utils/Utils.h>
#include <ttl/include/EventFramework/Interfaces/IBaseEvent.hpp>




inline std::string KeyboardEvent::toStringImpl() const
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
#endif
