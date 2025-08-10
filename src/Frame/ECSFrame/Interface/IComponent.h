#pragma once

#include <type_traits>
#include <typeinfo>

class IComponent
{
public:
    virtual ~IComponent() = default;

protected:
    IComponent() = default;

public: /* id getter */
    virtual size_t getTypeId() const;
    virtual size_t getInstanceId() const;
};

template <typename T>
class IComponentWrapper
    : public IComponent
{
public: /* static function */
    static size_t TypeId()
    {
        return typeid(IComponentWrapper<T>).hash_code();
    }

public:
    T *Value()
    {
        return &m_value;
    }

private:
    T m_value;
};