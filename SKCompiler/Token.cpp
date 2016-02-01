#include <string>
#include "Token.h"

Token::Token(TokenType _type) {
	this->type = _type;
}

Token::Token(TokenType _type, int _value) {
	this->type = _type;
	this->_integer = _value;
}

Token::Token(TokenType _type, char _value) {
	this->type = _type;
	this->_char = _value;
}

Token::Token(TokenType _type, std::string _value) {
	this->type = _type;
	this->_string = _value;
}

Token::~Token() {

}

TokenType Token::getType() {
	return this->type;
}

int Token::getInteger() {
	return this->_integer;
}

char Token::getChar() {
	return this->_char;
}

std::string Token::getString() {
	return this->_string;
}