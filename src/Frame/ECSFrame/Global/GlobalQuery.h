#pragma once

#include <type_traits>

#include "ECSFrame/Pointer/EPointer.h"
#include "ECSFrame/Range/ERange.h"

class IQuery;

/**
 * GlobalQuery
 * 全局查询
 * - 管理查询缓存
 */
class GlobalQuery
{
    using QueryKey = size_t; // 查询的唯一标识符
    using QueryCache = EMap<size_t, EPointer<IQuery>>;
public:
    GlobalQuery() = default;
    ~GlobalQuery() = default;

protected:
    QueryCache queryCache; // 查询缓存
};