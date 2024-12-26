#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <stack>
#include <memory>
#include <sstream>
#include <unordered_map>

/*
 * Task 22
 *    Составить   программу  поиска  записи  с  включением  в
 *    сильно ветвящемся  Б-дереве  порядка N.  Б-дерево  хранится  и
 *    обрабатывается в оперативной памяти (12).
 * Name:
 *     Дмитрий Рыбаков, ПС-21
 * IDE:
 *     clion-2024.2.1
 * Compiler:
 *     gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
 */

// INPUT: номер: int фамилия: string


struct BTreeNode
{
    int grade;
    int minChildrenNumber = grade - 1;
    int maxChildrenNumber = 2 * grade - 1;
    std::vector<int> content;
    std::vector<std::shared_ptr<BTreeNode>> children;

    explicit BTreeNode(int _grade)
        : grade(_grade)
    {
    }
};

int main(int argc, char* args[])
{

};