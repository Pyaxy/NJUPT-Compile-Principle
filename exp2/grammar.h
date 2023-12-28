//
// Created by 潘鹏宇 on 2023/12/19.
//

#ifndef EXP2_GRAMMAR_H
#define EXP2_GRAMMAR_H

#include <vector>
#include <iostream>
#include <utility>
#include <unordered_map>
#include <cctype>
#include <set>
#include <sstream>
class Grammar {
public:
    int grammar_number = 0;     // 文法数量
    char firstSymbol;           // 开始符
    std::set<char> Terminal;    // 终结符集合
    std::set<char> noneTerminal;// 非终结符集合
    std::unordered_map<char, std::vector<std::string>> grammar; // 文法映射
    std::unordered_map<std::string, std::set<char>> firstSet;   // first集
    std::unordered_map<char, std::set<char>> followSet;         // follow集

    explicit Grammar(const std::vector<std::string>& grammarList);

    void firstSetInit();
    std::set<char> getOneSentenceFirstSet(const std::string& sentence);
    std::set<char> getNoneTerminalFirstSet(const char& noneterminal);

    void followSetInit();
    void getNoneTerminalFollowSet(const char& noneterminal);

    // 显示函数
    void grammarDisplay();
    void firstSetDisplay();

    void followSetDisplay();

    bool isTerminal(const char &symbol);

    static std::string getFirstFormat(const std::string &sentence, std::set<char> &firstset);

    static std::string getFollowFormat(char noneterminal, std::set<char> &followset);
};


#endif //EXP2_GRAMMAR_H
