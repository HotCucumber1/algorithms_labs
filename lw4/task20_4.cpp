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
    bool isRoot = true;
    std::shared_ptr<Node> root;

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

        if (isRoot) {
            root = graphBase[order.first];
            isRoot = false;
        }
    }
    return root;
}

bool TopologicSort(
    std::shared_ptr<Node>& graphNode,
    std::vector<std::shared_ptr<Node>> &nodeStack,
    std::vector<std::shared_ptr<Node>> &loopStack,
    std::shared_ptr<Node> &cycleStart,
    int& currentTime,
    int& maxTime
)
{
    currentTime += graphNode->time;
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
        if (TopologicSort(node, nodeStack, loopStack, cycleStart, currentTime, maxTime)) {
            return true;
        }
        currentTime -= node->time;
    }
    if (currentTime > maxTime) {
        maxTime = currentTime;
    }
    graphNode->color = Color::black;
    loopStack.pop_back();
    nodeStack.push_back(graphNode);

    return false;
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

    std::cout << "READ GRAPH ..." << std::endl;
    auto graph = ReadFromFile(inFile1, inFile2);

    std::cout << "FIND PATH ..." << std::endl;

    int currentTime = 0;
    int maxTime = 0;

    if (TopologicSort(graph, nodeStack, loopStack, cycleStart, currentTime, maxTime)) {
        PrintLoop(loopStack, cycleStart);
        exit(1);
    }

    while (!nodeStack.empty()) {
        std::cout << nodeStack.back()->content;
        nodeStack.pop_back();
        if (!nodeStack.empty()) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl << std::endl;
    std::cout << "Max path: " << maxTime << std::endl;
    return 0;
}