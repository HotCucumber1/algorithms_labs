#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <stack>
#include <memory>

/*
 * Task 27
 *     Имеется  И-ИЛИ   дерево,   соответствующее   некоторому
 *     множеству  конструкций.  Требуется выдать на экран в наглядном
 *     виде все элементы дерева (13).
 * Name:
 *     Дмитрий Рыбаков, ПС-21
 * IDE:
 *     clion-2024.2.1
 * Compiler:
 *     gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
 */


struct Node
{
    std::string content;
    char type;
    std::vector<std::shared_ptr<Node>> children;
    const int childrenCount = children.size();

    Node(std::string content, char type)
        : content(std::move(content))
        , type(type)
    {
    }
};

int countLevel(const std::string& line) {
    int count = 0;
    while (count < line.size() && line[count] == '.') {
        count++;
    }
    return count;
}

std::shared_ptr<Node> buildTreeFromFile(std::ifstream& treeFile)
{
    std::string line;
    std::shared_ptr<Node> root = nullptr;
    std::vector<std::pair<std::shared_ptr<Node>, int>> nodeStack;

    while (std::getline(treeFile, line))
    {
        int nodeLevel = countLevel(line);
        std::string lineContent = line.substr(nodeLevel);

        char nodeType = (line.size() > lineContent.size() + nodeLevel + 1)
                ? line[lineContent.size() + nodeLevel + 1]
                : ' ';

        auto newNode = std::make_shared<Node>(lineContent, nodeType);
        if (nodeStack.empty())
        {
            root = newNode;
        }
        else
        {
            // подняться к верхнему узлу
            while (!nodeStack.empty() && nodeStack.back().second >= nodeLevel)
            {
                nodeStack.pop_back();
            }
            if (!nodeStack.empty())
            {
                nodeStack.back().first->children.push_back(newNode);
            }
        }
        nodeStack.emplace_back(newNode, nodeLevel);
    }

    return root;
}

void printTree(const std::shared_ptr<Node>& node, int depth=0) {
    if (!node)
    {
        return;
    }

    std::cout << std::string(depth, '.')
              << node->content
              << ' '
              << node->type
              << "\n";
    for (auto& child : node->children)
    {
        printTree(child, depth + 1);
    }
}


int main(int argc, char* args[])
{
    setlocale(LC_ALL, "RU");
    if (argc != 2)
    {
        std::cout << "Передано неверное количество файлов" << std::endl;
        return 1;
    }
    std::ifstream inFile(args[1]);
    if (!inFile.is_open())
    {
        std::cout << "Ошибка открытия файла " << args[1] << std::endl;
        return 1;
    }
    auto tree = buildTreeFromFile(inFile);
    printTree(tree);
    return 0;
}


