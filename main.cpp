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
        {
            // delete testing
            int el = element(gen);
            filter.Delete(std::to_string(el));
            set.erase(el);
            break;
        }
        }
    }
    std::cout << "Accuracy: " << 100 * correct_answers / answers << "%\n";
}

void self_check()
{
    CuckooFilter filter;
    while (true)
    {
        int operation = 0;
        std::cout << "Choose operation:\n1.insert element\n2.check existance\n3.delete element\n4.clear filter\n5.exit program\n";
        std::cin >> operation;
        if (std::cin.fail() || operation > 5 || operation < 1)
        {
            std::cout << "wrong input\n";
            return;
        }
        std::string element;
        switch (operation)
        {
        case 1:
        {
            std::cout << "enter element:\n";
            std::cin >> element;
            filter.Insert(element);
            break;
        }
        case 2:
        {
            std::cout << "enter element:\n";
            std::cin >> element;
            bool check = filter.Check(element);
            std::cout << check << '\n';
            break;
        }
        case 3:
        {
            std::cout << "enter element:\n";
            std::cin >> element;
            filter.Delete(element);
            break;
        }
        case 4:
        {
            filter.Clear();
            break;
        }
        default:
            return;
            break;
        }
    }
}

int main()
{
    test_filter(10000);
    self_check();
    return 0;
}