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
    for(auto &subSystem : subSystems) {
        if (subSystem) {
            // Assuming nullptr for now, replace with actual query if needed
            subSystem->executeBase(nullptr); 
        } else {
            qWarning() << "Null subSystem encountered in ISystemObject::notify.";
        }
    }
}

void ISystemObject::addSubSystem(ISubSystemObject *subSystem)
{
    if (subSystem) {
        subSystems.append(subSystem);
    } else {
        qWarning() << "Attempted to add a null subSystem to ISystemObject.";
    }
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
    m_systems.append(system);
}

void IGlobalSystem::addEntity(IEntityObject *entity)
{
    m_entities.append(entity);
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
