#include "IComponent.h"

size_t IComponentObject::getTypeId() const
{
    return typeid(*this).hash_code();
}

size_t IComponentObject::getInstanceId() const
{
    return reinterpret_cast<size_t>(this);
}
