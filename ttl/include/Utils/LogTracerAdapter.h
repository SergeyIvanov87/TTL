#ifndef LOG_TRACER_ADAPTER_H
#define LOG_TRACER_ADAPTER_H
#include <iostream>
#include <ttl/include/Utils/LogTracer.h>


class Stdout
{
public:
    Stdout(char indent = char(' ')) :
     m_ss(std::cout),
     m_ident(indent)
    {
    }

    void increaseDeep()
    {
        m_prefix.push_back(m_ident);
    }

    void decreaseDeep()
    {
        if(!m_prefix.empty())
        {
            m_prefix.pop_back();
        }
    }

    const std::string &getPrefix() const
    {
        return m_prefix;
    }

    template <class T>
    std::ostream  &operator<< (const T &val)
    {
        m_ss << val;
        return m_ss;
    }

    template <class ...Args>
    void trace(Args &&...args)
    {
        m_ss << makeString(std::forward<Args>(args)...) << "\n";
    }
private:
    std::ostream &m_ss;
    std::string m_prefix;
    char m_ident;
};

using SSTracerImpl = Streamed<std::stringstream>;
#endif //LOG_TRACER_ADAPTER_H
