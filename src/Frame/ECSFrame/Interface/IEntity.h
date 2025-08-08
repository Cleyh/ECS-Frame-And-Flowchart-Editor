#pragma once
#include <type_traits>
#include <unordered_set>
#include <unordered_map>
#include "../SystemUtils.h"
#include "IComponent.h"

using ComponentIdSet = std::unordered_set<size_t>;
using ComponentMap = std::unordered_map<size_t, IComponent *>;

/// Entity基础对象
/// 组件的容器，组件不可重复
class IEntityObject
{
protected:
    virtual void attach(IComponent *component);

public:
    template <typename T>
    void attach()
    {
        static_assert(std::is_base_of_v<IComponent, T>,
                      "T must be IComponent or derived from it");
        attach(new T());
    }

public:
    virtual ComponentIdSet getComponentIds() const;

    virtual IComponent *getComponent(size_t componentId);

    virtual void setComponent(size_t componentId, IComponent *component);

    IEntityObject();

    virtual ~IEntityObject() = default;

protected:
    ComponentMap components;
};

/// Entity模板类
template <typename... Args>
class IEntity : public IEntityObject
{
    static_assert((!std::is_base_of_v<IComponent, Args> && ...),
                  "All Args should be not IComponent");

public:
    template <typename T>
    IComponentWrapper<T> *getComponent()
    {
        static_assert((std::is_same_v<T, Args> || ...),
                      "T arg must be one of the template arguments");
        IComponentWrapper<T> *component = dynamic_cast<IComponentWrapper<T> *>(
            IEntityObject::getComponent(typeid(IComponentWrapper<T>).hash_code()));
        if (component)
        {
            return component;
        }
        return nullptr;
    }

    template <typename T>
    T *getValue()
    {
        static_assert((std::is_same_v<T, Args> || ...),
                      "T arg must be one of the template arguments");
        if (auto component = getComponent<T>())
        {
            return component->Value();
        }
        return nullptr;
    }

public:
    static ComponentIdSet getStaticComponentIds()
    {
        ComponentIdSet ids;
        (ids.insert(typeid(Args).hash_code()), ...);
        return ids;
    }

public:
    IEntity()
    {
        (attach<IComponentWrapper<Args>>(),
         ...);
    }

public:
    virtual ~IEntity() = default;
};