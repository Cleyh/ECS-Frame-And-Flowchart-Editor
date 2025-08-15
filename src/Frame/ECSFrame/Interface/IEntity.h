#pragma once

#include <type_traits>
#include <unordered_set>
#include <unordered_map>

#include "IComponent.h"

#include "ECSFrame/Range/RangeSet.h"
#include "ECSFrame/SystemUtils.h"
#include "ECSFrame/Pointer/EPointer.h"

/* Entity */
using IdSet = ESet<size_t>;
using ComponentMap = EMap<size_t, IComponentObject *>;

/// Entity基础对象
/// 组件的容器，组件不可重复
class IEntityObject
{
protected:
    virtual void attach(IComponentObject *component);

public:
    template <typename T>
    void attach()
    {
        static_assert(std::is_base_of_v<IComponentObject, T>,
                      "T must be IComponentObject or derived from it");
        attach(new T());
    }

public:
    virtual size_t getTypeId() const;

    virtual size_t getInstanceId() const;

public:
    virtual IdSet getComponentIds();

    virtual IComponentObject *getComponent(size_t componentId);

    virtual void setComponent(size_t componentId, IComponentObject *component);

    IEntityObject();

    virtual ~IEntityObject() = default;

protected:
    ComponentMap components;
};

/// Entity模板类
template <typename... Args>
class IEntity : public IEntityObject
{
public:
    template <typename T>
    IComponent<T> *getComponent()
    {
        static_assert((std::is_same_v<T, Args> || ...),
                      "T arg must be one of the template arguments");
        IComponent<T> *component =
            dynamic_cast<IComponent<T> *>(IEntityObject::getComponent(typeid(IComponent<T>).hash_code()));
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
    static IdSet ComponentIds()
    {
        IdSet ids;
        (
            ids.insert(IComponent<Args>::TypeId()),
            ...);
        return ids;
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