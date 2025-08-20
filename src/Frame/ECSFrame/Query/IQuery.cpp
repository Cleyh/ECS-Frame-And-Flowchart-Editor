#include "IQuery.h"

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

void IQuery::update(QueryNotifyMsg &msg)
{
    switch (msg.type)
    {
    case NotifyType::ENTITY_ADD:
    case NotifyType::ENTITY_REMOVE:

        m_dirty = true;
        break;

    case NotifyType::COMPONENT_ATTACH:
    case NotifyType::COMPONENT_DETACH:

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
    // if (m_dirty)
    {
        doQuery();
        m_dirty = false;
    }
    return m_results;
}