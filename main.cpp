#include <iostream>
#include "include/cuckoo_filter.hpp"
#include <random>
#include <unordered_set>
void test_filter(int epoches)
{
    double correct_answers = 0;
    double answers = 0;
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, 2);
    std::uniform_int_distribution<> element(0, 2 << 29);
    CuckooFilter filter;
    std::unordered_set<int> set;
    for (int i = 0; i < epoches; ++i)
    {
        int op = dist(gen);
        switch (op)
        {
        case 0:
        {
            // insert testing
            int el = element(gen);
            filter.Insert(std::to_string(el));
            set.insert(el);
            break;
        }
        case 1:
        {
            // check testing
            answers++;
            int el = element(gen);
            correct_answers += ((set.find(el) != set.end()) == (filter.Check(std::to_string(el))));
            break;
        }
        case 2:
            // delete testing
            int el = element(gen);
            filter.Delete(std::to_string(el));
            set.erase(el);
            break;
        }
    }
    std::cout << "Accuracy: " << 100 * correct_answers / answers << "%\n";
}

int main()
{
    test_filter(10000);
    return 0;
}