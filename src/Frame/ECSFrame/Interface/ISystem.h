#pragma once

#include <type_traits>
#include <functional>

#include "ECSFrame/Range/RangeSet.h"

// 前向声明
class IEntityObject;
class ISubSystemObject;
class ISystemObject;
class IQuery;

/* System */
using SubSystemList = EVector<ISubSystemObject *>;
using EntityList = EVector<IEntityObject *>;
using SystemList = EVector<ISystemObject *>;

namespace ECS
{
    namespace SystemUtils
    {
        // 检查是否为函数指针
        template <typename T>
        struct is_function_pointer : std::false_type
        {
        };

        template <typename R, typename... Args>
        struct is_function_pointer<R (*)(Args...)> : std::true_type
        {
        };

        // 提取函数参数类型
        template <typename T>
        struct function_traits;

        template <typename R, typename... Args>
        struct function_traits<R (*)(Args...)>
        {
            using return_type = R;
            static constexpr size_t arity = sizeof...(Args);

            template <size_t N>
            struct argument
            {
                static_assert(N < arity, "error: invalid parameter index.");
                using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
            };
        };

        // 检查函数参数是否为IQuery引用
        template <auto Func>
        constexpr static bool is_valid_function()
        {
            using FuncType = decltype(Func);

            // 检查是否为函数指针
            if constexpr (!is_function_pointer<FuncType>::value)
            {
                return false;
            }

            // 检查参数数量是否为1
            if constexpr (function_traits<FuncType>::arity != 1)
            {
                return false;
            }

            // 检查参数类型是否为IQuery引用或其派生类引用
            using ArgType = typename function_traits<FuncType>::template argument<0>::type;
            using BaseArgType = std::remove_reference_t<ArgType>;

            return std::is_reference_v<ArgType> &&
                   std::is_base_of_v<IQuery, BaseArgType>;
        }
    } // namespace SystemUtils
} // namespace ECS

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

/// 函数化系统接口
template <auto... T>
class ISystemFunctionalSupport
    : public ISystemObject
{
    // 编译期断言检查所有函数
    static_assert((ECS::SystemUtils::is_valid_function<T>() && ...),
                  "All functions must have signature: void func(IQuery& or derived&)");
public:
    ISystemFunctionalSupport()
    {
        (addSubSystem(T), ...);
    }

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