#include <iostream>
#include "StringAnalysis.h"
#include "grammar.h"
#include "LL1Parser.h"
int main()
{
	std::vector<std::string> grammarList;
    std::string path = "../grammar.txt";
	readFile(grammarList, path);
	printGrammar(grammarList);
	Grammar grammar(grammarList);
    grammar.grammarDisplay();
    grammar.firstSetInit();
    grammar.firstSetDisplay();
    grammar.followSetInit();
    grammar.followSetDisplay();

    LL1Parser ll1Parser(grammar);
    ll1Parser.displayParserTable();
    bool res = ll1Parser.analyzeString("eao");
    if(res)
        std::cout << "该串是文法的句子";
    else
        std::cout << "该串不是文法的句子";
	return 0;
}
