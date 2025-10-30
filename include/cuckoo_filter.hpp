#pragma once
#include <any>
#include <vector>
#include <unordered_set>
#include <string>
#include "hash_utils.hpp"

struct FilterConfig
{
    unsigned int filterSize = 2 << 16;
    unsigned int maxCycles = 2 << 8;
};

class CuckooFilter
{
public:
    CuckooFilter(FilterConfig config = FilterConfig());
    ~CuckooFilter();
    FilterConfig GetConfiguration();
    void SetConfiguration(FilterConfig config);

    void Insert(std::string element);
    void Delete(std::string element);
    bool Check(std::string element);
    void Clear();

private:
    FilterConfig _configuration;
    std::unordered_set<std::pair<size_t, size_t>> elements;

    size_t _Fingerprint(std::string element);
    void _Resize();
    void _Insert(size_t hash1, size_t fingerprint);

    size_t *row1;
    size_t *row2;
};