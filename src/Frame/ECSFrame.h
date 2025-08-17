#pragma once
#include "ECSFrame/Model/System/ISystem.h"
#include "ECSFrame/Model/Component/IComponent.h"
#include "ECSFrame/Model/Entity/IEntity.h"
#include "ECSFrame/Query/IQuery.h"
#include "ECSFrame/SystemUtils.h"

#include "ECSFrame/Global/GlobalPool.h"

namespace ECS
{
    static GlobalPool globalPool;

} // namespace ECS