#pragma once
#include <type_traits>
#include <functional>
#include <vector>

// 前向声明
class IEntityObject;
class ISubSystemObject;
class ISystemObject;
class IQuery;

using SubSystemList = std::vector<ISubSystemObject *>;

using EntityList = std::vector<IEntityObject *>;
using SystemList = std::vector<ISystemObject *>;

/// 子系统基类
class ISubSystemObject
{
public:
    virtual void executeBase(IQuery *queries) = 0;
};

/// 子系统接口
/// 表示每个处理实体或组件的逻辑单元，即一个函数实现
template <typename QueryType>
class ISubSystem
    : public ISubSystemObject
{
    static_assert(std::is_base_of_v<IQuery, QueryType>,
                  "All QueryType must be derived from IQuery");

public:
    virtual void execute(QueryType &queries) = 0;
    void executeBase(IQuery *queries) override
    {
        auto typeQuery = QueryType();
        execute(typeQuery);
    }
};

/// 系统基类
class ISystemObject
{
public:
    virtual void notify(/* notify */);
    virtual void addSubSystem(ISubSystemObject *subSystem);

private:
    SubSystemList subSystems;
};

/// 系统接口
/// 一套子系统的集合，可以按顺序执行子系统或异步执行
/// 例如：渲染系统、物理系统等
template <typename... SubSystems>
class ISystem
    : public ISystemObject
{
    static_assert((std::is_base_of_v<ISubSystemObject, SubSystems> && ...),
                  "All SubSystems must be derived from ISubSystem");

public:
    ISystem()
    {
        (addSubSystem(new SubSystems()), ...);
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