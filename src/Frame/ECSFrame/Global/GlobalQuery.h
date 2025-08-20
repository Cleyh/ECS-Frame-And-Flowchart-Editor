#pragma once

#include <type_traits>

#include "ECSFrame/Pointer/EPointer.h"
#include "ECSFrame/Range/ERange.h"

class IEntityObject;
class IQuery;

enum class NotifyType
{
    ENTITY_ADD,       // 实体添加
    ENTITY_REMOVE,    // 实体移除
    COMPONENT_ATTACH, // 组件添加
    COMPONENT_DETACH, // 组件移除
};

struct QueryNotifyMsg
{
    NotifyType type;
    /* 实体key。未确定是否保留 */
    size_t entityId;
    /* 涉及实体的指针，可能为空。未确定是否保留 */
    EPointer<IEntityObject> entity;
    EVector<size_t> componentIds;
};

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

public:
    void notify();

protected:
    QueryCache queryCache; // 查询缓存
};