# TTL

Collection of usefull C++ templates utils, based on Mix-in classes concepts(https://en.wikipedia.org/wiki/Mixin), which extending user-class functionality by providing additional capabilities. The following capabilities are provided at now:

1) EventFramework
2) Serialize/Deserialize framework
3) other utils: 
      thread_local variables storage per class object, not static per class (In MAP-REDUCE typical usage)
      LocklessWriter - concurrent file writer (based on std::atomic)
      
# EventFramework
  <TODO> Will add description TODO...
  
  
# Serialize/Deseriaize
  Let's imagine, that you have several classes for which you need provide serialize/deserialize capability (store data in files and load data from files, for example). In common way you need to implement abstract class which provide serialize/deserialize interface, for example:
  
    struct ISerializable
    {
        virtual ~ISerialize() = default;
        virtual bool serialize() = 0;
        virtual bool deserialize() = 0;
     };
  
  So, this interface required to be implemented in all your classes: every class will implement their own serialize/deserialize capability. Every software developer will provide own implementation of this interface with own types and logics...
  
  Look at the typical TTL implementation usage example:
  
    struct A : public ISerializable<A>
    {
      enum
      {
          isSerializableSupport = true
      };

      //Serializable data
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

      //No Impl, default stub behavior
    };
    
    //USAGE: Somewhere in code...
    A a;
    ...
    std::stringstream ss;
    a.serialize(ss);
    ...
    A aDup;
    aDup.deserialize(ss);
It's looked: Short, pretty, extendable!

