#pragma once
#include "ECSFrame/Global/GlobalPool.h"
#include "ECSFrame/Global/GlobalQuery.h"

namespace ECS
{
    namespace Global
    {
        static GlobalPool globalpool;
        static GlobalQuery globalquery;

        /**
         * Global Pool Accessor
         * Provides static access to the global pool and query.
         */
        static EPointer<GlobalPool> Pool()
        {
            return EPointer<GlobalPool>{
                &globalpool,
                [](GlobalPool *ptr) {}};
        }

        static EPointer<EntityMap> Entities()
        {
            return Pool()->getEntities();
        }

        /**
         * Global Query Accessor
         * Provides static access to the global query.
         */
        static EPointer<GlobalQuery> Query()
        {
            return EPointer<GlobalQuery>{
                &globalquery,
                [](GlobalQuery *ptr) {}};
        }
    }

} // namespace ECS