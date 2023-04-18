//Compile it g++ -DTEST -I../../Framework/Base/ -I../.. test.cpp
#include <sstream>
#include "Framework/SerializeFramework/Interfaces/ISerializable.h"

struct A : public ISerializable<A>
{
    enum
    {
        isSerializableSupport = true
    };

    std::string txt;
    int num;
    std::vector<double> doubleVector;

    //Impl
    bool serializeImpl(std::ostream &out)
    {
        size_t bytes_count = serializeParams(out, txt, num, doubleVector);
        if (bytes_count != (sizeof(num) + doubleVector.size() * sizeof(double) + txt.size()))
        {
            abort();
        }
        txt.clear();
        txt.shrink_to_fit();
        doubleVector.clear();
        doubleVector.shrink_to_fit();
        return true;
    }
    bool deserializeImpl(std::istream &out)
    {
        deserializeParams(out, txt, num, doubleVector);
        return true;
    }
};

struct B : public ISerializable<B>
{
    enum
    {
        isSerializableSupport = false
    };

    //No Impl
};

void foo()
{
    std::cout << "foo" << std::endl;
}
#include <future>
int main(int argc, char *argv[])
{
    A a, aCopy;
    a.txt = "Aclassobjectstring";
    a.num = 123;
    a.doubleVector = {0.1, 0.2, 0.3};

    aCopy = a;
    std::cout << "a: " << a.isSerializable() << std::endl;
    std::stringstream ss;
    a.serialize(ss);
    std::cout << "A serialized: " << ss.str() << std::endl;


    {
        auto r = std::async(&foo);
        std::stringstream ss_stub;
        a.serialize(ss_stub);
        std::cout << "A serialized again" << ss_stub.str() << std::endl;
        r.wait();
    }

    A aa;
    aa.deserialize(ss);
    assert(aCopy.txt == aa.txt && "aCopy.txt == aa.txt");
    assert(aCopy.num == aa.num && "aCopy.num == aa.num");
    assert(aCopy.doubleVector == aa.doubleVector && "aCopy.doubleVector == aa.doubleVector");

    B b;
    std::cout << "b: " << b.isSerializable() << std::endl;
    b.serialize(std::cout);
    return 0;

}
