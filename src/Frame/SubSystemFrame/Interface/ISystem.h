#pragma once

template <typename... SubSystems>
class ISystem
{
    static_assert((std::is_base_of_v<ISubSystem, SubSystems> && ...), "All SubSystems must be derived from ISubSystem");
};