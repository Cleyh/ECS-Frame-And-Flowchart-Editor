# 基于ECS架构设计的流程图绘制软件

## ECS架构实现介绍

TODO


## Feature TODO

2025/8/19
- 组件位索引
- Entity组件位掩码
- 全局EntityMap优化

2025/8/18
- 条件查询[Done]
- 查询缓存机制：列表交并差、结果缓存
- 组件数据监控

# Query
## Global Query

保存`Map<key, IQuery*>`.

缓存过期/数据更新时触发更新，更新方式：
- 遍历Map，调用IQuery->update();

## QueryCondition


