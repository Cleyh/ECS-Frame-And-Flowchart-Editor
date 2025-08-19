#include "IQuery.h"

/**
 * IQuery Implementation
 */

void IQuery::doQuery()
{
    results.clear();
    auto entities = ECS::Global::Entities();
    for (auto &pair : *entities)
    {
        auto entity = pair.second;
        if (matchFunc(entity))
        {
            results.push_back(entity);
        }
    }
}

void IQuery::update()
{
    dirty = true;
}

EVector<EPointer<IEntityObject>> IQuery::getAllResults()
{
    // if (dirty)
    {
        doQuery();
        dirty = false;
    }
    return results;
}