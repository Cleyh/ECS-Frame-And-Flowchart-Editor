#include "IQuery.h"

#include <iostream>

#include "ECSFrame/Global/GlobalPool.h"
#include "ECSFrame/Global/GlobalQuery.h"
#include "ECSFrame/SystemUtils.h"

/**
 * IQuery Util Functions
 */

bool checkMsgComponent(EVector<size_t> &config, EVector<size_t> &changed)
{
    for (auto &changed_id : changed)
    {
        if (!config.contains(changed_id))
        {
            return false;
        }
    }
    return true;
}

/**
 * IQuery Implementation
 */

IQuery::IQuery()
{
}

void IQuery::doQuery()
{
    m_results.clear();
    auto entities = ECS::Global::Entities();
    for (auto &pair : *entities)
    {
        auto entity = pair.second;
        if (m_matchFunc(entity))
        {
            m_results.push_back(entity);
        }
    }
}

void IQuery::update(EntityEventMsg &msg)
{
    switch (msg.type)
    {
    case EntityEventType::ENTITY_ADD:
    case EntityEventType::ENTITY_REMOVE:

        m_dirty = true;
        break;

    case EntityEventType::COMPONENT_ATTACH:
    case EntityEventType::COMPONENT_DETACH:

    {
        if (checkMsgComponent(m_config, msg.componentIds))
        {
            m_dirty = true;
        }
    }

    default:
        break;
    }
}

EVector<EPointer<IEntityObject>> IQuery::getAllResults()
{
    if (m_dirty)
    {
        doQuery();
        m_dirty = false;
    }
    return m_results;
}