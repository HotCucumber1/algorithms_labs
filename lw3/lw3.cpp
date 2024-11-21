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
    std::string name;
    char type;
    std::vector<std::shared_ptr<Node>> children;
    const int childrenCount = children.size();

    Node(std::string name, char type)
        : name(std::move(name))
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


struct Stack
{
    Node* key{};
    Stack* next{};
};


struct FileNode
{
    std::string name;
    NodeType type{};
    int level = 0;
};

void push(Stack *& ptr, Node* nodePtr);
Node* pop(Stack *& ptr);


FileNode parseStringToFileNode(const std::string& nodeElement)
{
    FileNode node;
    std::vector<std::string> stripedStr;
    std::string tempStr;

    for (auto ch : nodeElement)
    {
        if (ch == '.')
        {
            node.level += 1;
        }
        else if (ch != ' ')
        {
            tempStr += ch;
        }
        else {
            stripedStr.push_back(tempStr);
            tempStr = "";
        }
    }
    if (!tempStr.empty())
    {
        stripedStr.push_back(tempStr);
    }

    node.type = (stripedStr.size() > 1 ?
                (stripedStr[1] == "a" ? _and_ : _or_) :
                _leaf_);

    node.name = stripedStr[0];
    return node;
}


void addToTree(FileNode el, Node* nodePtr);
//{
//    auto newElement = strip(el);
//
//    auto* newNode = new Node;
//    newNode->name = el;
//    newNode->type = (newElement[1] == "a" ? _and_ : _or_);
//    newNode->children = {};
//
//    if (nodePtr == nullptr)
//    {
//        nodePtr = newNode;
//    }
//    else
//    {
//
//    }
//}


void readFromFile(const std::string& fileName, Node* rootPtr)
{
    std::ifstream inFile(fileName);
    if (!inFile.is_open())
    {
        std::cout << "Ошибка открытия файла " << fileName << std::endl;
        exit(1);
    }
    std::string line;
    while (getline(inFile, line))
    {
        auto newNode = parseStringToFileNode(line);
        addToTree(newNode, rootPtr);
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


    return 0;
}



//void push(Stack *& ptr, Node* nodePtr)
//{
//    auto* element = new Stack;
//    element->key = nodePtr;
//    element->next = ptr;
//    ptr = element;
//}
//
//Node* pop(Stack *& ptr)
//{
//    Stack * element = ptr;
//    ptr = ptr->next;
//    Node* nodePtr = element->key;
//    delete element;
//    return nodePtr;
//}