#pragma once

#include <type_traits>

#include "ECSFrame/Model/Entity/IEntity.h"
#include "ECSFrame/Model/Component/IComponent.h"
#include "ECSFrame/Range/ERange.h"

/* 前向声明 */
class ComponentConstructor;

/* Global */

using EntityMap = EMap<size_t, EPointer<IEntityObject>>;
/// @brief 全局实体池
/// 用于存储所有实体
/// key: 实例ID
/// value: 实体指针
using ComponentMap = EMap<size_t, EPointer<IComponentObject>>;
/// @brief 全局组件类型注册表
/// key: 组件ID
/// value: 该组件的构造器
using ComponentRegistry = EMap<size_t, EPointer<ComponentConstructor>>;

class GlobalPool
{
public:
    /// @brief 添加实体到全局实体池
    /// @param entity 实体指针
    /// @tparam Args... 实体组件类型，入参自动推导
    template <typename... Args>
    void addEntity(EPointer<IEntity<Args...>> entity)
    {
        (registerComponent<Args>(), ...);

        // auto ids = IEntity<Args...>::ComponentIds();
        // for (auto id : ids)
        // {
        //     if (auto component = entity->getComponent(id);
        //         component && m_components.contains(id))
        //         // 如果组件已存在，则不允许重复添加
        //         return;
        // }

        m_entities.insert({reinterpret_cast<size_t>(entity.get()), entity});
        // for (auto id : ids)
        // {
        //     m_components.insert(id, entity->getComponent(id));
        // }
    }

    template <typename T>
    bool registerComponent()
    {
        auto id = IComponent<T>::TypeId();
        if (m_component_registry.contains(id))
        {
            return false;
        }
        m_component_registry.insert({id, EPointer<ComponentConstructor>::make()});
        return true;
    }

private:
    EntityMap m_entities;
    ComponentMap m_components;
    ComponentRegistry m_component_registry;
};

class ComponentConstructor
{
};