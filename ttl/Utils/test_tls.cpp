#ifdef TEST

#include <iostream>
#include "ArraysOperations.h"
#include "MultipleInstanceTLS.h"
#include <thread>
#include <vector>
#include <sstream>

  struct Value
    {
      Value(std::string &&s) : value(std::move(s))
      {
      }
      Value(const std::string &s) : value(s)
      {
      }
        std::string value;
    };

class Observer : public TLSObserver<Observer, Value>
{
public:
    using Lock = typename TLSObserver<Observer, Value>::SafeGuard;
    Observer() = default;
    void onDestroyThreadImpl(Value &valueToDestroy, Lock &lock)
    {
        std::cout << "this: " << this << " " << __FUNCTION__
                  << " value: " << valueToDestroy.value
                  << " acrossAllThreadsValues: " << lock.acrossAllThreadsValuesLocked.size()
                  << std::endl;
    }

    void onCreateThreadImpl(Value &valueToDestroy, Lock &lock)
    {
        std::cout << "this: " << this << " " << __FUNCTION__
                  << " value: " << valueToDestroy.value
                  << " acrossAllThreadsValues: " << lock.acrossAllThreadsValuesLocked.size()
                  << std::endl;
    }

    void foo(std::string val)
    {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        tls_value(ss.str() + "-foo: " + val);
    }

    static Observer&& create()
    {
        Observer bbb;
        bbb.foo("create");
        return std::move(bbb);
    }
};

void worker(Observer &observer, std::string mess)
{
    for(int i =0; i < 1000000; i++)
    {
        observer.foo(mess);
    }
}
void creator()
{
    auto &&ccc = Observer::create();
    ccc.foo("creator");
}

static Observer globalStatic;
Observer global;

int main(int argc, char *argv[])
{
    std::array<float, 3> a1{0.1f, 0.2f, 0.3f};
    std::array<float, 3> a2{1.f, 2.f, 3.f};
    std::cout << "a1: " << std::to_string(a1) << std::endl;
    std::cout << "a2: " << std::to_string(a2) << std::endl;
    std::cout << "sum: " << std::to_string(a1 + a2) << std::endl;

    {
        std::cout << "simple creation test" << std::endl;
        Observer tmp;
    }

    {
        std::cout << "simple creation test with main thread value" << std::endl;
        Observer tmp;
        tmp.foo("main");
    }

    {
        std::cout << "creation test with worker thread value" << std::endl;
        Observer tmp;
        std::thread t(worker,
                     std::ref(tmp),
                     "worker");
        t.join();
    }
    
    {
        std::cout << "creation test with main thread value, and thread early destroy value" << std::endl;
        Observer tmp;
        tmp.foo("main");
        {
            std::thread t(worker,
                         std::ref(tmp),
                         "worker");
            t.join();
        }
    }
    
    {
        std::cout << "Object destroyed before thread" << std::endl;
        std::unique_ptr<std::thread> tPtr;
        {
            Observer tmp;
            tmp.foo("main");
            tPtr.reset(new std::thread(worker,
                                       std::ref(tmp),
                                       "worker"));
        }
        tPtr->join();
    }
    
    {
        std::cout << "huge test" << std::endl;
        Observer aaaa;
        aaaa.foo("main");
        aaaa.foo("main");
        aaaa.foo("main");
        aaaa.foo("main");
        size_t threadsNumber = 10;
        std::vector<std::thread> threadVector;
        for(size_t i = 0; i < threadsNumber; i++)
        {
            threadVector.push_back(std::thread(worker,
                                    std::ref(aaaa),
                                    std::string("worker_") + std::to_string(i)));
        }

        for(int i = 0; i < 100000; i++)
        {
            auto&& guard = aaaa.getAcrossThreadsValues();
            for(auto it = guard.acrossAllThreadsValuesLocked.begin(); it != guard.acrossAllThreadsValuesLocked.end(); ++it)
            {
                std::cout << it->second->value << ", ";
            }
            std::cout << std::endl;
        }

        std::thread tt(&Observer::create);
        for(size_t i = 0; i < threadsNumber; i++)
        {
            threadVector[i].join();
        }
        tt.join();
    }
    
    
    {
        std::cout << "huge test with global" << std::endl;
        global.foo("main");
        global.foo("main");
        global.foo("main");
        global.foo("main");
        size_t threadsNumber = 10;
        std::vector<std::thread> threadVector;
        for(size_t i = 0; i < threadsNumber; i++)
        {
            threadVector.push_back(std::thread(worker,
                                    std::ref(global),
                                    std::string("worker_") + std::to_string(i)));
        }

        for(int i = 0; i < 100000; i++)
        {
            auto&& guard = global.getAcrossThreadsValues();
            for(auto it = guard.acrossAllThreadsValuesLocked.begin(); it != guard.acrossAllThreadsValuesLocked.end(); ++it)
            {
                std::cout << it->second->value << ", ";
            }
            std::cout << std::endl;
        }

        std::thread tt(&Observer::create);
        for(size_t i = 0; i < threadsNumber; i++)
        {
            threadVector[i].join();
        }
        tt.join();
    }
}
#endif
