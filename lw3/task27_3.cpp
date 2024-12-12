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
    std::vector<std::shared_ptr<Node>> children;
    std::string content;
    int childrenCount = 0;
    int index = 0;
    char type;
    std::shared_ptr<Node> parent;

    Node(
        std::string content,
        char type
    )
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
    std::shared_ptr<Node> last = root;

    while (std::getline(treeFile, line))
    {
        int nodeLevel = countLevel(line);
        std::string lineContent = line.substr(nodeLevel);
        size_t spacePos = lineContent.find(' ');

        char nodeType = (spacePos != std::string::npos)
                ? lineContent.substr(spacePos + 1)[0]
                : ' ';
        std::string content = (spacePos != std::string::npos)
                ? lineContent.substr(0, spacePos)
                : lineContent;

        auto newNode = std::make_shared<Node>(
            content,
            nodeType
        );
        if (nodeStack.empty())
        {
            newNode->parent = last;
            root = newNode;
        }
        else
        {
            while (!nodeStack.empty() && nodeStack.back().second >= nodeLevel)
            {
                last = nodeStack.back().first->parent;
                nodeStack.pop_back();
            }
            if (!nodeStack.empty())
            {
                newNode->parent = last;
                nodeStack.back().first->children.push_back(newNode);
            }
        }
        nodeStack.emplace_back(newNode, nodeLevel);
        last = newNode;
    }

    return root;
}

void printTree(std::shared_ptr<Node>& node, int depth=0)
{
    std::cout << std::string(depth, '.')
              << node->content
              << ' '
              << node->type
              << "\n";
    if (node->type == 'a')
    {
        for (auto& child : node->children)
        {
            printTree(child, depth + 1);
        }
    }
    else if (node->type == 'o')
    {
        printTree(node->children[node->index], depth + 1);
    }
}

void dropTree(std::shared_ptr<Node>& node)
{
    node->index = 0;
    for (auto& child : node->children)
    {
        dropTree(child);
    }
}

bool printSubTrees(std::shared_ptr<Node>& node, bool& isChecked)
{
    if (isChecked)
    {
        return isChecked;
    }
    if (node->type == 'a')
    {
        for (int i = 0; i < node->children.size(); i++)
        {
            node->index = i;
            printSubTrees(node->children[i], isChecked);
        }
    }
    else if (node->type == 'o')
    {
        printSubTrees(node->children[node->index], isChecked);
    }
    if (!isChecked && node->type == 'o' && node->index < node->children.size() - 1)
    {
        node->index += 1;
        isChecked = true;
        std::shared_ptr<Node> droppedNode = node;
        while (droppedNode->parent != nullptr)
        {
            droppedNode = droppedNode->parent;
            for (int i = droppedNode->index - 1; i >= 0; i--)
            {
                dropTree(droppedNode->children[i]);
            }
        }
    }
    return isChecked;
}

void countSubTrees(const std::shared_ptr<Node>& node) {
    if (node->children.empty())
    {
        node->childrenCount = 1;
        return;
    }

    for (auto& child : node->children)
    {
        countSubTrees(child);
        if (node->type == 'a')
        {
            if (node->childrenCount == 0)
            {
                node->childrenCount = 1;
            }
            node->childrenCount *= child->childrenCount;
        }
        else if (node->type == 'o')
        {
            node->childrenCount += child->childrenCount;
        }
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
    bool isChecked = false;
    countSubTrees(tree);
    std::cout << tree->childrenCount << "\n\n";

    int i = 1;
    do
    {
        std::cout << i << ')' << "\n";
        printTree(tree);
        std::cout << "\n";
        isChecked = false;
        i++;
    } while (printSubTrees(tree, isChecked));
    return 0;
}


