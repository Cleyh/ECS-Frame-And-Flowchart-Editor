#pragma once

#include <type_traits>
#include <functional>
/*
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

            // return std::is_reference_v<ArgType> &&
            //    std::is_base_of_v<IQuery, BaseArgType>;
            return std::is_reference(ArgType);
        }
    } // namespace SystemUtils
} // namespace ECS
*/

/// 处理器对象基类接口
class ISystemHandlerObject
{
public:
    virtual ~ISystemHandlerObject() = default;
    /// 可重写执行函数
    virtual void execute() = 0;
};

/**
 * 处理器对象
 */
template <typename R, typename... Args>
class ISystemHandler
    : public ISystemHandlerObject
{
public:
    ISystemHandler(R (*function)(Args...))
        : m_function(function)
    {
    }

    void execute() override
    {
        // 执行函数
        m_function(createArg<Args>()...);
    }

private:
    template <typename T>
    auto createArg()
    {
        if constexpr (std::is_reference_v<T>)
        {
            using RefType = std::remove_reference_t<T>;
            static RefType ref;   // 静态变量用于返回引用
            return std::ref(ref); // 返回静态变量的引用
        }
        else
        {
            return T{}; // 返回默认构造的值
        }
    }

    std::function<R(Args...)> m_function; // 存储函数指针
};

/**
 * 处理器对象模板类
 */
template <typename Func>
class ISystemHandlerWrapper
{
    static_assert(std::is_function_v<Func>,
                  "Func must be a function type");
};

/**
 * 处理器对象模板类特化
 * 分离函数指针和函数对象的处理
 */
template <typename R, typename... Args>
class ISystemHandlerWrapper<R (*)(Args...)>
    : public ISystemHandler<R, Args...>
{
    /// 模板参数检查
    /// Func - 函数指针类型，参数必须是IQuery引用或其派生类引用
    /// todo

public:
    ISystemHandlerWrapper(R (*function)(Args...))
        : ISystemHandler<R, Args...>(function)
    {
    }
};