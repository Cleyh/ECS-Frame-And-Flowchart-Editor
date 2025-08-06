#pragma once
#include "ISystem.h"

// 示例：如何使用简化后的 SubSystem 接口

class RenderSystem : public ISystem<> 
{
public:
    void setupSubSystems() {
        // 方案一：直接使用 lambda 函数
        addSubSystem([]() {
            // 渲染逻辑
            renderEntities();
        });
        
        // 方案二：使用普通函数
        addSubSystem(updateAnimations);
        
        // 方案三：使用成员函数
        addSubSystem([this]() { this->renderUI(); });
        
        // 方案四：使用宏简化（如果定义了相应的宏）
        // REGISTER_SUBSYSTEM(*this, renderShadows());
    }
    
private:
    static void renderEntities() {
        // 渲染实体的逻辑
    }
    
    static void updateAnimations() {
        // 更新动画的逻辑
    }
    
    void renderUI() {
        // 渲染UI的逻辑
    }
};

// 更进一步：基于组件查询的系统
class MovementSystem : public ISystem<>
{
public:
    void setupSubSystems() {
        // 查询拥有 Position 和 Velocity 组件的实体
        addSubSystem([this]() {
            // Query<Position, Velocity> query;
            // query.forEach([](Position& pos, Velocity& vel) {
            //     pos.x += vel.x;
            //     pos.y += vel.y;
            // }, entityList);
        });
    }
};

// 使用示例
void setupGameSystems() {
    RenderSystem renderSystem;
    renderSystem.setupSubSystems();
    renderSystem.execute(); // 执行所有渲染子系统
    
    MovementSystem movementSystem;
    movementSystem.setupSubSystems();
    movementSystem.execute(); // 执行所有移动子系统
}
