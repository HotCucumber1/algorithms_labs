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

void TopologicSort(std::shared_ptr<Node>& graphNode, std::vector<std::shared_ptr<Node>> &nodeStack)
{
    graphNode->color = Color::gray;
    for (auto node : graphNode->next) {
        if (node->color == Color::black) {
            continue;
        }
        if (node->color == Color::gray) {
            std::cout << "Loop is found" << std::endl;
            exit(1);
        }
        TopologicSort(node, nodeStack);
    }
    graphNode->color = Color::black;
    nodeStack.push_back(graphNode);
}

void GetMaxTimePath(std::vector<std::shared_ptr<Node>> &nodeStack)
{
    int maxTime = 0;
    while (!nodeStack.empty())
    {
        auto node = nodeStack.back();
        nodeStack.pop_back();
        std::cout
            << node->orderNum << ' '
            << node->content << ' '
            << node->time
            << std::endl;
        maxTime += node->time;
    }
    std::cout << std::endl << "Максимальное время: " << maxTime << std::endl;
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

    auto graph = ReadFromFile(inFile1, inFile2);
    TopologicSort(graph, nodeStack);
    GetMaxTimePath(nodeStack);

    return 0;
}