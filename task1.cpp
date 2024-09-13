#include <iostream>
#include <fstream>
#include <vector>

/*
 * Task 27:
 *     Задан текстовый файл.
 *     Каждая строка содержит не более 255 символов.
 *     Создать новый файл, в котором строки будут следовать в обратном порядке. Размер файла  не ограничивается.
 *     Запрещается размещать файл целиком в основной памяти.
 *     Файлы размером порядка 10 Мгб должны обрабатываться не более 2 сек.
 * Name:
 *     Дмитрий Рыбаков, ПС-21
 * IDE:
 *     clion-2024.2.1
 * Compiler:
 *     gcc version 11.4.0 (Ubuntu 11.4.0-1ubuntu1~22.04)
 */

using namespace std;

int main(int argc, char * args[])
{
    setlocale(LC_ALL, "RU");
    if (argc != 3)  // + сам файл
    {
        cout << "Передано неверное количество файлов" << endl;
        return 1;
    }

    ifstream inFile(args[1]);
    ofstream outFile(args[2]);

    if (!inFile.is_open())
    {
        cout << "Ошибка открытия файла " << args[1] << endl;
        return 1;
    }
    if (!outFile.is_open())
    {
        cout << "Ошибка открытия файла " << args[2] << endl;
        return 1;
    }

    string line;
    vector<streampos> lineStarts;
    int counter = 0;

    lineStarts.push_back(inFile.tellg());
    while (getline(inFile, line))
    {
        lineStarts.push_back(inFile.tellg());
        counter++;
    }

    for (int i = counter - 1; i >= 0; i--)
    {
        inFile.clear();
        inFile.seekg(lineStarts[i]);
        getline(inFile, line);
        outFile << line << endl;
    }

    inFile.close();
    outFile.close();

    return 0;
}
