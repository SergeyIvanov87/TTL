#ifndef MULTIPLE_INSTANCE_TLS_H
#define MULTIPLE_INSTANCE_TLS_H
#include <cassert>
#include <mutex>
#include <shared_mutex>
#include <unordered_map>
#include <map>
#include <set>
#include <memory>
#include <algorithm>

template <typename T>
struct FriendMaker
{
    typedef T Type;
};

//Wrapper on std::map to intercept d-tor calling (when thread destroyed)
template <class Key, class T>
class ContainerWrapper : public std::map<Key, T>
{
public:
    ContainerWrapper() = default;
    ~ContainerWrapper()
    {
        //Specific thread destroying:
        //Notify all registered Key instances about TLS storage destruction
        //Happens when some thread finished its execution: all thread_local object would be destroyed
        for(auto &val : *this)
        {
            //Check that Key instance still alive, before call its method
            if(!isKeyDeregistered(val.first))
            {
                //parent is alive - notify about thread destruction
                val.first->onDestroyThread(this, val.second);
            } //observer is dead already - nothing to do
        }
        this->clear();
    }

    //push Key into dead Key pool, TODO hazard pointers?
    void deregisterKey(Key keyValue)
    {
        {
            std::unique_lock<std::shared_mutex> uniqGuard(m_deregisterKeyMutex);
            auto it = m_deregsteredKeySet.insert(keyValue);
            (void)it;
            assert(it.second);
        }
    }
    //Find key in dead pool
    bool isKeyDeregistered(Key keyValue)
    {
        {
            std::shared_lock<std::shared_mutex> sharedGuard(m_deregisterKeyMutex);
            return m_deregsteredKeySet.find(keyValue) != m_deregsteredKeySet.end();
        }
    }
    void removeDeregisteredKeyIfExist(Key keyValue)
    {
        {
            std::unique_lock<std::shared_mutex> uniqGuard(m_deregisterKeyMutex); //TODO add upgradable here?
            auto it = m_deregsteredKeySet.find(keyValue);
            if(it != m_deregsteredKeySet.end())
            {
                m_deregsteredKeySet.erase(it);
            }
        }
    }
private:
    std::shared_mutex m_deregisterKeyMutex;
    std::set<Key> m_deregsteredKeySet;
};

//TLS Adapter for Multiple instances implementation
template <class Observer, class Value>
class MultiInstanceTLSAdapter
{
public:
    friend class FriendMaker<Observer>::Type;

    typedef Observer * Key;
    typedef Value ValueType;
    typedef Value * ValueTypePtr;

    MultiInstanceTLSAdapter(Observer *pParent) :
        m_parent(pParent)
    {
        assert(m_parent);
        //Add TLS counter for owner thread

        //Make sure, that TLS storage doesn't contains current instance for deregistration.
        //It can happens, if NEW parent will created directly AFTER the previous parent destruction.
        //Implementation can use the SAME memory for new object in this case
        getTLSContainer()->removeDeregisteredKeyIfExist(m_parent);

        //Remove the SAME memory of last instance, if exist
        TLSContainer *tlsContainer = getTLSContainer();
        tlsContainer->erase(m_parent);
    }

    ~MultiInstanceTLSAdapter()
    {
        /* TODO - my parent destroyed also,and should clear him from TLS

        //find registered parent instance and notify,
        // that storage destroying
        TLSContainer *tlsContainer = getTLSContainer();
        auto res = tlsContainer->find(m_parent);

        //storage may be not used in this thread, check instance existence
        if(res != tlsContainer->end())
        {
            res->first->onDestroyThread(tlsContainer, res->second);
            tlsContainer->erase(res);
        }*/
    }

    template<class ...Args>
    ValueType &value(Args &&...args)
    {
        //insert or find existing TLS counter for current thread container
        TLSContainer *tlsContainer = getTLSContainer();
        auto it = tlsContainer->find(m_parent);
        if(it == tlsContainer->end())   //value is not created for this parent in current thread
        {
            //TODO add check on acrossThreadValues for current 'tlsContainer' to prevent ABA problem
            it = tlsContainer->emplace(std::piecewise_construct,
                                       std::forward_as_tuple(m_parent),
                                       std::forward_as_tuple(std::forward<Args>(args)...)).first;

            //notify parent about new TLS instance creation
            m_parent->onCreateThread(tlsContainer, it->second);
        }

        //return current thread local value
        return it->second;
    }

private:
    Observer *m_parent;     //should be notified about thread destruction
    using TLSContainer = ContainerWrapper<Key, ValueType>;
    //Every instance of MultiInstanceTLSAdapter has the same static storage in one thread context
    //Different threads - different storage

