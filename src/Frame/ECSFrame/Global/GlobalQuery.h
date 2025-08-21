#pragma once

#include <type_traits>

#include "ECSFrame/Forward.h"
#include "ECSFrame/Notify/Notifier.h"

#include "ECSFrame/Pointer/EPointer.h"
#include "ECSFrame/Range/ERange.h"

class IEntityObject;
class IQuery;

/**
 * GlobalQuery
 * 全局查询
 * - 管理查询缓存
 */
class GlobalQuery
    : public IGlobalNotifyObject
{
    using QueryKey = size_t; // 查询的唯一标识符
    using QueryCache = EMap<size_t, EPointer<IQuery>>;

public:
    GlobalQuery() = default;
    ~GlobalQuery() = default;

public:
    virtual void notify(EPointer<MsgObject> msg) override;
    virtual void addQuery(EPointer<IQuery> query);
    virtual EPointer<IQuery> getQuery(size_t typeId);

protected:
    QueryCache m_cache; // 查询缓存
};