#pragma once
#include <type_traits>
#include <functional>
#include <memory>

template <typename T>
class EPointer
    : public std::shared_ptr<T>
{
};
