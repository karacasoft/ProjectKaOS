#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "Token.h"

enum PrimitiveType {
	_INTEGER,
	_LONG,
	_CHAR,
	_BYTE
};

class Parser
{
	std::vector<Token*>::iterator it;
	uint8_t errorState;
	std::vector<Token*>* tokens;

	std::vector<std::string> variables;
	std::vector<PrimitiveType> types;
	std::vector<std::string> initialValues;

	std::vector<std::string> libraries;

	std::ofstream* outFile;
	std::ostringstream code;
	std::ostringstream data;
public:

	void error(std::string err);
	void expected(std::string str);

	TokenType look();
	void match(TokenType t);

	void declareVar();

	void term();

	void add();
	void subtract();
	void multiply();
	void divide();
	void modulo();
	void assign();

	void factor();

	void expression();

	void writeData();
	void writeCode();

	Parser(std::vector<Token*>* v);
	~Parser();
	
};

#endif