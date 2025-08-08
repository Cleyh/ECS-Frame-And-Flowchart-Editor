#include "IComponent.h"

size_t IComponent::getComponentId() const
{
    return typeid(*this).hash_code();
}

size_t IComponent::getComponentInstanceId() const
{
    return reinterpret_cast<size_t>(this);
}
