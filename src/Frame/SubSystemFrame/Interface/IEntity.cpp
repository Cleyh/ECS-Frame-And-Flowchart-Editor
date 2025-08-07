#include "IEntity.h"
#include "SubSystemFrame/SystemUtils.h"

/////////////////////////////////////////////////
// IEntityObject implementation
/////////////////////////////////////////////////
void IEntityObject::attach(IComponent *component)
{
    components.insert(typeid(*component).hash_code(), component);
}

ComponentIdSet IEntityObject::getComponentIds() const
{
    auto keys = components.keys();
    return ComponentIdSet(keys.begin(), keys.end());
}

IComponent *IEntityObject::getComponent(size_t componentId)
{
    return components.value(componentId, nullptr);
}

void IEntityObject::setComponent(size_t componentId, IComponent *component)
{
    if (!components.contains(componentId))
    {
        attach(component);
        return;
    }

    auto old_component = components.value(componentId);
    if (old_component)
    {
        delete old_component; // 删除旧组件
        old_component = nullptr; // 清空指针
    }
    components[componentId] = component; // 更新组件映射
}

IEntityObject::IEntityObject()
{
    ECS::Utils::GlobalSystem()->addEntity(this);
}
