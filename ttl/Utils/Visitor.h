#ifndef VISITOR_H
#define VISITOR_H


template <class Derived>
class Visitor
{
public:
    typedef Derived Implementation;
    Visitor() = default;
    ~Visitor() = default;

    //Calls Implementation from Derived class
    template<class T, class ...VisitorData>
    void visit(const T *visitedObject, VisitorData &&...args)
    {
        (static_cast<Implementation *>(this))->visitImpl(visitedObject, std::forward<VisitorData>(args)...);
    }

    template<class T, class ...VisitorData>
    void visit(T *visitedObject, VisitorData &&...args)
    {
        (static_cast<Implementation *>(this))->visitImpl(visitedObject, std::forward<VisitorData>(args)...);
    }
};
#endif /* VISITOR_H */
