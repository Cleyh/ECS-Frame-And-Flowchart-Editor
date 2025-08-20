#pragma once
#include <type_traits>
#include <iostream>
#include <functional>
#include <memory>

/**
 * EPointer 智能指针封装
 * useage:
 * ```cpp
 * // 无参构造，初始为nullptr
 * EPointer<MyClass> ptr{};
 * EPointer<MyClass> ptr{nullptr};
 *
 * // 初始化，等同new MyClass()
 * EPointer<MyClass> ptr = EPointer<MyClass>::make();
 * EPointer<MyClass> ptr = EPointer<MyClass>::make(arg1, arg2, ...);
 * ```
 */
template <typename T>
class EPointer
    : public std::shared_ptr<T>
{
public:
    using std::shared_ptr<T>::shared_ptr; // 继承构造函数
    // 默认构造
    EPointer() = default;

    // 从shared_ptr构造
    EPointer(const std::shared_ptr<T> &ptr) : std::shared_ptr<T>(ptr) {}
    EPointer(std::shared_ptr<T> &&ptr) : std::shared_ptr<T>(std::move(ptr)) {}

    // 拷贝和移动构造
    EPointer(const EPointer &) = default;
    EPointer(EPointer &&) = default;
    EPointer &operator=(const EPointer &) = default;
    EPointer &operator=(EPointer &&) = default;

    // 从父类的shared_ptr赋值
    EPointer &operator=(const std::shared_ptr<T> &ptr)
    {
        std::shared_ptr<T>::operator=(ptr);
        return *this;
    }

    EPointer &operator=(std::shared_ptr<T> &&ptr)
    {
        std::shared_ptr<T>::operator=(std::move(ptr));
        return *this;
    }

public: /* 封装常用方法 */
    /// Initialize, equalent to std::make_shared<T>()
    template <typename... Args>
    static EPointer<T> make(Args &&...args)
    {
        return EPointer<T>(std::make_shared<T>(std::forward<Args>(args)...));
    }

    /// 类型转换
    template <typename U>
    EPointer<U> cast() const
    {
        auto casted = std::dynamic_pointer_cast<U>(*this);
        return EPointer<U>(casted);
        // return std::static_pointer_cast<U>(*this);
    }
};
