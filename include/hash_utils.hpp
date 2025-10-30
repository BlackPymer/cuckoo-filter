#pragma once
#include <utility>
#include <functional>

namespace std
{
    template <>
    struct hash<std::pair<size_t, size_t>>
    {
        size_t operator()(const std::pair<size_t, size_t> &p) const
        {
            return hash<size_t>{}(p.first) ^ (hash<size_t>{}(p.second) << 1);
        }
    };
}
