#pragma once

#include <type_traits>
#include <typeinfo>

#include "ECSFrame/Pointer/EPointer.h"

class IComponentObject
{
public:
    virtual ~IComponentObject() = default;

protected:
    IComponentObject() = default;

public: /* id getter */
    virtual size_t getTypeId() const;
    virtual size_t getInstanceId() const;
};

template <typename T>
class IComponent
    : public IComponentObject
{
public: /* static function */
    static size_t TypeId()
    {
        return typeid(IComponent<T>).hash_code();
    }

public:
    T *Value()
    {
        return &m_value;
    }

private:
    T m_value;
};

template <typename T>
class IComponentWrapper
    : public EPointer<IComponent<T>>
{
public:
};

class IComponentHandler
{
    EPointer<IComponentObject> m_component;
};