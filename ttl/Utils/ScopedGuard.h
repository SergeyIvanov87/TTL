#ifndef CF_SCOPED_GUARD_H
#define CF_SCOPED_GUARD_H
#include <tuple>
namespace Utils
{

//see http://www.drdobbs.com/cpp/generic-change-the-way-you-write-excepti/184403758?pgno=2
class ScopeGuardBase
{
public:
    void refuse() const noexcept
    {
        m_refused = true;
    }
    ~ScopeGuardBase()
    {
    }
    ScopeGuardBase(ScopeGuardBase &&src):
        m_refused(std::move(src.m_refused))
    {
        src.m_refused = true; //no need to invoke lambda twice
    }

    ScopeGuardBase &operator=(ScopeGuardBase &&src)
    {
        m_refused = std::move(src.m_refused);
        src.m_refused = true; //no need to invoke lambda twice
        return *this;
    }
    ScopeGuardBase(const ScopeGuardBase &src) = delete;
    ScopeGuardBase &operator=(const ScopeGuardBase &src) = delete;

protected:
    mutable bool m_refused;
    ScopeGuardBase() : m_refused(false)
    {
    }


};

template<class Lambda>
class ScopedGuardImpl : public ScopeGuardBase
{
public:
    using ScopeGuardBase::m_refused;

    ScopedGuardImpl(const Lambda &func) :
        ScopeGuardBase(),
        mFunc(func)
    {
    }

    ScopedGuardImpl(ScopedGuardImpl &&src) = default;
    ScopedGuardImpl &operator=(ScopedGuardImpl &&src) = default;

    ~ScopedGuardImpl()
    {
        if(!m_refused)
        {
            mFunc();
        }
    }
private:
    Lambda mFunc;
};

//Variadic version
/****************************Helper*************************************/
//https://stackoverflow.com/questions/7858817/unpacking-a-tuple-to-call-a-matching-function-pointer
template<int ...>
struct seq
{};

template<int N, int ...S>
struct gens : gens<N-1, N-1, S...>
{};

template<int ...S>
struct gens<0, S...>
{
  typedef seq<S...> type;
};
/***********************************************************************/
template<class Fun, class ...Params>
class ScopedGuardVariadicImpl : public ScopeGuardBase
{
public:
    ScopedGuardVariadicImpl(const Fun &func, Params &&...args) :
        ScopeGuardBase(),
        mFunc(func),
        m_tupleArgs(std::forward<Params>(args)...)
    {
    }

    ScopedGuardVariadicImpl(ScopedGuardVariadicImpl &&src) = default;
    ScopedGuardVariadicImpl &operator=(ScopedGuardVariadicImpl &&src) = default;

    template<int ...S>
    void callFunc(seq<S...>)
    {
        (*mFunc)(std::get<S>(m_tupleArgs)...);
    }

    ~ScopedGuardVariadicImpl()
    {
        if(!m_refused)
        {
            callFunc(typename gens<sizeof...(Params)>::type());
        }
    }
private:
    Fun *mFunc;
    std::tuple<Params...> m_tupleArgs;
};


//User Common interface
template <typename Fun>
ScopedGuardImpl<Fun>
makeGuard(const Fun& fun)
{
    return ScopedGuardImpl<Fun>(fun);
}

template <typename Fun, class ...Params>
ScopedGuardVariadicImpl<Fun, Params...>
makeGuard(const Fun& fun, Params &&...args)
{
    return ScopedGuardVariadicImpl<Fun, Params...>(fun, std::forward<Params>(args)...);
}
}


#endif //CF_SCOPED_GUARD_H
