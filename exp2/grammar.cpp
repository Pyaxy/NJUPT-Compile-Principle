//
// Created by 潘鹏宇 on 2023/12/19.
//

#include "grammar.h"

#include <utility>
#include <iomanip>

char getFirstChar(const std::string &sentence) {
    return sentence[0];
}

// 构造函数
Grammar::Grammar(const std::vector<std::string>& grammarList) {
    // 获得文法数量
    this->grammar_number = int(grammarList.size());
    this->firstSymbol = grammarList[0][0];
    this->Terminal.insert('#');
    // 遍历每一条文法
    for(const auto& oneGrammar:grammarList)
    {
        bool isLeft = true; // 是否是左部
        char leftPart;
        std::string temp;   // 用于保存句型

        // 遍历每一条文法
        for(int i = 0; i < oneGrammar.length(); i++)
        {
            char character = oneGrammar[i];

            // 如果是字母
            if(isalpha(character))
            {
                // 判断大写
                if(isupper(character))
                    // 加入非终结符
                    this->noneTerminal.insert(character);
                else if(islower(character))
                    // 加入终结符
                    this->Terminal.insert(character);
                // 如果是左部
                if(isLeft)
                {
                    leftPart = character;
                    isLeft = false;
                }
                else
                    // 生成句型
                    temp += character;
            }
            else if(oneGrammar.substr(i,1) == "|") {
                // 加入新句型
                this->grammar[leftPart].push_back(temp);
                temp = "";
            }
            else if(oneGrammar.substr(i, 1) == "@") {
                // 加入空符
                this->Terminal.insert('@');
                this->grammar[leftPart].emplace_back("@");
            }
            else
                // 如果是::=则跳过
                i += 2;
        }
        if(!temp.empty())
            this->grammar[leftPart].push_back(temp);
    }
}

// 打印文法的基本信息
void Grammar::grammarDisplay() {
    // 打印文法数量
    std::cout << "The number of grammar is " << this->grammar_number << std::endl;

    // 打印终结符
    std::cout << "Terminal: ";
    for(auto terminal: this->Terminal)
        std::cout << terminal << ' ';
    std::cout << std::endl;

    // 打印非终结符
    std::cout << "noneTerminal: ";
    for(auto noneterminal: this->noneTerminal)
        std::cout << noneterminal << ' ';
    std::cout << std::endl;

}

// 计算First集
void Grammar::firstSetInit() {
    // 所有的终结符的first集里只有他本身
    for(auto item : this->Terminal) {
        std::set<char> tmp;
        std::string str = std::string() + item;
        tmp.insert(item);
        this->firstSet[str] = tmp;
    }

    // 遍历每个文法
    for(auto & it : this->grammar) {
        // 遍历每个句型
        for (auto const& sentence : it.second) {
            this->firstSet[sentence] = getOneSentenceFirstSet(sentence);
        }
    }
}

// 获取一条句型的first集
std::set<char> Grammar::getOneSentenceFirstSet(const std::string& sentence) {
    // 存储答案
    std::set<char> answer;
    // 如果句型为空，则返回空集
    if (sentence.empty()) return answer;
    // 求句型的第一个字符
    char firstChar = getFirstChar(sentence);

    // 如果是终结符, 则first集就直接是该符号
    if (this->isTerminal(firstChar)) {
        answer.insert(firstChar);
        return answer;
    }
    // 如果是非终结符
    else {
        // 获得该非终结符的first集并加入
        std::set<char> noneTerminalFirstSet = this->getNoneTerminalFirstSet(firstChar);
        answer.insert(noneTerminalFirstSet.begin(), noneTerminalFirstSet.end());

        // 如果该非终结符的first集中有空符，则继续看去除该非终结符的字串的first集，并加入
        if (noneTerminalFirstSet.find('@') != noneTerminalFirstSet.end()) {
            noneTerminalFirstSet = this->getOneSentenceFirstSet(sentence.substr(1));
            answer.insert(noneTerminalFirstSet.begin(), noneTerminalFirstSet.end());
        }
    }

    // 返回答案
    return answer;
}

// 计算非终结符的first集
std::set<char> Grammar::getNoneTerminalFirstSet(const char &noneterminal) {
    // 存储答案
    std::set<char> answer;
    // 遍历非终结符生成的句型集合
    auto sentences = this->grammar[noneterminal];
    for (const auto& item : sentences) {
        // 继续获取每一个句型的first集并加入
        std::set<char> temp = this->getOneSentenceFirstSet(item);
        answer.insert(temp.begin(), temp.end());
    }
    // 返回答案
    return answer;
}

