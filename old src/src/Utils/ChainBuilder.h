//
// Created by unbd_ on 2025/4/30.
//

#ifndef CHAINBUILDER_H
#define CHAINBUILDER_H

#include <utility>

// 链式构造器核心模板（CRTP基类）
template<typename Derived>
class ChainBuilder {
public:
    // 通用链式入口
    static Derived builder() {
        return Derived();
    }

    // 返回最终构建对象（可选）
    Derived build() {
        return std::move(static_cast<Derived &>(*this));
    }
};

// setter和getter方法生成宏
#define PROPERTY_SETGET(Derived,Type, Name) \
protected: \
Type m_##Name; \
public: \
Derived& set_##Name(const Type& value) { \
this->m_##Name = value; \
return *static_cast<Derived*>(this); \
} \
Type get_##Name() const { \
return m_##Name; \
}
#endif //CHAINBUILDER_H
