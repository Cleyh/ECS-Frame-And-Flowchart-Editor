#pragma once

// Forward declarations

class IGlobalNotifyObject;
class GlobalPool;
class GlobalQuery;
class IEntityObject;
class IComponentObject;
class IQuery;


// Type aliases

template <typename T>
class EPointer;
template <typename K, typename V>
class EMap;
template <typename T>
class EVector;
template <typename T>
class ESet;

/**
 *  Entity
 */
using IdSet = ESet<size_t>;

/**
 * Global
 */

class ComponentConstructor;
/// @brief 全局实体池
using EntityMap = EMap<size_t, EPointer<IEntityObject>>;
/// @brief 全局组件池
/// 用于存储所有实体
/// key: 实例ID
/// value: 实体指针
using ComponentMap = EMap<size_t, EPointer<IComponentObject>>;
/// @brief 全局组件类型注册表
/// key: 组件ID
/// value: 该组件的构造器
using ComponentRegistry = EMap<size_t, EPointer<ComponentConstructor>>;
