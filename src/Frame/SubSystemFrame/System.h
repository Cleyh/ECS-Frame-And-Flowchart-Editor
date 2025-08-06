#pragma once
#include <qvector.h>

typedef QVector<IEntityObject *> EntityList;

class GlobleSystem
{
public:
    void registerEntity(IEntityObject *entity)
    {
        // 将实体注册到系统中
        entities.append(entity);
        // 测试： void* 实例为 IEntity
        auto ids = entity->getComponentIds(); // 调用获取组件标识符的方法
    }

    // 其他系统相关方法
private:
    EntityList entities; // 存储实体的列表
};