    //G++ BUG -- https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66944
    //Workaround
    static TLSContainer *getTLSContainer()
    {
        static thread_local TLSContainer *tlsStoragePtr;
        if(!tlsStoragePtr)
        {
            static thread_local TLSContainer tlsStorage;
            tlsStoragePtr = &tlsStorage;
        }
        return tlsStoragePtr;
    }
};

//G++ BUG -- https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66944
/*template <class Observer, class Value>
thread_local typename MultiInstanceTLSAdapter<Observer, Value>::TLSContainer MultiInstanceTLSAdapter<Observer, Value>::m_tlsStorage;
*/


//CRTP
template<class Impl, class StoragedType>
class TLSObserver
{
    using TLSImplType = MultiInstanceTLSAdapter<TLSObserver<Impl, StoragedType>, StoragedType>;
    using ValueType = StoragedType;
    using ValueTypePtr = StoragedType *;
    using TLSOwnerPtr = typename TLSImplType::TLSContainer *;
    typedef std::map<TLSOwnerPtr, ValueTypePtr> TotalThreadsCountersStorage;      //aggregation counters storage across threads


public:
    struct SafeGuard
    {
        SafeGuard(std::mutex &m, TotalThreadsCountersStorage &storage) :
                mutex(m), acrossAllThreadsValuesLocked(storage)
        {
            mutex.lock();
        }
        ~SafeGuard()
        {
            mutex.unlock();
        }
        SafeGuard(SafeGuard&&src) = default;
        SafeGuard &operator=(SafeGuard &&src) = default;
        SafeGuard(const SafeGuard &src) = delete;
        SafeGuard &operator=(const SafeGuard &src) = delete;
        TotalThreadsCountersStorage &acrossAllThreadsValuesLocked;
    private:
        std::mutex &mutex;
    };

    TLSObserver() : m_storageImpl(this)
    {
    }

    ~TLSObserver()
    {
        //My instance destroyed before thread... I need to remove my observer instance from ALL TLS storages!!!

        std::lock_guard<std::mutex> guard(m_acrossAllThreadsValuesMutex);
        //find current TLS thread value in global storage

        //And deregister current observer from TLS owner storage to
        //do not call onDestroyThread() in future for already dead this instance
        auto currentTLSStoragePtr = m_storageImpl.getTLSContainer();
        for(auto &val : m_acrossAllThreadsValues )
        {
            if(val.first == currentTLSStoragePtr)
            {
                //remove from own TLS
                currentTLSStoragePtr->erase(this);
                continue;
            }
            if(val.first)
            {
                val.first->deregisterKey(this);
            }
        }

        //destroy thread independent instance if present
        for(auto &val : m_acrossAllThreadsValues )
        {
            if(!val.first)
            {
                delete val.second;
                val.second = nullptr;
            }
        }
        m_acrossAllThreadsValues.clear();
    }

    void onDestroyThread(TLSOwnerPtr tlsContainerOwnerPtr, StoragedType &args)
    {
        //Thread destroyed before me, i need to deregister associated TLS from my 'm_acrossAllThreadsValues'
        //REMOVE from across threads!!!
        {
            std::lock_guard<std::mutex> guard(m_acrossAllThreadsValuesMutex);

            //find current TLS thread counter in global storage
            auto it = m_acrossAllThreadsValues.find(tlsContainerOwnerPtr);

            //It must exist, because class instance is alive and thread is not destroyed yet
            if(it != m_acrossAllThreadsValues.end())
            {
                //remove current TLS thread counter from global map
                assert(it->second == &args);
                m_acrossAllThreadsValues.erase(it);
            }
        }

        SafeGuard lock( m_acrossAllThreadsValuesMutex, m_acrossAllThreadsValues);
        return static_cast<Impl *>(this)->onDestroyThreadImpl(args, lock);
    }


    void onCreateThread(TLSOwnerPtr tlsContainer, StoragedType &args)
    {
        {
            //New counter for this new thread is adding into global thread list
            std::lock_guard<std::mutex> guard(m_acrossAllThreadsValuesMutex);
            m_acrossAllThreadsValues.emplace(tlsContainer, &args);
        }
        SafeGuard lock( m_acrossAllThreadsValuesMutex, m_acrossAllThreadsValues);
        return static_cast<Impl *>(this)->onCreateThreadImpl(args, lock);
    }

    template<class ...Args>
    StoragedType &tls_value(Args &&...args)
    {
        return m_storageImpl.value(std::forward<Args>(args)...);
    }

    const StoragedType &tls_value() const
    {
        return m_storageImpl.value();
    }

    SafeGuard getAcrossThreadsValues()
    {
        return SafeGuard(m_acrossAllThreadsValuesMutex, m_acrossAllThreadsValues);
    }

private:
    TLSImplType m_storageImpl;
    mutable std::mutex m_acrossAllThreadsValuesMutex;
    TotalThreadsCountersStorage m_acrossAllThreadsValues;
};

#endif //MULTIPLE_INSTANCE_TLS_H
