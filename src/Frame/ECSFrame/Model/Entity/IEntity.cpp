#include "IEntity.h"
#include "ECSFrame/SystemUtils.h"

/////////////////////////////////////////////////
// IEntityObject implementation
/////////////////////////////////////////////////
void IEntityObject::attach(EPointer<IComponentObject> component)
{
    size_t componentId = component->getTypeId();
    if (components.contains(componentId))
    {
        return;
    }
    components.insert({componentId, component});
}

size_t IEntityObject::getTypeId() const
{
    return typeid(*this).hash_code();
}

size_t IEntityObject::getInstanceId() const
{
    return reinterpret_cast<size_t>(this);
}

IdSet IEntityObject::getComponentIds()
{
    IdSet idSet;
    for (const auto &pair : components)
    {
        idSet.insert(pair.first);
    }
    return idSet;
}

EPointer<IComponentObject> IEntityObject::getComponent(size_t componentId)
{
    return components.get(componentId, {});
}

void IEntityObject::setComponent(size_t componentId, EPointer<IComponentObject> component)
{
    if (!components.contains(componentId))
    {
        attach(component);
        return;
    }
    components[componentId] = component;
}

IEntityObject::IEntityObject()
{
}