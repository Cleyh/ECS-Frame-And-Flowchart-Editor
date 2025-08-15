#include "IEntity.h"
#include "ECSFrame/SystemUtils.h"

/////////////////////////////////////////////////
// IEntityObject implementation
/////////////////////////////////////////////////
void IEntityObject::attach(IComponentObject *component)
{
    size_t componentId = component->getTypeId();
    components.insert({componentId, component});
}

size_t IEntityObject::getTypeId() const
{
    return typeid(IEntityObject).hash_code();
}

size_t IEntityObject::getInstanceId() const
{
    return reinterpret_cast<size_t>(this);
}

IdSet IEntityObject::getComponentIds()
{
    auto keys = components.keys();
    return IdSet::fromVector(keys);
}

IComponentObject *IEntityObject::getComponent(size_t componentId)
{
    return components.get(componentId, nullptr);
}

void IEntityObject::setComponent(size_t componentId, IComponentObject *component)
{
    if (!components.contains(componentId))
    {
        attach(component);
        return;
    }
    auto old_component = components.get(componentId, nullptr);
    if (old_component)
    {
        delete old_component;    // 删除旧组件
        old_component = nullptr; // 清空指针
    }
    components[componentId] = component; // 替换为新组件
}

IEntityObject::IEntityObject()
{
}
