#pragma once
#include <qset.h>
#include <qhash.h>
#include "../SystemUtils.h"
#include "IComponent.h"

using ComponentIdSet = QSet<size_t>;
using ComponentMap = QHash<size_t, IComponent *>;

/// @brief Entity对象
class IEntityObject
{
public:
    virtual void attach(IComponent *component)
    {
        // 将组件附加到实体
        components.insert(typeid(*component).hash_code(), component);
    }

    virtual ComponentIdSet getComponentIds() const
    {
        return components.keys().toSet();
    }

    virtual IComponent *getComponent(size_t componentId)
    {
        return components.value(componentId);
    }

    virtual ~IEntityObject() = default;

public:
    virtual IComponent *operator[](size_t componentId)
    {
        return getComponent(componentId);
    }

protected:
    ComponentMap components; // 存储组件的映射
};

/// @brief
/// Entity中间对象
/// @tparam ...Args
/// 固定组件类型
template <typename... Args>
class IEntity : public IEntityObject
{
    static_assert((std::is_base_of_v<IComponent, Args> && ...), "All Args must be derived from IComponent");

public:
    // 返回快速查找组件的标识符
    static ComponentIdSet getStaticComponentIds()
    {
        ComponentIdSet ids;
        (ids.insert(typeid(Args).hash_code()), ...); // 使用 typeid 获取每个组件的唯一标识符
        return ids;
    }

    ComponentIdSet getComponentIds() const override
    {
        return IEntityObject::getComponentIds();
    }

protected:
    IEntity()
    {
        // 自动注册entity到系统中
        GetSystem()->registerEntity(this);
        // 自动附加组件到实体
        (Attach(new Args()), ...); // 使用 fold expression 来附加所有组件
    }

public:
    virtual ~IEntity() = default;
};