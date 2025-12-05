//Compile it g++ -DTEST -I../../Framework/Base/ -I../.. test_serialize.cpp
#include <sstream>
#include <ttl/SerializeFramework/Interfaces/ISerializable.h>

struct B : public ISerializable<B>
{
    //No Impl
};

struct A : public ISerializable<A>
{
    std::string txt;
    int num;
    std::vector<double> doubleVector;
    std::vector<B> bbb;

    //Impl
    size_t onSerialize(std::ostream &out) const
    {
        return serializeParams(out, txt, num, doubleVector, bbb);
    }
    size_t onDeserialize(std::istream &out)
    {
        return deserializeParams(out, txt, num, doubleVector, bbb);
    }
};

void foo()
{
    std::cout << "foo" << std::endl;
}
#include <future>
int main(int argc, char *argv[])
{
    A a;
    if constexpr (a.isSerializable())
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

    std::stringstream ss;
    size_t bytes_count = a.serialize(ss);
    std::cout << "A serialized bytes: " << bytes_count << std::endl;


    {
        std::stringstream ss_stub;
        a.serialize(ss_stub);
        assert (ss.str() == ss_stub.str() && "subsequent serialization of A must give the same result");
    }

    A aCopy;
    aCopy.deserialize(ss);
    assert(aCopy.txt == a.txt && "aCopy.txt == a.txt");
    assert(aCopy.num == a.num && "aCopy.num == a.num");
    assert(aCopy.doubleVector == a.doubleVector && "aCopy.doubleVector == a.doubleVector");
    assert(aCopy.bbb.size() == a.bbb.size() && "aCopy.bbb == a.bbb");

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
    bytes_count = b.serialize(std::cout);
    assert(!bytes_count && "B serialization must not occupy memory");
    return 0;

}
