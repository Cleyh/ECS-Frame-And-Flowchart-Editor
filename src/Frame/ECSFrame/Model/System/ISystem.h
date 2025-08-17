#pragma once

#include <type_traits>
#include <functional>

#include "ISystemHandler.h"
#include "ECSFrame/Range/ERange.h"

/* 前向声明 */
class IEntityObject;
class ISystemObject;
class ISystemHandlerObject;

/* System */
using HandlerList = EVector<ISystemHandlerObject *>;
using EntityList = EVector<IEntityObject *>;
using SystemList = EVector<ISystemObject *>;

/**
 * 子系统基类
 */
class ISystemObject
{
public:
    virtual void notify(/* notify */);
    virtual void addHandler(ISystemHandlerObject *handler);

private:
    HandlerList m_handlers;
};

/// 子系统接口
class ISystem
    : public ISystemObject
{
public:
    /**
     * 添加一个处理函数
     */
    template <typename Func>
    void addFunction(Func function)
    {
        // 函数包装为处理器对象
        addHandler(new ISystemHandlerWrapper<Func>(function));
    }
};

/// 全局系统接口
/// 用于管理实体和组件的全局访问点
/// 例如：注册实体、获取实体、所有系统等
class IGlobalSystem
{
public:
    IGlobalSystem();
    virtual void addSystem(ISystemObject *system);
    virtual void addEntity(IEntityObject *entity);
    virtual EntityList *getEntities();
    static IGlobalSystem *getInstance() { return &instance; };

public:
    virtual void notify(/* notify */);

private:
    static IGlobalSystem instance;

    EntityList m_entities;
    SystemList m_systems;
};