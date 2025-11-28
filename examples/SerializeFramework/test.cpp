//Compile it g++ -DTEST -I../../Framework/Base/ -I../.. test.cpp
#include <sstream>
#include <ttl/SerializeFramework/Interfaces/ISerializable.h>

struct B : public ISerializableIntrusive<B>
{
    //No Impl
};

struct A : public ISerializableIntrusive<A>
{
    std::string txt;
    int num;
    std::vector<double> doubleVector;
    std::vector<B> bbb;

    //Impl
    size_t onSerialize(std::ostream &out)
    {
        size_t bytes_count = serializeParams(out, txt, num, doubleVector, bbb);

        txt.clear();
        txt.shrink_to_fit();
        doubleVector.clear();
        doubleVector.shrink_to_fit();
        return bytes_count;
    }
    size_t onDeserialize(std::istream &out)
    {
        return deserializeParams(out, txt, num, doubleVector);
    }
};

void foo()
{
    std::cout << "foo" << std::endl;
}
#include <future>
int main(int argc, char *argv[])
{
    A a, aCopy;
    if constexpr(a.isSerializable())
    {
        std::cout << "A is serializable: " << std::endl;
    }
    else
    {
        std::cout << "A is nonserializable: " << std::endl;
    }
    std::cout << "A size: " << a.getObjectSize() << std::endl;

    a.txt = "Aclassobjectstring";
    a.num = 123;
    a.doubleVector = {0.1, 0.2, 0.3};
    a.bbb.resize(5);

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
    if constexpr (b.isSerializable())
    {
        std::cout << "B is serializable: " << std::endl;
    }
    else
    {
        std::cout << "B is nonserializable: " << std::endl;
    }
    std::cout << "B size: " << b.getObjectSize() << std::endl;
    b.serialize(std::cout);
    return 0;

}
