#pragma once
#include <typeinfo>

class IComponent
{
public:
    virtual ~IComponent() = default;

protected:
    IComponent() = default;

public:
    virtual size_t getComponentId() const
    {
        // 返回组件(派生)的唯一标识符
        return typeid(*this).hash_code();
    }
    virtual size_t getComponentInstanceId() const
    {
        // 返回实例的地址作为唯一标识符
        return reinterpret_cast<size_t>(this);
    }
};