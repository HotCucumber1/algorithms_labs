#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <stack>
#include <memory>
#include <algorithm>

/*
 * Task 22
 *    Составить программу поиска записи с включением в
 *    сильно ветвящемся Б-дереве порядка N. Б-дерево  хранится  и
 *    обрабатывается в оперативной памяти (12).
 * Name:
 *     Дмитрий Рыбаков, ПС-21
 * IDE:
 *     clion-2024.2.1
 * Compiler:
 *     gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
 */

struct FileData
{
    bool needToWrite;
    std::string fileName;

    FileData(bool needToWrite, std::string fileName)
        : needToWrite(needToWrite)
        , fileName(std::move(fileName))
    {
    }
};

struct Record
{
    int number;
    std::string name;

    Record(int num, std::string str)
        : number(num)
        , name(std::move(str))
    {
    }
};

struct BTreeNode
{
    int grade;
    int maxChildrenNumber;
    bool isLeaf;
    std::shared_ptr<BTreeNode> parent;
    std::vector<Record> content;
    std::vector<std::shared_ptr<BTreeNode>> children;

    BTreeNode(
        int _grade,
        std::shared_ptr<BTreeNode> _parent,
        bool isLeaf
        )
        : grade(_grade)
        , maxChildrenNumber(2 * grade - 1)
        , parent(std::move(_parent))
        , isLeaf(isLeaf)
    {
    }
};

bool Contains(const std::vector<Record>& vec, const Record& value) {
    for (auto& record : vec) {
        if (record.number == value.number) {
            return true;
        }
    }
    return false;
}


Record ParseLine(const std::string& line)
{
    size_t pos = line.find(' ');
    std::pair<int, std::string> node;
    if (pos == std::string::npos) {
        std::cout << "Неверный формат ввода данных" << std::endl;
        exit(1);
    }
    node.first = std::stoi(
        line.substr(0, pos)
    );
    node.second = line.substr(pos + 1, line.length() - pos - 1);
    return {
        std::stoi(line.substr(0, pos)),
        line.substr(pos + 1, line.length() - pos - 1)
    };
}


std::shared_ptr<BTreeNode> CreateNode(
    int grade,
    const std::shared_ptr<BTreeNode>& parent,
    bool isLeaf)
{
    return std::make_shared<BTreeNode>(grade, parent, isLeaf);
}


void SplitNode(
    const std::shared_ptr<BTreeNode>& parent,
    int index,
    const std::shared_ptr<BTreeNode>& child
    )
{
    int midIndex = child->content.size() / 2;
    auto newNode = CreateNode(child->grade, parent, child->isLeaf);

    newNode->content.assign(
        child->content.begin() + midIndex + 1,
        child->content.end()
        );
    child->content.erase(
        child->content.begin() + midIndex + 1,
        child->content.end()
        );

    if (!child->isLeaf) {
        newNode->children.assign(
            child->children.begin() + midIndex + 1,
            child->children.end()
            );
        child->children.erase(
            child->children.begin() + midIndex + 1,
            child->children.end()
            );
    }

    parent->content.insert(
        parent->content.begin() + index,
        child->content[midIndex]
    );
    child->content.erase(
        child->content.begin() + midIndex
    );

    parent->children.insert(parent->children.begin() + index + 1, newNode);
}


void InsertToNode(const std::shared_ptr<BTreeNode>& node, const Record& record) {
    int i = node->content.size() - 1;

    if (node->isLeaf) {
        while (i >= 0 && record.number < node->content[i].number) {
            i--;
        }
        if (!Contains(node->content, record)) {
            node->content.insert(node->content.begin() + i + 1, record);
        }
    }
    else {
        while (i >= 0 && record.number < node->content[i].number) {
            i--;
        }
        i++;
        auto child = node->children[i];

        if (child->content.size() == child->maxChildrenNumber) {
            SplitNode(node, i, child);
            if (record.number > node->content[i].number) {
                i++;
            }
        }
        InsertToNode(node->children[i], record);
    }
}

void Insert(
    std::shared_ptr<BTreeNode>& root,
    int grade,
    const Record& record,
    const FileData& fileData
)
{
    if (root == nullptr) {
        root = CreateNode(grade, nullptr, true);
        root->content.push_back(record);
        return;
    }

    if (root->content.size() == root->maxChildrenNumber) {
        auto newRoot = CreateNode(grade, nullptr, false);
        newRoot->children.push_back(root);
        root->parent = newRoot;

        SplitNode(newRoot, 0, root);
        root = newRoot;
    }

    InsertToNode(root, record);
    if (fileData.needToWrite) {

        std::ofstream file(fileData.fileName, std::ios::app);
        if (file.is_open()) {
            file << record.number << ' ' << record.name << std::endl;
        }
    }
}

void PrintTree(const std::shared_ptr<BTreeNode>& node, int level = 0) {
    if (node != nullptr) {
        std::cout << std::string(level * 2, ' ');
        for (auto& record : node->content) {
            std::cout << record.number << " ";
        }
        std::cout << std::endl;

        for (auto& child : node->children) {
            PrintTree(child, level + 1);
        }
    }
}


std::shared_ptr<BTreeNode> BuildTreeFromFile(std::ifstream& inFile, std::string fileName, const int& order)
{
    std::string line;
    std::shared_ptr<BTreeNode> root = nullptr;

    while (std::getline(inFile, line))
    {
        auto nodeContent = ParseLine(line);
        Insert(
            root,
            order,
            nodeContent,
            {false, fileName}
            );
    }
    return root;
}

Record* FindRecord(std::shared_ptr<BTreeNode> root, const int& requiredRecord)
{
    for (int i = 0; i < root->content.size(); i++) {
        if (root->content[i].number < requiredRecord) {
            continue;
        }
        if (root->content[i].number == requiredRecord) {
            return &root->content[i];
        }
        else {
            if (!root->isLeaf) {
                return FindRecord(root->children[i], requiredRecord);
            }
        }
    }
    if (!root->isLeaf) {
        return FindRecord(root->children.back(), requiredRecord);
    }
    return nullptr;
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

    std::string orderStr;
    std::getline(inFile, orderStr);
    int order = std::stoi(orderStr);

    auto root = BuildTreeFromFile(inFile, args[1], order);

    std::string command;

    while (true) {
        std::cout << std::endl << "Выберите действие: найти | вставить | вывести | конец" << std::endl;
        int number;
        std::string surname;

        std::cin >> command;

        if (command == "найти") {
            std::cout <<  "Введите число: ";
            std::cin >> number;
            auto record = FindRecord(root, number);

            if (record == nullptr) {
                Insert(
                    root,
                    order,
                    {number, "Новая фамилия"},
                    {true, args[1]}
                );
                std::cout << "Значение " << number << " не было найдено и успешно добавлено в дерево" << std::endl;
            }
            else {
                std::cout << "Запись найдена: " << record->number << ' ' << record->name << std::endl;
            }
        }

        if (command == "вставить") {
            std::cout <<  "Введите число: ";
            std::cin >> number;
            std::cout <<  "Введите фамилию: ";
            std::cin >> surname;
            Insert(
                root,
                order,
                {number, surname},
                {true, args[1]}
                );
            std::cout << "Значение " << number << " успешно добавлено в дерево" << std::endl;
        }

        if (command == "вывести") {
            PrintTree(root);
        }
        if (command == "конец") {
            break;
        }
    }
};