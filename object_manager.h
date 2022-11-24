#pragma once

#include "intrusive_list.h"
#include "singleton.h"

template<typename T>
class ObjectVisitor : public T, public IntrusiveNode<ObjectVisitor<T>>
{
public:
    ObjectVisitor();
    virtual ~ObjectVisitor();
};

template<typename T>
class ObjectManager : public Singleton<ObjectManager<T>, true>
{
public:
    template<typename F>
    void visit(F f)
    {
        auto object  = _objects.begin();
        while (object != _objects.end())
        {
            f(*object);
            object = object->next;
        }
    }

    void add(ObjectVisitor<T>* object)
    {
        _objects.push_back(object);
    }
private:
    IntrusiveList<ObjectVisitor<T>> _objects;
};

template<typename T>
ObjectVisitor<T>::ObjectVisitor()
{
    ObjectManager<T>::get_instance().add(this);
}

template<typename T>
ObjectVisitor<T>::~ObjectVisitor()
{
    this->remove();
}
