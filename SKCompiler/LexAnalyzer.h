#ifndef LEX_ANALYZER_H
#define LEX_ANALYZER_H

#include <string>

#include "Token.h"

class LexAnalyzer
{
	std::ifstream* is;
public:
	char look();

	uint8_t isAlpha(char c);
	uint8_t isDigit(char c);
	uint8_t isValidChar(char c);
	uint8_t isWhitespace(char c);
	uint8_t isOperator(char c);

	int getNumber();
	std::string getString();
	std::string getStringValue();
	std::string getOperator();
	TokenType identifyToken(std::string token);

	Token* getNextToken();

	void skipWhitespace();

	LexAnalyzer(std::ifstream* _is);
	~LexAnalyzer();
	
};

#endif