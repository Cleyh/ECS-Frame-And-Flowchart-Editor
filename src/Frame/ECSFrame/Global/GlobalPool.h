#pragma once

#include <type_traits>

#include "ECSFrame/Forward.h"
#include "ECSFrame/Notify/Notifier.h"

#include "ECSFrame/Model/Component/IComponent.h"
#include "ECSFrame/Model/Entity/IEntity.h"

#include "ECSFrame/Pointer/EPointer.h"
#include "ECSFrame/Range/ERange.h"

/**
 * 全局池
 * 用于存储全局实体和组件
 *
 * ComponentMap - 会接管所有entity中组件的所有权
 * EntityMap - 用于存储所有实体
 * ComponentRegistry - 用于存储所有组件类型的注册信息
 */
class GlobalPool
    : public IGlobalNotifyObject,
      public IGlobalNotify<GlobalQuery>
{
public:
    GlobalPool() = default;
    ~GlobalPool() = default;

    virtual void notify(EPointer<MsgObject> msg) override;

public: /* Getter */
    EPointer<EntityMap> getEntities()
    {
        return m_entities;
    }
    EPointer<ComponentMap> getComponents() { return m_components; }
    EPointer<ComponentRegistry> getComponentRegistry() { return m_component_registry; }

public: /* Remove Functions */
    bool removeEntity(size_t entityId);
    bool removeComponent(size_t componentId);
    bool removeComponentType(size_t componentTypeId);

public: /* Add Functions */
    /// @brief 添加实体到全局实体池
    /// @param entity 实体指针
    /// @tparam Args... 实体组件类型，入参自动推导
    template <typename... Args>
    void addEntity(EPointer<IEntity<Args...>> &entity)
    {
        // 将实体中的组件插入全局池，如果组件已经存在，则entity代表可能重复，entity不能插入
        IdSet entity_ids = entity->getComponentIds();
        for (auto id : entity_ids)
        {
            if (m_components->contains(id))
            {
                return;
            }
        }

        // 将实体插入全局实体池
        auto instance_id = reinterpret_cast<size_t>(entity.get());
        if (m_entities->contains(instance_id))
        {
            return; // 实体已存在
        }

        m_entities->insert({instance_id, entity});

        // 注册组件，在entity成功插入后再注册组件
        {
            (registerComponent<Args>(), ...);
        }

        auto msg = EPointer<EntityEventMsg>(
            new EntityEventMsg(
                EntityEventType::ENTITY_ADD, instance_id, EVector<size_t>::FromSet(entity_ids)
            )
        );
        SendNotify(msg);
    }

    template <typename T>
    bool registerComponent()
    {
        auto id = IComponent<T>::TypeId();
        if (m_component_registry->contains(id))
        {
            return false;
        }
        m_component_registry->insert({id, EPointer<ComponentConstructor>::make()});
        return true;
    }

private:
    EPointer<EntityMap> m_entities = EPointer<EntityMap>::make();
    EPointer<ComponentMap> m_components = EPointer<ComponentMap>::make();
    EPointer<ComponentRegistry> m_component_registry = EPointer<ComponentRegistry>::make();
};

class ComponentConstructor
{
};