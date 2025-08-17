#pragma once

#include <type_traits>
#include <unordered_set>
#include <unordered_map>

#include "ECSFrame/Model/Component/IComponent.h"
#include "ECSFrame/Model/Component/IComponentHandler.h"

#include "ECSFrame/Range/ERange.h"
#include "ECSFrame/SystemUtils.h"
#include "ECSFrame/Pointer/EPointer.h"

/* Entity */
using IdSet = ESet<size_t>;
using ComponentMap = EMap<size_t, EPointer<IComponentObject>>;

/**
 * Entity基础对象
 * 组件的容器，组件不可重复
 */
class IEntityObject
{
protected:
    virtual void attach(EPointer<IComponentObject> component);

public:
    template <typename T>
    void attach()
    {
        static_assert((std::is_base_of_v<IComponentObject, T> || std::is_same_v<T, IComponentObject>),
                      "T must be IComponentObject or derived from it");
        /// todo
        /// 这里需要将组件注册到中心池
        /// 目前先直接创建一个组件对象
        /// 以后可以改为从池中获取组件对象
        EPointer<IComponentObject> component_base = EPointer<T>::make();
        attach(component_base);
    }

public:
    virtual size_t getTypeId() const;
    virtual size_t getInstanceId() const;
    virtual IdSet getComponentIds();

public:
    virtual EPointer<IComponentObject> getComponent(size_t componentId);
    virtual void setComponent(size_t componentId, EPointer<IComponentObject> component);

public:
    IEntityObject();
    virtual ~IEntityObject() = default;

protected:
    ComponentMap components;
};

/**
 * Entity模板类
 */
template <typename... Args>
class IEntity : public IEntityObject
{
public:
    using IEntityPtr = EPointer<IEntity<Args...>>;

public: /* getter */
    /// 获取组件值
    template <typename T>
    EPointer<T> getValue()
    {
        static_assert((std::is_same_v<T, Args> || ...),
                      "T must be one of the component types in this IEntity");
        auto real_component = getComponent<T>();
        if (!real_component)
        {
            return EPointer<T>(nullptr);
        }
        return real_component->Value();
    }

    /// 获取组件指针
    template <typename T>
    EPointer<IComponent<T>> getComponent()
    {
        static_assert((std::is_same_v<T, Args> || ...),
                      "T must be one of the component types in this IEntity");
        auto component = IEntityObject::getComponent(IComponent<T>::TypeId());
        if (!component)
        {
            return EPointer<IComponent<T>>(nullptr);
        }
        return component.cast<IComponent<T>>();
    }

public: /* static function */
    static IdSet ComponentIds()
    {
        IdSet ids;
        (
            ids.insert(IComponent<Args>::TypeId()),
            ...);
        return ids;
    }

    static IEntityPtr create()
    {
        return IEntityPtr::make();
    }

public:
    IEntity()
    {
        (
            attach<IComponent<Args>>(),
            ...);
    }

public:
    virtual ~IEntity() = default;
};