#include "IComponent.h"

size_t IComponent::getTypeId() const
{
    return typeid(*this).hash_code();
}

size_t IComponent::getInstanceId() const
{
    return reinterpret_cast<size_t>(this);
}
