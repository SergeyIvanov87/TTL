#ifndef LOG_TRACER
#define LOG_TRACER
#include <sstream>
#include <utility>
#include <ttl/include/Utils/StringUtils.h>

template<class Stream = std::stringstream>
class Streamed
{
public:
    Streamed(char indent = char(' ')) :
     m_ss(),
     m_ident(indent)
    {
    }

    std::string str() const
    {
        return m_ss.str();
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
    Stream &operator<< (const T &val)
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
    Stream m_ss;
    std::string m_prefix;
    char m_ident;
};


class EmptyTracerImpl
{
public:
    EmptyTracerImpl() = default;

    const std::string &str() const
    {
        static const std::string empty;
        return empty;
    }

    void increaseDeep() {}

    void decreaseDeep() {}

    const std::string &getPrefix() const
    {
        static const std::string empty;
        return empty;
    }


    template <class T>
    EmptyTracerImpl &operator<< (const T &val)
    {
        return *this;
    }
    template <class ...Args>
    void trace(Args &&...args) {}
};


template<class TracerImpl>
class Tracer
{
public:
    Tracer(TracerImpl &tr) : m_tracer(tr)
    {
        //add space before printing
        m_tracer.increaseDeep();
    }
    ~Tracer()
    {
        //remove space
        m_tracer.decreaseDeep();
    }

    Tracer(Tracer &wr) : m_tracer(wr.m_tracer)
    {
        //add space
        m_tracer.increaseDeep();
    }

    template <class T>
    Tracer &operator<< (const T &val)
    {
        //print indentations before
        m_tracer << m_tracer.getPrefix();
        //print data
        m_tracer << val << std::endl;
        return *this;
    }

    /*template< class CharT, class Traits >
    std::basic_ostream<CharT, Traits>&
*/
    TracerImpl &getManagedTracer()
    {
        return m_tracer;
    }

    template <class ...Args>
    void trace(Args &&...args)
    {
        this->operator<<(makeString(std::forward<Args>(args)...));
    }
private:
    TracerImpl &m_tracer;
};


template<>
class Tracer<EmptyTracerImpl>
{
public:
    template <class T>
    Tracer &operator<< (const T &val) noexcept
    {
        return *this;
    }

    EmptyTracerImpl &getManagedTracer() noexcept
    {
        static EmptyTracerImpl empty;
        return empty;
    }

    template <class ...Args>
    void trace(Args &&...args) noexcept {}
};

/*
template<class TracerImpl, class T>
Tracer<TracerImpl>& operator<<(Tracer<TracerImpl> &tracer, const T& val)
{
    tracer << val;
    return tracer;
}*/
#endif
