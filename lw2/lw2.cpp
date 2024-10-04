#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

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

float countPolish(std::vector<std::string> polish, bool negative);

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
    bool negative = false;

    std::string expression;
    getline(inFile, expression);

    const int spaceCount = (int) expression.length();
    std::string resultString = " Result";

    std::cout << "Element" << '\t' << "|" << resultString;
    int spaces = spaceCount - resultString.length();
    if (spaces > 0)
    {
        for (int i = 0; i < spaces; i++)
        {
            std::cout << ' ';
        }
    }

    resultString = "";
    std::cout << "| Stack" << std::endl;

    for (auto element : parseInput(expression))
    {
        // Правило 0
        if (element == "~")
        {
            negative = true;
        }
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
        spaces = spaceCount - resultString.length();
        if (spaces > 0)
        {
            for (int i = 0; i < spaces; i++)
            {
                std::cout << ' ';
            }
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

    // Счет
    std::cout << std::endl << "Result: " << countPolish(polish, negative) << std::endl;
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


int getOperationPriority(std::string & operation)
{
    if (operation == "EXP") return 4;
    if (operation == "SIN" || operation == "COS") return 3;
    if (operation == "^") return 2;
    if (operation == "*" || operation == "/") return 1;
    if (operation == "+" || operation == "-") return 0;
    return -1;
}

std::vector<std::string> parseInput(std::string & expression)
{
    bool canBeNegative = true;
    std::vector<std::string> parsedExpression;
    std::string number;
    std::string functionExpression;
    char ch;

    for (int i = 0; i < expression.length(); i++)
    {
        ch = expression[i];
        if (ch == '(')
        {
            if (canBeNegative)
            {
                parsedExpression.emplace_back(1, '~');
                number = "";
            }
            canBeNegative = true;
            parsedExpression.emplace_back(1, ch);
        }
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
            if (i == 0)
            {
                canBeNegative = true;
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

float countPolish(std::vector<std::string> polish, bool negative)
{
    Stack * top = nullptr;
    for (auto el : polish)
    {
        if (isNumeric(el))
        {
            push(top, el);
        }
        else
        {
            float result;
            if (el == "SIN" || el == "COS" || el == "EXP")
            {
                std::string el1 = pop(top);
                float elFloat = stof(el1);

                if (el == "SIN")
                {
                    result = std::sin(elFloat);
                }
                else if (el == "COS")
                {
                    result = std::cos(elFloat);
                }
                else if (el == "EXP")
                {
                    result = std::exp(elFloat);
                }
            }
            if (el == "+" || el == "-" || el == "/" || el == "*" || el == "^")
            {
                std::string el1 = pop(top);
                std::string el2 = pop(top);
                float el1Float = stof(el1);
                float el2Float = stof(el2);
                if (el == "+")
                {
                    result = el2Float + el1Float;
                }
                else if (el == "-")
                {
                    result = el2Float - el1Float;
                }
                else if (el == "*")
                {
                    result = el2Float * el1Float;
                }
                else if (el == "/")
                {
                    result = el2Float / el1Float;
                }
                else if (el == "^")
                {
                    result = std::pow(el2Float, el1Float);
                }
            }
            std::string stringRes = std::to_string(result);
            push(top, stringRes);
        }
    }
    std::string res = pop(top);
    if (negative)
        return -std::stof(res);
    return std::stof(res);
}