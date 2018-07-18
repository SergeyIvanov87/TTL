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
        serializeParams(out, txt, num, doubleVector);
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

int main(int argc, char *argv[])
{
    A a;
    a.txt = "Aclassobjectstring";
    a.num = 123;
    a.doubleVector = {0.1, 0.2, 0.3};

    std::cout << "a: " << a.isSerializable() << std::endl;
    std::stringstream ss;
    a.serialize(ss);
    std::cout << "A serialized: " << ss.str() << std::endl;

    A aa;
    aa.deserialize(ss);
    assert(a.txt == aa.txt && "a.txt == aa.txt");
    assert(a.num == aa.num && "a.num == aa.num");
    assert(a.doubleVector == aa.doubleVector && "a.doubleVector == aa.doubleVector");

    B b;
    std::cout << "b: " << b.isSerializable() << std::endl;
    b.serialize(std::cout);
    return 0;

}
