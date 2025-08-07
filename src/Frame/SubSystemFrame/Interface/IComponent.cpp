#include "IComponent.h"

size_t IComponent::getComponentId() const
{
    // 返回组件(派生)的唯一标识符
    return typeid(*this).hash_code();
}

size_t IComponent::getComponentInstanceId() const
{
    // 返回实例的地址作为唯一标识符
    return reinterpret_cast<size_t>(this);
}
