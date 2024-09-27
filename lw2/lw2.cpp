#include <iostream>
#include <fstream>
#include <vector>

/*
 * Task 23.
 *     В строке текстового файла задано выражение из целых
 *     чисел и операций '+', '-', '*', '/', '^', SIN, COS, EXP. Порядок
 *     вычислений определяется приоритетом операций и круглыми
 *     скобками. Возможен одноместный минус в начале выражения или
 *     после открывающей скобки. Преобразовать выражение в постфиксную
 *     форму (алгоритм Дейкстры) и вычислить его значение.  Показать
 *     этапы выполнения (11).
 * Name:
 *     Дмитрий Рыбаков, ПС-21
 * IDE:
 *     clion-2024.2.1
 * Compiler:
 *     gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
 */


struct Stack
{
    std::string key;
    Stack *next{};
};

void push(Stack *& ptr, std::string & value);
std::string pop(Stack *& ptr);
std::string getTop(Stack *& ptr);
void printStack(Stack *& ptr);

int getIntFromString(std::string & number);

bool isNumeric(std::string & number);
int getOperationPriority(std::string & operation);

std::vector<std::string> parseInput(std::string & expression);

int main(int argc, char * args[])
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

    Stack * top = nullptr;
    std::vector<std::string> polish;

    std::string expression;
    getline(inFile, expression);

    const int spaceCount = (int) expression.length(); // 2 пробела по бокам
    std::string resultString = " Result";

    std::cout << "Element" << '\t' << "|" << resultString;
    for (int i = 0; i < spaceCount - resultString.length(); i++)
    {
        std::cout << ' ';
    }
    resultString = "";
    std::cout << "| Stack" << std::endl;

    for (auto element : parseInput(expression))
    {
        // Правило 1
        if (isNumeric(element))
        {
            polish.push_back(element);
        }
        // Правило 2
        else if (element != "(" && element != ")")
        {
            // Правило 2-1
            if (top == nullptr || top->key == "(")
            {
                push(top, element);
            }
            // Правило 2-2
            else if (std::string stackTop = getTop(top);
                    getOperationPriority(element) > getOperationPriority(stackTop))
            {
                push(top, element);
            }
            // Правило 2-3
            else if (element == "+" || element == "-" || element == "*" || element == "/")
            {
                std::string topOperation = getTop(top);
                while ((top != nullptr || topOperation != "(") &&
                        getOperationPriority(topOperation) >= getOperationPriority(element))
                {
                    polish.push_back(pop(top));
                    if (top == nullptr) topOperation = "(";
                    else topOperation = getTop(top);
                }
                push(top, element);
            }
            // Правило 2-4
            else if (element == "^" || element == "SIN" || element == "COS" || element == "EXP")
            {
                std::string topOperation = getTop(top);
                while (getOperationPriority(topOperation) > getOperationPriority(element) &&
                       (top != nullptr || topOperation != "("))
                {
                    polish.push_back(pop(top));
                    topOperation = getTop(top);
                }
                push(top, element);
            }
        }
        // Правило 3
        else if (element == "(")
        {
            push(top, element);
        }
        // Правило 4
        else if (element == ")")
        {
            while (getTop(top) != "(")
            {
                polish.push_back(pop(top));
            }
            pop(top);
        }
        // Печать
        std::cout << element << '\t' << "|";
        for (auto el : polish)
        {
            resultString += ' ' + el;
        }
        std::cout << resultString;
        for (int i = 0; i < spaceCount - resultString.length(); i++)
        {
            std::cout << ' ';
        }
        std::cout << "| ";
        resultString = "";

        printStack(top);
    }
    // Конец
    while (top != nullptr)
    {
        polish.push_back(pop(top));
    }

    std::cout << "End:" << std::endl;
    for (auto el : polish)
    {
        std::cout << el << ' ';
    }
    std::cout << std::endl;


}





void push(Stack *& ptr, std::string & value)
{
    auto * element = new Stack;
    element->key = value;
    element->next = ptr;
    ptr = element;
}

std::string pop(Stack *& ptr)
{
    Stack * element = ptr;
    ptr = ptr->next;
    std::string value = element->key;
    delete element;
    return value;
}

std::string getTop(Stack *& ptr)
{
    return ptr->key;
}

bool isNumeric(std::string & number)
{
    if (number == "-")
    {
        return false;
    }
    for (auto el : number)
    {
        if (!isdigit(el) && el != '-')
        {
            return false;
        }
    }
    return true;
}

int getIntFromString(std::string & number)
{
    int intNumber = 0;
    int start = 0;
    bool isNegative = false;

    if (number[start] == '-')
    {
        isNegative = true;
        start = 1;
    }
    for (int i = start; i < number.length(); i++)
    {
        intNumber = 10 * intNumber + (number[i] - '0');
    }
    return isNegative ? -intNumber : intNumber;
}

int getOperationPriority(std::string & operation)
{
    if (operation == "EXP") return 3;
    if (operation == "SIN" || operation == "COS") return 2;
    if (operation == "*" || operation == "/") return 1;
    if (operation == "+" || operation == "-") return 0;
    return -1;
}

std::vector<std::string> parseInput(std::string & expression)
{
    bool canBeNegative = false;
    std::vector<std::string> parsedExpression;
    std::string number;
    std::string functionExpression;
    char ch;

    for (int i = 0; i < expression.length(); i++)
    {
        ch = expression[i];
        if (isdigit(ch))
        {
            number += ch;
            canBeNegative = false;
        }
        else if (!number.empty() && ch != ' ')
        {
            parsedExpression.push_back(number);
            number = "";
            canBeNegative = false;
        }
        if (ch == 'S' || ch == 'I' || ch == 'N' ||
            ch == 'C' || ch == 'O' ||
            ch == 'E' || ch == 'X' || ch == 'P')
        {
            canBeNegative = false;
            functionExpression += ch;
            if (functionExpression == "SIN" || functionExpression == "COS" || functionExpression == "EXP")
            {
                parsedExpression.push_back(functionExpression);
                functionExpression = "";
            }
        }
        if (ch == '(')
        {
            canBeNegative = true;
            parsedExpression.emplace_back(1, ch);
        }
        if (ch == '-')
        {
            if (canBeNegative)
            {
                number += ch;
                canBeNegative = false;
            }
            else
            {
                parsedExpression.emplace_back(1, ch);
            }
        }
        if (ch == '+' || ch == '*' || ch == '/' ||
            ch == '^' || ch == ')')
        {
            canBeNegative = false;
            parsedExpression.emplace_back(1, ch);
        }
    }
    if (!number.empty())
    {
        parsedExpression.push_back(number);
    }
    return parsedExpression;
}

void printStack(Stack *& ptr)
{
    Stack * current = ptr;
    while (current != nullptr)
    {
        std::cout << current->key << ' ';
        current = current->next;
    }
    std::cout << std::endl;
}