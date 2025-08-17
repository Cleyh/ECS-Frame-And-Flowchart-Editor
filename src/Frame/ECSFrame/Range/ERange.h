#pragma once

#include <vector>
#include <unordered_map>
#include <unordered_set>

template <typename T>
class EVector
    : public std::vector<T>
{
};

template <typename K, typename V>
class EMap
    : public std::unordered_map<K, V>
{
public: /* getter */
    /* Get value by key, if key not found, return defaultValue */
    V get(const K &key, const V &defaultValue = V()) const
    {
        auto it = this->find(key);
        if (it != this->end())
        {
            return it->second;
        }
        return defaultValue;
    }

    std::vector<K> keys()
    {
        std::vector<K> keyList;
        for (const auto &pair : *this)
        {
            keyList.push_back(pair.first);
        }
        return keyList;
    }
};

template <typename T>
class ESet
    : public std::unordered_set<T>
{
public: /* static function */
    static ESet<T> fromVector(const std::vector<T> &vec)
    {
        ESet<T> set;
        for (const auto &item : vec)
        {
            set.insert(item);
        }
        return set;
    }

public:
    /*
     * If contains all elements of other
     * it returns true, otherwise false.
     */
    bool contains(const ESet<T> &other) const
    {
        for (const auto &item : other)
        {
            if (this->find(item) == this->end())
            {
                return false;
            }
        }
        return true;
    }
};