// 打印first集
void Grammar::firstSetDisplay() {
    std::unordered_map<std::string, bool> flags;
    int count = 0;
    // 遍历每条文法
    for(auto & it : this->grammar) {
        // 遍历每个句型
        for (auto const& sentence : it.second) {
            // 如果没有输出过这个句型
            if (!flags[sentence]) {
                // 格式化输出
                std::string firstFormat = getFirstFormat(sentence, this->firstSet[sentence]);
                std::cout << std::left << std::setfill(' ') <<  std::setw(25) << firstFormat;
                if (++count % 4 == 0)
                    std::cout << std::endl;
                // 标记为已输出
                flags[sentence] = true;
            }
        }
    }
    std::cout << std::endl;
}

// 计算Follow集
void Grammar::followSetInit() {
    // 开始符号先加入'#'号
    this->followSet[this->firstSymbol].insert('#');
    // 计算开始符号的follow集
    getNoneTerminalFollowSet(this->firstSymbol);

    // 计算其余的非终结符的follow集
    for(auto noneterminal: this->noneTerminal) {
        if (noneterminal != this->firstSymbol)
            getNoneTerminalFollowSet(noneterminal);
    }
}

// 计算一个非终结符的follow集
void Grammar::getNoneTerminalFollowSet(const char &noneterminal) {
    //遍历每一个句型
    for(const auto& it : this->grammar) {
        for(const auto& sentence : it.second) {

            int index = int(sentence.find(noneterminal));
            // 如果句型中能找到该非终结符，即右部出现该非终结符
            if (index != std::string::npos) {
                // 如果他的后面有其他符号
                if (index < sentence.size() - 1) {
                    std::set<char> firstset = this->getOneSentenceFirstSet(sentence.substr(index + 1));
                    // 如果该符号不能推出空符号，该符号的first集加入follow集
                    if (firstset.find('@') == firstset.end()) {
                        this->followSet[noneterminal].insert(firstset.begin(), firstset.end());
                        break;
                    } else { // 如果后面的符号的可以推导出空符号，该符号的first集加入follow集并且左部的follow集也加入非终结符的follow集
                        firstset.erase('@');
                        this->followSet[noneterminal].insert(firstset.begin(), firstset.end());
                        if (this->followSet[it.first].empty())
                            this->getNoneTerminalFollowSet(it.first);
                        this->followSet[noneterminal].insert(this->followSet[it.first].begin(), this->followSet[it.first].end());
                    }
                }
                else { // 后面没有任何符号，直接左部的follow集加入非终结符的follow集
                    if (this->followSet[it.first].empty())
                        this->getNoneTerminalFollowSet(it.first);
                    this->followSet[noneterminal].insert(this->followSet[it.first].begin(),
                                                         this->followSet[it.first].end());
                }
            }
        }
    }
}

// 打印follow集
void Grammar::followSetDisplay() {
    int count = 0;
    for(auto noneterminal: this->noneTerminal) {
        // 获取格式化的follow集
        std::string followFormat = getFollowFormat(noneterminal, this->followSet[noneterminal]);
        std::cout << std::left << std::setfill(' ') << std::setw(25) << followFormat;
        if(++count % 4 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 判断是否是终结符
bool Grammar::isTerminal(const char& symbol) {
    return this->Terminal.find(symbol) != this->Terminal.end();
}

// 获得first的打印格式
std::string Grammar::getFirstFormat(const std::string &sentence, std::set<char> &firstset) {
    std::ostringstream oss;
    oss << "First(" << sentence << ")={";
    for(auto it = firstset.begin(); it != firstset.end();) {
        oss << *it;
        if(++it != firstset.end())
            oss << ',';
    }
    oss << '}';
    return oss.str();
}

// 获得follow集的打印格式
std::string Grammar::getFollowFormat(char noneterminal, std::set<char> &followset) {
    std::ostringstream oss;
    oss << "Follow(" << noneterminal << ")={";
    for(auto it = followset.begin(); it != followset.end();) {
        oss << *it;
        if(++it != followset.end())
            oss << ',';
    }
    oss << '}';
    return oss.str();
}
