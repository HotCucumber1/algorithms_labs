#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <stack>
#include <memory>
#include <sstream>
#include <unordered_map>


/*
 * Task 20
 *    Проект научно-технической программы задан с помощью
 *    ориентированного графа. Вершина графа соответствует отдельному
 *    исследованию, а дуги показывают очередность выполнения
 *    исследований (каждое исследование может начаться не ранее
 *    окончания предшествующих исследований). Продолжительность
 *    каждого исследования известна. Требуется:
 *     1) проверить граф на отсутствие циклов;
 *     2) используя топологическую сортировку, найти путь наибольшей
 *        трудоемкости.
 *    Путем считается последовательность работ, которые должны
 *    выполняться друг за другом.  Трудоемкость пути - суммарная
 *    продолжительность работ на этом пути (10).
 * Name:
 *     Дмитрий Рыбаков, ПС-21
 * IDE:
 *     clion-2024.2.1
 * Compiler:
 *     gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
 */

enum Color
{
    white,
    gray,
    black
};


struct Node
{
    int time;
    int orderNum;
    std::string content;
    Color color;
    std::vector<std::shared_ptr<Node>> next;

    Node(
        int _time,
        int _orderNum,
        std::string _content
    )
    : time(_time)
    , content(std::move(_content))
    , orderNum(_orderNum)
    , color(Color::white)
    {
    }
};

Node ParseNode(const std::string& nodeLine)
{
    std::string token;
    std::istringstream iss(nodeLine);
    std::vector<std::string> tokens;

    while (iss >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() != 3) {
        std::cout << "Неверный формат ввода данных" << std::endl;
        exit(1);
    }
    return {
        std::stoi(tokens[2]),
        std::stoi(tokens[0]),
        tokens[1]
    };
}

std::pair<int, int> ParseOrder(const std::string& orderString)
{
    size_t pos = orderString.find(' ');
    std::string orderNum;
    std::pair<int, int> order;
    if (pos == std::string::npos) {
        std::cout << "Неверный формат ввода данных" << std::endl;
        exit(1);
    }
    order.first = std::stoi(
        orderString.substr(0, pos)
    );
    order.second = std::stoi(
        orderString.substr(pos + 1, orderString.length() - pos - 1)
    );
    return order;
}

std::shared_ptr<Node> ReadFromFile(std::ifstream& inFile1, std::ifstream& inFile2)
{
    std::string line;
    std::unordered_map<int, std::shared_ptr<Node>> graphBase;

    while (std::getline(inFile1, line))
    {
        auto newNode = std::make_shared<Node>(
            ParseNode(line)
        );

        size_t pos = line.find(' ');
        std::string orderNum;
        if (pos != std::string::npos) {
            orderNum = line.substr(0, pos);
        }
        else {
            orderNum = line;
        }
        graphBase[std::stoi(orderNum)] = newNode;
    }

    while (std::getline(inFile2, line))
    {
        auto order = ParseOrder(line);
        graphBase[order.first]->next.push_back(
            graphBase[order.second]
        );
    }
    return graphBase[1];
}

void AllTopologicalSortsUtil(
    std::shared_ptr<Node>& node,
    std::vector<std::shared_ptr<Node>>& currentSort,
    std::vector<std::vector<std::shared_ptr<Node>>>& allPaths
)
{
    if (node->color == Color::white) {
        currentSort.push_back(node);

        for (auto& nextNode : node->next) {
            if (nextNode->color == Color::gray) {
                break;
            }
            node->color = Color::gray;
            AllTopologicalSortsUtil(nextNode, currentSort, allPaths);

            currentSort.pop_back();
            node->color = Color::white;
        }
    }
    if (node->next.empty()) {
        allPaths.push_back(currentSort);
    }
}


std::vector<std::vector<std::shared_ptr<Node>>> AllTopologicalSorts(std::shared_ptr<Node>& node) {
    std::vector<std::vector<std::shared_ptr<Node>>> allPaths;
    std::vector<std::shared_ptr<Node>> currentSort;

    AllTopologicalSortsUtil(node, currentSort, allPaths);

    std::cout << "Все возможные топологические сортировки:" << std::endl;
    for (const auto& path : allPaths) {
        for (auto node : path) {
            std::cout << node->orderNum << " ";
        }
        std::cout << std::endl;
    }

    return allPaths;
}

void DropToWhite(std::shared_ptr<Node>& graphNode)
{
    graphNode->color = Color::white;
    for (auto node : graphNode->next) {
        DropToWhite(node);
    }
}

bool TopologicSort(
    std::shared_ptr<Node>& graphNode,
    std::vector<std::shared_ptr<Node>> &nodeStack,
    std::vector<std::shared_ptr<Node>> &loopStack,
    std::shared_ptr<Node> &cycleStart
)
{
    graphNode->color = Color::gray;
    loopStack.push_back(graphNode);
    for (auto node : graphNode->next) {
        if (node->color == Color::black) {
            continue;
        }
        if (node->color == Color::gray) {
            cycleStart = node;
            return true;
        }
        if (TopologicSort(node, nodeStack, loopStack, cycleStart)) {
            return true;
        }
    }
    graphNode->color = Color::black;
    loopStack.pop_back();
    nodeStack.push_back(graphNode);

    DropToWhite(graphNode);
    return false;
}

void PrintMaxTimePath(std::vector<std::vector<std::shared_ptr<Node>>>& allPath)
{
    int maxTime = 0;
    for (auto path : allPath) {
        int currentPath = 0;
        for (auto node : path) {
            currentPath += node->time;
        }
        if (currentPath > maxTime) {
            maxTime = currentPath;
        }
    }
    std::cout << std::endl << "Максимальное время: " << maxTime << std::endl;
}

void PrintLoop(std::vector<std::shared_ptr<Node>> &loopStack, const std::shared_ptr<Node>& cycleStart) {
    if (cycleStart == nullptr) {
        return;
    }

    std::vector<std::shared_ptr<Node>> cycle;
    while (!loopStack.empty()) {
        auto node = loopStack.back();
        loopStack.pop_back();
        cycle.push_back(node);
        if (node->orderNum == cycleStart->orderNum) {
            break;
        }
    }

    std::cout << "Loop is found: ";
    while (!cycle.empty()) {
        auto node = cycle.back();
        cycle.pop_back();
        std::cout << node->content << ' ';
    }
    std::cout << std::endl;
}


int main(int argc, char* args[])
{
    setlocale(LC_ALL, "RU");
    if (argc != 3)
    {
        std::cout << "Передано неверное количество файлов" << std::endl;
        return 1;
    }
    std::ifstream inFile1(args[1]);
    if (!inFile1.is_open())
    {
        std::cout << "Ошибка открытия файла " << args[1] << std::endl;
        return 1;
    }
    std::ifstream inFile2(args[2]);
    if (!inFile2.is_open())
    {
        std::cout << "Ошибка открытия файла " << args[2] << std::endl;
        return 1;
    }

    std::vector<std::shared_ptr<Node>> nodeStack;
    std::vector<std::shared_ptr<Node>> loopStack;
    std::shared_ptr<Node> cycleStart = nullptr;

    auto graph = ReadFromFile(inFile1, inFile2);

    if (TopologicSort(graph, nodeStack, loopStack, cycleStart)) {
        PrintLoop(loopStack, cycleStart);
        exit(1);
    }
    auto allPaths = AllTopologicalSorts(graph);

    PrintMaxTimePath(allPaths);

    return 0;
}