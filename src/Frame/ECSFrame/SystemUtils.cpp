#include "SystemUtils.h"
#include "ECSFrame/Global/GlobalPool.h"
#include "ECSFrame/Global/GlobalQuery.h"
#include "ECSFrame/Pointer/EPointer.h"

namespace ECS
{
    namespace Global
    {
        EPointer<GlobalPool> Pool()
        {
            static GlobalPool pool;
            return EPointer<GlobalPool>(&pool, [](GlobalPool *) {});
        }

        EPointer<EntityMap> Entities()
        {
            return Pool()->getEntities();
        }

        EPointer<GlobalQuery> Query()
        {
            static GlobalQuery query;
            return EPointer<GlobalQuery>(&query, [](GlobalQuery *) {});
        }
    }
} // namespace ECS