#include <iostream>
#include <fstream>
#include <vector>


int main(int argc, char * args[])
{
    if (argc != 3)  // + сам файл
    {
        std::cout << "Передано неверное количество файлов" << std::endl;
        return 1;
    }

    std::ifstream inFile(args[1], std::ios::binary);
    std::ofstream outFile(args[2]);

    if (!inFile.is_open())
    {
        std::cout << "Ошибка открытия файла " << args[1] << std::endl;
        return 1;
    }
    if (!outFile.is_open())
    {
        std::cout << "Ошибка открытия файла " << args[2] << std::endl;
        return 1;
    }

    std::string line;
    std::vector<std::streampos> lineStarts;
    int counter = 0;
   
    lineStarts.push_back(inFile.tellg());
    while (std::getline(inFile, line))
    {
        lineStarts.push_back(inFile.tellg());
        counter++;
    }

    std::cout << inFile.tellg();

    for (int i = counter - 1; i >= 0; i--)
    {
        inFile.seekg(lineStarts[i]);

        std::cout << (int)inFile.tellg() << " " << lineStarts[i] << std::endl;

        std::getline(inFile, line);
        outFile << line << std::endl;
    }

    inFile.close();
    outFile.close();

    return 0;
}
