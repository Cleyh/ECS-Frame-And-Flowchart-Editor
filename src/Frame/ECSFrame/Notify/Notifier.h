#pragma once

#include "ECSFrame/Forward.h"

#include "ECSFrame/Range/ERange.h"
#include "ECSFrame/SystemUtils.h"

enum class EntityEventType
{
    ENTITY_ADD,       // 实体添加
    ENTITY_REMOVE,    // 实体移除
    COMPONENT_ATTACH, // 组件添加
    COMPONENT_DETACH, // 组件移除
};

class MsgObject
{
public:
    virtual ~MsgObject() = default;
};

struct EntityEventMsg
    : public MsgObject
{
    EntityEventType type = EntityEventType::ENTITY_ADD;
    size_t entityId = 0;
    EVector<size_t> componentIds;
    
    EntityEventMsg(EntityEventType t = EntityEventType::ENTITY_ADD, size_t id = 0, EVector<size_t> ids = {})
        : type(t), entityId(id), componentIds(std::move(ids)) {}
};

class IGlobalNotifyObject
{
public:
    virtual void notify(EPointer<MsgObject> msg) = 0;
};

/*
 * @brief 通知对象接口
 * @tparam T 被通知对象的类型
 */
template <typename T>
class IGlobalNotify
{
public:
    virtual void SendNotify(EPointer<MsgObject> msg);
};

class Notifier
{
};
