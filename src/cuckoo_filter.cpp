#include "../include/cuckoo_filter.hpp"
#include "../include/exceprions.hpp"

#include <functional>
#include <cstdlib>
#include <ctime>

CuckooFilter::CuckooFilter(FilterConfig config)
{
    _configuration = config;
    row1 = new size_t[_configuration.filterSize]();
    row2 = new size_t[_configuration.filterSize]();

    std::srand(std::time(nullptr));
}

CuckooFilter::~CuckooFilter()
{
    Clear();
}

void CuckooFilter::Clear()
{
    delete[] row1;
    delete[] row2;
    elements.clear();
}

FilterConfig CuckooFilter::GetConfiguration()
{
    return _configuration;
}

void CuckooFilter::SetConfiguration(FilterConfig config)
{
    _configuration = config;
}

void CuckooFilter::Insert(std::string element)
{
    size_t hash1 = std::hash<std::string>{}(element);
    size_t fingerprint = _Fingerprint(element);
    _Insert(hash1, fingerprint);
}

void CuckooFilter::_Insert(size_t hash1, size_t fingerprint)
{
    hash1 = hash1 % _configuration.filterSize;
    for (int i = 0; i < _configuration.maxCycles; ++i)
    {
        if (row1[hash1] != 0 && row1[hash1] != fingerprint)
        {
            size_t hash2 = (hash1 ^ fingerprint) % _configuration.filterSize;
            if (row2[hash2] != 0 && row2[hash2] != fingerprint)
            {
                if (_configuration.resizable)
                    elements.insert(std::make_pair(hash1, fingerprint));
                if (std::rand() % 2)
                {
                    size_t new_fingerprint = row2[hash2];
                    row2[hash2] = fingerprint;
                    fingerprint = new_fingerprint;
                    hash1 = (hash2 ^ fingerprint) % _configuration.filterSize;
                }
                else
                {
                    size_t new_fingerprint = row1[hash1];
                    row1[hash1] = fingerprint;
                    fingerprint = new_fingerprint;
                }
            }
            else
            {
                row2[hash2] = fingerprint;
                return;
            }
        }
        else
        {
            row1[hash1] = fingerprint;
            return;
        }
    }
    if (_configuration.resizable)
        _Resize();
    else
        throw FilterOverflowException("Filter was overflowed");
}

void CuckooFilter::_Resize()
{
    _configuration.filterSize *= 2;
    std::unordered_set<std::pair<size_t, size_t>> pairs = elements;
    Clear();
    for (std::pair<size_t, size_t> pair : pairs)
        _Insert(pair.first, pair.second);
}

bool CuckooFilter::Check(std::string element)
{
    size_t hash1 = (std::hash<std::string>{}(element)) % _configuration.filterSize;
    size_t fingerprint = _Fingerprint(element);
    return row1[hash1] == fingerprint ||
           row2[hash1 ^ fingerprint] == fingerprint;
}

void CuckooFilter::Delete(std::string element)
{
    size_t hash1 = (std::hash<std::string>{}(element)) % _configuration.filterSize;
    size_t fingerprint = _Fingerprint(element);
    if (row1[hash1] == fingerprint)
        row1[hash1] = 0;
    else
    {
        size_t hash2 = (hash1 ^ fingerprint) % _configuration.filterSize;
        if (row2[hash2] == fingerprint)
            row2[hash2] = 0;
    }
    if (_configuration.resizable)
        elements.erase(std::make_pair(hash1, fingerprint));
}

size_t CuckooFilter::_Fingerprint(std::string element)
{
    return (std::hash<std::string>{}(element) >> 8) & 0xFF;
}