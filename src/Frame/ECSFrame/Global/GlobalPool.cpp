#include "GlobalPool.h"

bool GlobalPool::removeEntity(size_t entityId)
{
    auto entity = m_entities->get(entityId, nullptr);
    if (!entity)
        return false;

    auto component_ids = entity->getComponentIds();
    for (auto id : component_ids)
    {
        m_components->erase(id);
    }

    return m_entities->erase(entityId) > 0;
}

bool GlobalPool::removeComponent(size_t componentId)
{
    auto component = m_components->get(componentId, nullptr);

    // In this case, we don't remove the component if it is still in use
    if (!component || component.use_count() > 1)
        return false;

    return m_components->erase(componentId) > 0;
}

bool GlobalPool::removeComponentType(size_t componentTypeId)
{
    // TODO
    return false;
}
