#include "GlobalQuery.h"
#include "ECSFrame/Query/IQuery.h"

void GlobalQuery::notify(EPointer<MsgObject> msg)
{
    auto real_msg = msg.cast<EntityEventMsg>();
    if (!real_msg)
    {
        return;
    }

    for (const auto &[key, query] : m_cache)
    {
        if (query)
        {
            query->update(*real_msg);
        }
    }
}

void GlobalQuery::addQuery(EPointer<IQuery> query)
{
    m_cache.insert({typeid(*query).hash_code(), query});
}

EPointer<IQuery> GlobalQuery::getQuery(size_t typeId)
{
    return m_cache.get(typeId);
}
