#include "IEntity.h"
#include "ECSFrame/SystemUtils.h"

/////////////////////////////////////////////////
// IEntityObject implementation
/////////////////////////////////////////////////
void IEntityObject::_attach(size_t componentId, EPointer<IComponentObject> pcomponent)
{
    components[componentId] = pcomponent;
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

void IEntityObject::setComponent(size_t componentId, EPointer<IComponentObject> pcomponent)
{
    if (!components.contains(componentId))
    {
        _attach(componentId, pcomponent);
        return;
    }
    components[componentId] = pcomponent;
}

IEntityObject::IEntityObject()
{
}