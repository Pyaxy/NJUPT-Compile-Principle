//
// Created by 潘鹏宇 on 2023/12/22.
//

#include "LL1Parser.h"

#include <iomanip> // 用于格式化输出
#include <utility>
#include <sstream>
//构造函数
LL1Parser::LL1Parser(Grammar grammar) : grammar(std::move(grammar)) {
    buildParserTable();
}
// 构造LL(1)分析表
void LL1Parser::buildParserTable() {
    // 对于每个产生式
    for(const auto& prod: this->grammar.grammar) {
        char noneTerminal = prod.first;
        for(const auto& sentence : prod.second) {
            // 获取产生式右侧的 First 集
            std::set<char> firstSet = this->grammar.firstSet[sentence];
            for(const auto& terminal : firstSet) {
                // 如果不是空字符，直接添加到分析表中
                if(terminal != '@')
                    this->parserTable[noneTerminal][terminal] = sentence;
                else { // 如果包含空字符，那么对于 Follow(A) 中的每个终结符 b
                    for (const auto& followSymbol : this->grammar.followSet[noneTerminal]) {
                        // 将产生式 A -> α 添加到 M[A, b] 中
                        this->parserTable[noneTerminal][followSymbol] = sentence;
                    }
                }
            }
        }
    }
}
// 格式化输出LL(1)分析表
void LL1Parser::displayParserTable() {
    const int cell_width = 10;

    // 打印表头
    std::cout << std::left << std::setw(cell_width) << " ";
    for (const char& terminal : grammar.Terminal) {
        std::cout << std::setw(cell_width) << terminal;
    }
    std::cout << std::setw(cell_width) << std::endl; // 表示输入的结束

    // 打印分析表的行
    for (const char& nonTerminal : grammar.noneTerminal) {
        std::cout << std::setw(cell_width) << nonTerminal;
        for (const char& terminal : grammar.Terminal) {
            auto it = parserTable[nonTerminal].find(terminal);
            if (it != parserTable[nonTerminal].end()) {
                // 创建文法规则字符串
                std::stringstream rule;
                rule << nonTerminal << "::=" << it->second;
                std::cout << std::setw(cell_width) << rule.str(); // 打印文法规则
            } else {
                std::cout << std::setw(cell_width) << " "; // 空位
            }
        }

        std::cout << std::endl;
    }
}

// 打印分析过程的表头
void printTitle(int cell_width) {
    std::cout << std::left << std::setw(' ');
    std::cout << std::setw(cell_width) << "analysisStack";
    std::cout << std::right << std::setw(' ');
    std::cout << std::setw(cell_width) << "residualString";
    std::cout << std::right << std::setw(' ');
    std::cout << std::setw(cell_width) << "grammar";

    std::cout << std::endl;
}

// 使用LL(1)分析表分析字符串
bool LL1Parser::analyzeString(const std::string& input) {
    const int cell_width = 15;  // 格式化输出宽度
    std::string remainString = input + '#'; // 字符串底加入#
    std::stack<char> parseStack;    // 分析栈
    size_t pointer = 0;         // 余留栈指针
    parseStack.push('#');   //分析栈先入#
    parseStack.push(this->grammar.firstSymbol); // 分析栈入开始符
    // 打印表头
    printTitle(cell_width);
    // 栈不为空时
    while(!parseStack.empty()) {
        char top = parseStack.top();
        char currentInput = remainString[pointer];

        if(this->grammar.isTerminal(top)) {
            printRow(parseStack, remainString.substr(pointer), "", cell_width);
            // 当栈顶符号与输入符号匹配时
            if(top == currentInput) {
                parseStack.pop();
                pointer++;
            } else
                // shb
                return false;
        } else {
            // 栈顶是非终结符，查找预测分析表
            if(!this->parserTable[top][currentInput].empty()) {
                std::string prod = this->parserTable[top][currentInput];
                printRow(parseStack, remainString.substr(pointer), prod, cell_width);

                parseStack.pop();
                if (prod != "@") {
                    // 将产生式的符号逆序压栈
                    for (auto it = prod.rbegin(); it != prod.rend(); ++it) {
                        parseStack.push(std::string(1, *it)[0]);
                    }
                }
            } else {
                printRow(parseStack, remainString.substr(pointer), "", cell_width);
                // 预测分析表中没有匹配项，输入串不是文法的句子
                return false;
            }
        }
    }
    // 如果输入完全匹配且栈被清空，则是文法的句子
    return pointer == remainString.length();
}

// 逆序将栈的内容存到字符串中
std::string getReverseStack(std::stack<char> stack1) {
    std::stack<char> tmp;
    std::string res;
    while (!stack1.empty()) {
        tmp.push(stack1.top());
        stack1.pop();
    }
    while (!tmp.empty()) {
        res += tmp.top();
        tmp.pop();
    }
    return res;
}

// 打印分析过程的每一行
void LL1Parser::printRow(std::stack<char> stack1,
                         const std::basic_string<char, std::char_traits<char>, std::allocator<char>>& basicString,
                         const std::string& basicString1,
                         int cell_width) {
    std::cout << std::left << std::setfill(' ');
    std::cout << std::setw(cell_width) << getReverseStack(stack1);
    std::cout << std::right << std::setfill(' ');
    std::cout << std::setw(cell_width) << basicString;

    if(!basicString1.empty()) {
        std::cout << std::right << std::setfill(' ');
        std::stringstream rule;
        rule  << stack1.top() << "::=" << basicString1;
        std::cout << std::setw(cell_width) << rule.str();
    }
    std::cout << std::endl;

}