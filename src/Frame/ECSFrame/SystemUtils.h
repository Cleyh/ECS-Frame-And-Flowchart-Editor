#pragma once
#include "ECSFrame/Model/System/ISystem.h"

namespace ECS
{
    namespace Utils
    {
        /// 获取全局系统实例
        static IGlobalSystem *GlobalSystem()
        {
            return IGlobalSystem::getInstance();
        }
    }
}; // namespace ECS