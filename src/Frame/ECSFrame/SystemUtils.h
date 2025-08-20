#pragma once

#include "ECSFrame/Global/GlobalPool.h"
#include "ECSFrame/Global/GlobalQuery.h"

class GlobalManager
{
public:
    static EPointer<GlobalPool> getPool()
    {
        static GlobalPool pool;
        return EPointer<GlobalPool>(&pool, [](GlobalPool *) {});
    }

    static EPointer<GlobalQuery> getQuery()
    {
        static GlobalQuery query;
        return EPointer<GlobalQuery>(&query, [](GlobalQuery *) {});
    }
};

namespace ECS
{
    namespace Global
    {
        /**
         * Global Pool
         */

        /*
         * Global Pool Accessor
         * Provides static access to the global pool and query.
         */
        static EPointer<GlobalPool>
        Pool()
        {
            return GlobalManager::getPool();
        }

        static EPointer<EntityMap> Entities()
        {
            return GlobalManager::getPool()->getEntities();
        }

        /*
         * Global Query Accessor
         * Provides static access to the global query.
         */
        static EPointer<GlobalQuery> Query()
        {
            return GlobalManager::getQuery();
        }
    }

    namespace Notify
    {
    }

} // namespace ECS