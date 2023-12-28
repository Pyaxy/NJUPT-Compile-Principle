#include "StringAnalysis.h"
#include <iostream>
#include <fstream>

// 读文件
void readFile(std::vector<std::string>& grammarList, const std::string& path)
{
	std::ifstream file(path);
	std::string line;
    std::cout << "reading "<< path << std::endl;
	if(file.is_open())
	{
        std::cout << "successfully read file!\n";
		while (std::getline(file, line))
		{
			grammarList.push_back(line);
		}
	}
}

// 打印内容
void printGrammar(std::vector<std::string>& grammarList)
{
    std::cout << "「--------grammarList--------" << std::endl;
	for(const auto& oneGrammar:grammarList)
	{
		std::cout << oneGrammar << std::endl;
	}
    std::cout << "--------grammarList--------」" << std::endl;
}
