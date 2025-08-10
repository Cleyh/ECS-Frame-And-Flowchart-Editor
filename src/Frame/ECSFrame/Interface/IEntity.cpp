#include "IEntity.h"
#include "ECSFrame/SystemUtils.h"

/////////////////////////////////////////////////
// IEntityObject implementation
/////////////////////////////////////////////////
void IEntityObject::attach(IComponent *component)
{
    size_t componentId = component->getTypeId();
    components.insert({componentId, component});
}

IdSet IEntityObject::getComponentIds()
{
    auto keys = components.keys();
    return IdSet::fromVector(keys);
}

IComponent *IEntityObject::getComponent(size_t componentId)
{
    return components.get(componentId, nullptr);
}

void IEntityObject::setComponent(size_t componentId, IComponent *component)
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
