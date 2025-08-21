#pragma once

#include <type_traits>
#include <vector>
#include <unordered_set>

#include "IQuery.h"

#include "ECSFrame/Model/Entity/IEntity.h"
#include "ECSFrame/Pointer/EPointer.h"
#include "ECSFrame/Range/ERange.h"
#include "ECSFrame/SystemUtils.h"

template <typename... Args>
class QueryMul
    : public IQueryProxy<ECS::Query::And<Args...>>
{
    using Parent = IQueryProxy<ECS::Query::And<Args...>>;
public:
    EVector<EPointer<IEntity<Args...>>>
    mul()
    {
        auto result = Parent::Core()->getAllResults();
        EVector<EPointer<IEntity<Args...>>> query_results;
        for (auto &entity : result)
        {
            // Convert IEntityObject* to IEntity<Args...>
            auto queryEntity = ECS::QueryUtils::ConvertToQueryEntity<Args...>(entity);
            if (queryEntity)
            {
                query_results.push_back(queryEntity);
            }
        }
        return query_results;
    }
};