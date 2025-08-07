# 基于ECS架构设计的流程图绘制软件

## ECS架构实现介绍

三大基类：
- 实体： `IEntityObject`
- 组件：`IComponentObject`
- 系统：`ISystemObject`

其中系统实际还包括：
- `ISystemObject`
- `ISubSystemObject`

以上基类的拓展实现：
- IEntity
- ISubSystem
- ISystem
