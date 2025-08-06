#pragma once
#include <functional>
#include <tuple>
#include <type_traits>

// 更进阶的 ECS 框架设计，参考 Bevy

// 组件标记接口
class IComponent {};

// 示例组件
struct Position : public IComponent {
    float x, y, z;
};

struct Velocity : public IComponent {
    float dx, dy, dz;
};

struct Health : public IComponent {
    int current, max;
};

// 查询系统
template<typename... Components>
class Query {
public:
    using QueryCallback = std::function<void(Components&...)>;
    
    void forEach(QueryCallback callback) {
        // 这里应该实现实际的组件查询逻辑
        // 遍历所有实体，找到拥有指定组件的实体，调用回调
    }
};

// 系统函数类型定义
template<typename... Queries>
using SystemFunction = std::function<void(Queries&...)>;

// 简化的系统类
class SimpleSystem {
private:
    std::vector<std::function<void()>> systemFunctions;
    
public:
    // 添加系统函数
    template<typename Func>
    void addSystem(Func&& func) {
        systemFunctions.push_back(std::forward<Func>(func));
    }
    
    // 执行所有系统
    void execute() {
        for (auto& system : systemFunctions) {
            system();
        }
    }
};

// 使用示例：类似 Bevy 的用法
void setupBevyLikeSystems() {
    SimpleSystem gameSystem;
    
    // 移动系统
    gameSystem.addSystem([]() {
        Query<Position, Velocity> query;
        query.forEach([](Position& pos, Velocity& vel) {
            pos.x += vel.dx;
            pos.y += vel.dy;
            pos.z += vel.dz;
        });
    });
    
    // 生命值系统
    gameSystem.addSystem([]() {
        Query<Health> query;
        query.forEach([](Health& health) {
            if (health.current <= 0) {
                // 处理死亡逻辑
            }
        });
    });
    
    // 渲染系统
    gameSystem.addSystem([]() {
        Query<Position> renderQuery;
        renderQuery.forEach([](Position& pos) {
            // 渲染实体在指定位置
        });
    });
    
    // 执行游戏循环
    gameSystem.execute();
}

// 更进一步：支持资源访问的系统
template<typename... Resources>
class ResourceQuery {
public:
    template<typename Func>
    void access(Func func) {
        // 访问全局资源
    }
};

// 复合查询示例
void complexSystemExample() {
    SimpleSystem system;
    
    system.addSystem([]() {
        // 同时查询多种组件和资源
        Query<Position, Velocity> entityQuery;
        ResourceQuery<> resourceQuery; // 可以访问时间、输入等资源
        
        entityQuery.forEach([&](Position& pos, Velocity& vel) {
            // 使用实体组件和全局资源的逻辑
        });
    });
}
