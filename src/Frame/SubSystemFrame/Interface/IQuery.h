#pragma once

#include <type_traits>
#include <qvector.h>
#include "IComponent.h"
#include "IEntity.h"

#include "../SystemUtils.h"

using QueryResult = IEntityObject *;
using QueryResults = QVector<IEntityObject *>;

template <typename... Args>
static IEntity<Args...> ConvertToQueryEntity(IEntityObject *entity)
{
    // Construct result.
    IEntity<Args...> queryEntity;
    QSet<size_t> ids = IEntity<Args...>::getStaticComponentIds(); // the component IDs we are looking for

    for (auto id : ids)
    {
        // Get the component from original entity.
        IComponent *component = entity->getComponent(id);
        if (component)
        {
            queryEntity.setComponent(id, component);
        }
        else
        {
            qWarning() << "Component with ID" << id << "not found in entity.";
        }
    }
    return queryEntity;
}

class IQuery
{
public:
    virtual void buildQuery() = 0;

public:
    QueryResults getAllResults() const { return results; }

    QueryResult operator[](size_t index) const
    {
        if (index < results.size())
        {
            return results[index];
        }
        return nullptr; // 或者抛出异常
    }

private:
    QueryResults results; // 存储查询结果
};

/// 单一结果查询
template <typename... Components>
class IQuerySingle
    : public IQuery
{
    static_assert((std::is_base_of_v<IComponent, Components> && ...), "All Components must be derived from IComponent");

public:
    IEntity<Components...> single() const
    {
        QSet<size_t> ids = IEntity<Components...>::getStaticComponentIds();
        QVector<IEntityObject*>* entities = ECS::Utils::GlobalSystem()->getEntities();
        for (IEntityObject *entity : *entities)
        {
            QSet<size_t> entity_ids = entity->getComponentIds();
            if (entity_ids.contains(ids))
            {
                return ConvertToQueryEntity<Components...>(entity);
            }
        }
        return IEntity<Components...>();
    }
    void buildQuery() override
    {
        auto ids = IEntity<Components...>::getStaticComponentIds();
    }
};

/// 多组件查询（与）
template <typename... Components>
class IQueryMultiple
    : public IQuery
{
    static_assert((std::is_base_of_v<IComponent, Components> && ...), "All Components must be derived from IComponent");

public:
    IEntity<Components...> multiple() const
    {
        return IEntity<Components...>::getStaticComponentIds();
    }
    void buildQuery() override
    {
        auto ids = IEntity<Components...>::getStaticComponentIds();
    }
};

/// 多组件查询（或）
template <typename... Components>
class IQueryAny
    : public IQuery
{
    static_assert((std::is_base_of_v<IComponent, Components> && ...), "All Components must be derived from IComponent");

public:
    IEntity<Components...> any() const
    {
        return IEntity<Components...>::getStaticComponentIds();
    }
    void buildQuery() override
    {
        auto ids = IEntity<Components...>::getStaticComponentIds();
    }
};