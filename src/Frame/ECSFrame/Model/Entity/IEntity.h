#pragma once

#include <type_traits>
#include <unordered_set>
#include <unordered_map>

#include "ECSFrame/Model/Component/IComponent.h"

#include "ECSFrame/Range/ERange.h"
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
    virtual void _attach(size_t componentId, EPointer<IComponentObject> pcomponent);

public:
    template <typename T>
    void attach(EPointer<IComponent<T>> pcomponent)
    {
        /// todo
        /// 这里需要将组件注册到中心池
        /// 目前先直接创建一个组件对象
        /// 以后可以改为从池中获取组件对象

        // 静态获取typeid。pcomponent可能是nullptr
        size_t componentId = IComponent<T>::TypeId();
        _attach(componentId, pcomponent);
    }

public:
    virtual size_t getTypeId() const;
    virtual size_t getInstanceId() const;
    virtual IdSet getComponentIds();

public:
    virtual EPointer<IComponentObject> getComponent(size_t componentId);
    virtual void setComponent(size_t componentId, EPointer<IComponentObject> pcomponent);

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
    EPointer<T> Value()
    {
        static_assert((std::is_same_v<T, Args> || ...),
                      "T must be one of the component types in this IEntity");
        auto real_component = Component<T>();
        if (!real_component)
        {
            return EPointer<T>(nullptr);
        }
        return real_component->Value();
    }

    /// 获取组件指针
    template <typename T>
    EPointer<IComponent<T>> Component()
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

    template <typename... UArgs>
    static IEntityPtr create(UArgs &&...uargs)
    {
        // New entity with empty component pointer;
        auto entity = IEntityPtr::make();
        // Init all components
        if constexpr (sizeof...(UArgs) > 0)
        {
            static_assert(sizeof...(Args) == sizeof...(UArgs),
                          "Error: Argument count does not match template parameter count.");

            // fold expression + static_assert
            static_assert((std::is_same_v<std::decay_t<UArgs>, Args> && ...),
                          "Error: Argument types or order do not match IEntity<Args...>.");

            // Attach components with user arguments
            (
                entity->attach(
                    EPointer<IComponent<Args>>::make(
                        std::forward<UArgs>(uargs))),
                ...);
        }
        else
        {
            // no arguments initialization
            (entity->attach(
                 EPointer<IComponent<Args>>::make()),
             ...);
        }
        return entity;
    }

public:
    /// @brief 构造函数
    /// 默认构造函数不会初始化组件
    /// 初始化组件，使用create()方法
    IEntity()
    {
        (attach(EPointer<IComponent<Args>>(nullptr)),
         ...);
    }

public:
    virtual ~IEntity() = default;
};