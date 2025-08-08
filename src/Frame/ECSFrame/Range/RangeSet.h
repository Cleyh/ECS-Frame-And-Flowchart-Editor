#include <unordered_set>

template<typename... T>
class RangeSet
{
private:
    std::unordered_set<size_t> ids;
};