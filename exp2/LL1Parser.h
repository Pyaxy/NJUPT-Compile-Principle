//
// Created by 潘鹏宇 on 2023/12/22.
//

#ifndef EXP2_LL1PARSER_H
#define EXP2_LL1PARSER_H

#include "grammar.h"
#include <stack>

class LL1Parser {
private:
    Grammar grammar;
    // 存储的LL(1)分析表
    std::unordered_map<char, std::unordered_map<char, std::string>> parserTable;

public:
    explicit LL1Parser(Grammar grammar);

    void buildParserTable();

    void displayParserTable();

    bool analyzeString(const std::string& input);

    static void
    printRow(std::stack<char> stack1,
             const std::basic_string<char, std::char_traits<char>, std::allocator<char>> &basicString,
             const std::string &basicString1, int i);
};


#endif //EXP2_LL1PARSER_H
