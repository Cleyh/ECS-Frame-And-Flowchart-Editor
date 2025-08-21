#pragma once

#include "ECSFrame/Forward.h"

namespace ECS
{
    namespace Global
    {
        // 前向声明的静态访问函数
        EPointer<GlobalPool> Pool();
        EPointer<EntityMap> Entities();
        EPointer<GlobalQuery> Query();
    }
} // namespace ECS