#pragma once

#include <type_traits>
#include <vector>
#include <unordered_set>

#include "ECSFrame/Model/System/ISystem.h"
#include "ECSFrame/Model/Entity/IEntity.h"

#include "ECSFrame/Pointer/EPointer.h"
#include "ECSFrame/Range/ERange.h"
#include "ECSFrame/SystemUtils.h"

/* Query */
using QueryResult = EPointer<IEntityObject>;
using QueryResultList = EVector<EPointer<IEntityObject>>;

namespace ECS
{
    namespace QueryUtils
    {
        template <typename... Args>
        static EPointer<IEntity<Args...>> ConvertToQueryEntity(EPointer<IEntityObject> entity)
        {
            // Construct result.
            auto queryEntity = EPointer<IEntity<Args...>>::make();
            IdSet ids = IEntity<Args...>::ComponentIds(); // the component IDs we are looking for

            for (auto id : ids)
            {
                // Get the component from original entity.
                auto component = entity->getComponent(id);
                if (component)
                {
                    queryEntity->setComponent(id, component);
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
    EPointer<IEntity<Args...>> single() const
    {
        IdSet ids = IEntity<Args...>::ComponentIds();
        EPointer<EntityMap> entities = ECS::Global::Entities();
        for (auto entity : *entities)
        {
            IdSet entity_ids = entity.second->getComponentIds();
            if (entity_ids.contains(ids))
            {
                return ECS::QueryUtils::ConvertToQueryEntity<Args...>(entity.second);
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
public:
    EVector<
        EPointer<IEntity<Args...>>>
    mul() const
    {
        EVector<EPointer<IEntity<Args...>>> results;
        IdSet ids = IEntity<Args...>::ComponentIds();
        EPointer<EntityMap> entities = ECS::Global::Entities();
        for (auto entity : *entities)
        {
            IdSet entity_ids = entity.second->getComponentIds();
            if (entity_ids.contains(ids))
            {
                results.push_back(ECS::QueryUtils::ConvertToQueryEntity<Args...>(entity.second));
            }
        }
        return results;
    }

    void buildQuery() override
    {
    }
};