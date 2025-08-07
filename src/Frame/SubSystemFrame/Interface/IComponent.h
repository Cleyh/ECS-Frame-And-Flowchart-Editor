#pragma once
#include <typeinfo>

class IComponent
{
public:
    virtual ~IComponent() = default;

protected:
    IComponent() = default;

public:
    virtual size_t getComponentId() const;
    virtual size_t getComponentInstanceId() const;
};