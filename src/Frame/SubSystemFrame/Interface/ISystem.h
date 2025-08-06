#pragma once
#include <functional>
#include <vector>
#include <QVector>

class IEntityObject; // 前向声明
class ISubSystem;
template <typename... SubSystems>
class ISystem;
class IGlobalSystem;

using EntityList = QVector<IEntityObject *>;

// 方案一：使用 std::function 简化 SubSystem
using SubSystemFunction = std::function<void()>;
using SubSystemList = std::vector<SubSystemFunction>;

/// 子系统接口（保留原有接口用于兼容性）
/// 表示每个处理实体或组件的逻辑单元，即一个函数实现
class ISubSystem
{
public:
    virtual void execute() = 0;
    
    // 转换为函数形式
    SubSystemFunction toFunction() {
        return [this]() { this->execute(); };
    }
};

/// 系统接口
/// 一套子系统的集合，可以按顺序执行子系统或异步执行
/// 例如：渲染系统、物理系统等
template <typename... SubSystems>
class ISystem
{
    static_assert((std::is_base_of_v<ISubSystem, SubSystems> && ...), "All SubSystems must be derived from ISubSystem");
    
private:
    SubSystemList subSystems;
    
public:
    // 添加函数式子系统
    void addSubSystem(SubSystemFunction func) {
        subSystems.push_back(func);
    }
    
    // 添加 lambda 或函数对象
    template<typename Func>
    void addSubSystem(Func&& func) {
        subSystems.push_back(std::forward<Func>(func));
    }
    
    // 执行所有子系统
    void execute() {
        for (auto& subSystem : subSystems) {
            subSystem();
        }
    }
};

// 方案二：模板化的系统查询和组件访问
template<typename... Components>
using QueryFunction = std::function<void(Components&...)>;

template<typename... Components>
class Query {
public:
    void forEach(QueryFunction<Components...> func, EntityList& entities) {
        // 这里需要实现实际的组件查询逻辑
        // 遍历实体，获取对应组件，调用函数
    }
};

// 方案三：使用宏简化子系统注册
#define REGISTER_SUBSYSTEM(SystemName, Function) \
    SystemName.addSubSystem([&]() { Function; });

/// @brief 全局系统接口
/// 用于管理实体和组件的全局访问点
/// 例如：注册实体、获取实体、所有系统等
class IGlobalSystem
{
public:
    virtual void addEntity(IEntityObject *entity)
    {
        entityList.append(entity);
    }

private:
    EntityList entityList;
};
