#ifndef TEST_CUSTOM_EVENT_COMMANDS_H
#define TEST_CUSTOM_EVENT_COMMANDS_H

struct CustomEventCMD : public IEventField<CustomEventCMD>
{
    friend class IEventField<CustomEventCMD>;
    enum ids
    {
        TEID_1_CMD,
        MAX__EVENT_TYPE
    };
    ids m_id;

private:
    static CustomEventCMD createFromStringImpl(const std::string &commandStr)
    {
        static const std::map<std::string, CustomEventCMD::ids> data
                            {
                                {TO_STRING(TEID_1_CMD), CustomEventCMD::TEID_1_CMD},
                            };

        CustomEventCMD ret;
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

    constexpr const char *toCStringImpl() const
    {
        using namespace Utils;
        switch(m_id)
        {
            case CustomEventCMD::TEID_1_CMD:
                return TO_STRING(TEID_1_CMD);
            default:
                assert(false);
        }
        return TO_STRING(TEID_1_CMD);
    }

    ids valueImpl() const noexcept
    {
        return m_id;
    }
};
#endif //TEST_CUSTOM_EVENT_COMMANDS_H
