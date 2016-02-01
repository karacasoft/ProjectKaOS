#include <iostream>
#include <string>
#include <fstream>
#include "LexAnalyzer.h"

#include "Token.h"

LexAnalyzer::LexAnalyzer(std::ifstream* _is) {
	this->is = _is;
}

LexAnalyzer::~LexAnalyzer() {

}

char LexAnalyzer::look() {
	char c = this->is->peek();
	return c;
}

uint8_t LexAnalyzer::isAlpha(char c) {
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
		return true;
	}
	return false;
}

uint8_t LexAnalyzer::isValidChar(char c) {
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || isDigit(c)) {
		return true;
	}
	return false;
}

uint8_t LexAnalyzer::isDigit(char c) {
	if(c >= '0' && c <= '9') {
		return true;
	}
	return false;
}

uint8_t LexAnalyzer::isWhitespace(char c) {
	if(c == ' ' || c == '\n' || c == '\t')
		return true;
	return false;
}

uint8_t LexAnalyzer::isOperator(char c) {
	if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '=' || c == '<' || c == '>') {
		return true;
	}
	return false;
}

int LexAnalyzer::getNumber() {
	int number = 0;
	char c = look();
	while(isDigit(look())) {
		c = this->is->get();
		number *= 10;
		number += (c - '0');
	}
	return number;
}

std::string LexAnalyzer::getOperator() {
	std::string token;
	char c = look();
	while(isOperator(look())) {
		c = this->is->get();
		token += c;
	}
	return token;
}

std::string LexAnalyzer::getString() {
	std::string token;
	char c = look();
	while(isValidChar(look())) {
		c = this->is->get();
		token += c;
	}
	return token;
}

std::string LexAnalyzer::getStringValue() {
	std::string token;
	char c = look();
	while(look() != '"')) {
		c = this->is->get();
		token += c;
	}
	this->is->get();
	return token;
}

TokenType LexAnalyzer::identifyToken(std::string token) {
	
	TokenType type;
	if(token == "define") {
		type = DEFINE;
	} else if(token == "var") {
		type = VAR;
	} else if(token == "end") {
		type = END;
	} else if(token == "use") {
		type = USE;
	} else {
		type = IDENTIFIER;
	}
	return type;
}

Token* LexAnalyzer::getNextToken() {
	
	char c = look();
	while(isWhitespace(c)) {
		this->is->get();
		c = look();
	}

	if(isAlpha(c)) {
		std::string token = getString();
		TokenType ttype = identifyToken(token);
		if(ttype == USE) {
			//skip whitspace
			while(isWhitespace(look())) {
				this->is->get();
			}
			if(look() == '<') {
				this->is->get();
			}
			//get library name
			token = getString();

			if(look() == '>') {
				this->is->get();
			}

		}
		Token* t = new Token(ttype, token);
		return t;
	}

	if(isDigit(c)) {
		int i = getNumber();
		Token* t = new Token(INTEGER, i);
		return t;
	}

	if(isOperator(c)) {
		std::string op = getOperator();
		Token* t = new Token(OPERATOR, op);
		return t;
	}

	if(c == ':') {
		c = this->is->get();

		//skip whitspace
		while(isWhitespace(look())) {
			this->is->get();
		}

		//get type identifier
		std::string typeIdentifier = getString();

		Token* t = new Token(COLON_TYPE, typeIdentifier);
		return t;
	}

	if(c == ';') {
		c = this->is->get();
		Token* t = new Token(SEMICOLON);
		return t;
	}

	if(c == '(') {
		c = this->is->get();
		Token* t = new Token(PARAN_OPEN);
		return t;
	}

	if(c == ')') {
		c = this->is->get();
		Token* t = new Token(PARAN_CLOSE);
		return t;
	}

	if(c == '"') {
		c = this->is->get();
		Token* t = new Token(STRING, getStringValue());
		return t;
	}

	Token* t = new Token(ENDOFFILE);
	return t;
}