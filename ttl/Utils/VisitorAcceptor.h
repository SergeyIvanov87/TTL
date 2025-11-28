#ifndef VISITORACCEPTOR_H
#define VISITORACCEPTOR_H

template<class Derived>
class VisitorAcceptor
{
protected:
    VisitorAcceptor() = default;
    ~VisitorAcceptor() = default;

public:
    typedef Derived Implementation;

    //Calls Implementation from Derived class
    template<class Visitor, class ...VisitorData>
    void accept(Visitor *visitor, VisitorData ...args) const
    {
          (static_cast<const Implementation *>(this))->acceptImpl(visitor, args...);
    }

    template<class Visitor, class ...VisitorData>
    void accept(Visitor *visitor, VisitorData ...args)
    {
           (static_cast<Implementation *>(this))->acceptImpl(visitor, args...);
    }
};
#endif /* VISITORACCEPTOR_H */

