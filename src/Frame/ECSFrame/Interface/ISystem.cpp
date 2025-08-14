#include "ISystem.h"
#include <QDebug>

/////////////////////////////////////////////////
/// ISubSystem Implementation
/////////////////////////////////////////////////

///////////////////////////////////////////////////
/// ISystemObject && ISystem Implementation
/////////////////////////////////////////////////

void ISystemObject::notify(/* notify */)
{
}
void ISystemObject::addHandler(ISystemHandlerObject *handler)
{
}
/////////////////////////////////////////////////
/// IGlobalSystem Implementation
/////////////////////////////////////////////////

IGlobalSystem IGlobalSystem::instance;

IGlobalSystem::IGlobalSystem()
{
}

void IGlobalSystem::addSystem(ISystemObject *system)
{
    m_systems.push_back(system);
}

void IGlobalSystem::addEntity(IEntityObject *entity)
{
    m_entities.push_back(entity);
}

EntityList *IGlobalSystem::getEntities()
{
    return &m_entities;
}

void IGlobalSystem::notify(/* notify */)
{
    for (auto &system : m_systems) {
        if (system) {
            system->notify();
        } else {
            qWarning() << "Null system encountered in IGlobalSystem::notify.";
        }
    }
}
