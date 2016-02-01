#include <iostream>
#include <fstream>
#include <vector>

#include "Parser.h"
#include "LexAnalyzer.h"
#include "Token.h"

int main(int argc, char const *argv[])
{
	std::ifstream file("test.sk");

	LexAnalyzer lAn(&file);

	std::vector<Token*> tokens;

	Token* t = lAn.getNextToken();
	while(t->getType() != ENDOFFILE) {
		tokens.push_back(t);
		t = lAn.getNextToken();
	}
	tokens.push_back(t);

	for (std::vector<Token*>::iterator i = tokens.begin(); i != tokens.end(); ++i)
	{
		if((*i)->getType() == VAR) {
			std::cout << "T_VAR_DEC" << std::endl;
		}
		if((*i)->getType() == IDENTIFIER) {
			std::cout << "T_IDENTIFIER : " << (*i)->getString() << std::endl;
		}
		if((*i)->getType() == OPERATOR) {
			std::cout << "T_OPERATOR : " << (*i)->getString() << std::endl;
		}
		if((*i)->getType() == INTEGER) {
			std::cout << "T_INTEGER : " << (*i)->getInteger() << std::endl;
		}
		if((*i)->getType() == SEMICOLON) {
			std::cout << "T_SEMICOLON" << std::endl;
		}
		if((*i)->getType() == COLON_TYPE) {
			std::cout << "T_TYPE_IDENTIFIER : " << (*i)->getString() << std::endl;
		}
		if((*i)->getType() == USE) {
			std::cout << "T_USE_LIBRARY : " << (*i)->getString() << std::endl;
		}
		if((*i)->getType() == ENDOFFILE) {
			std::cout << "T_EOF" << std::endl;
		}

	}

	Parser p(&tokens);

	p.expression();

	return 0;
}