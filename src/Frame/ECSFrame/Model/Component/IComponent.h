#pragma once

#include <type_traits>
#include <typeinfo>

#include "ECSFrame/Forward.h"
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
        // return typeid(IComponent<T>).hash_code();
        return reinterpret_cast<size_t>(&m_type_holder);
    }

public:
    EPointer<T> Value()
    {
        return EPointer<T>(&m_value, [](void *ptr) {});
    }

protected:
    static inline ui8 m_type_holder = 0;
    T m_value;
};
