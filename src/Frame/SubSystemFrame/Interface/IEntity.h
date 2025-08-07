#pragma once
#include <type_traits>
#include <qset.h>
#include <qhash.h>
#include "../SystemUtils.h"
#include "IComponent.h"

using ComponentIdSet = QSet<size_t>;
using ComponentMap = QHash<size_t, IComponent *>;

/// Entity基础对象
/// 组件的容器，组件不可重复
class IEntityObject
{
protected:
    /// 附加组件到实体(内部方法)
    virtual void attach(IComponent *component);

public:
    /// 附加组件到实体
    template <typename T>
    void attach()
    {
        static_assert(std::is_base_of_v<IComponent, T>, "T must be derived from IComponent");
        attach(new T());
    }
    /// 获取组件的唯一标识符集合
    virtual ComponentIdSet getComponentIds() const;

    /// 获取组件实例
    virtual IComponent *getComponent(size_t componentId);

    /// 设置组件实例
    virtual void setComponent(size_t componentId, IComponent *component);

    IEntityObject();

    virtual ~IEntityObject() = default;

protected:
    ComponentMap components; // 存储组件的映射
};

/// Entity模板类
template <typename... Args>
class IEntity : public IEntityObject
{
    static_assert((std::is_base_of_v<IComponent, Args> && ...), "All Args must be derived from IComponent");

public:
    /// 获取组件实例
    template <typename ComponentRealType>
    ComponentRealType *getValue()
    {
        static_assert((std::is_same_v<ComponentRealType, Args> || ...),
                      "ComponentRealType must be one of the template arguments");
        return dynamic_cast<ComponentRealType *>(getComponent(typeid(ComponentRealType).hash_code()));
    }

public:
    // 返回快速查找组件的标识符
    static ComponentIdSet getStaticComponentIds()
    {
        ComponentIdSet ids;
        (ids.insert(typeid(Args).hash_code()), ...); // 使用 typeid 获取每个组件的唯一标识符
        return ids;
    }

public:
    IEntity()
    {
        // 自动附加组件到实体
        (attach<Args>(), ...);
    }

public:
    virtual ~IEntity() = default;
};