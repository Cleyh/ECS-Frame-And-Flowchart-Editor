#pragma once

#include <type_traits>
#include <vector>
#include <unordered_set>
#include <qdebug.h>

#include "IComponent.h"
#include "IEntity.h"

#include "ECSFrame/Range/RangeSet.h"
#include "ECSFrame/SystemUtils.h"

/* Query */
using QueryResult = IEntityObject *;
using QueryResultList = EVector<IEntityObject *>;

namespace ECS
{
    namespace QueryUtils
    {
        template <typename... Args>
        static IEntity<Args...> ConvertToQueryEntity(IEntityObject *entity)
        {
            // Construct result.
            IEntity<Args...> queryEntity;
            IdSet ids = IEntity<Args...>::ComponentIds(); // the component IDs we are looking for

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
    }
}

class IQuery
{
public:
    virtual void buildQuery() = 0;

public:
    QueryResultList getAllResults() const { return results; }

    QueryResult operator[](size_t index) const
    {
        if (index < results.size())
        {
            return results[index];
        }
        return nullptr; // 或者抛出异常
    }

private:
    QueryResultList results; // 存储查询结果
};

/// 单一结果查询
template <typename... Args>
class IQuerySingle
    : public IQuery
{
public:
    IEntity<Args...> single() const
    {
        IdSet ids = IEntity<Args...>::ComponentIds();
        EntityList *entities = ECS::Utils::GlobalSystem()->getEntities();
        for (IEntityObject *entity : *entities)
        {
            IdSet entity_ids = entity->getComponentIds();
            if (entity_ids.contains(ids))
            {
                return ECS::QueryUtils::ConvertToQueryEntity<Args...>(entity);
            }
        }
        return IEntity<Args...>();
    }
    void buildQuery() override
    {
        auto ids = IEntity<Args...>::ComponentIds();
    }
};

template <typename... Args>
class IQueryMul
    : public IQuery
{
    void buildQuery() override
    {
    }
};