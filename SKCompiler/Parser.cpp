#include <iostream>

#include "Parser.h"

Parser::Parser(std::istream* istr) {
	this->is = istr;
	errorState = 0;
	lineCounter = 1;
}

Parser::~Parser() {}

int Parser::isErrorState() {
	return errorState;
}

void Parser::line() {
	lineCounter++;
	char c;
	this->is->read(&c, 1);
}

void Parser::expected(char* str) {
	std::cout << "Compile error:" << std::endl;
	std::cout << lineCounter << ":Expected " << str << std::endl;
	errorState++;
}

bool Parser::isDigit(char c) {
	char num = '0';
	while(num <= '9') {
		if(c == num) {
			return true;
		}
		num++;
	}
	return false;
}

char Parser::look() {
	char c = this->is->peek();
}

void Parser::match(char c) {
	char next = look();
	if(c == next) {
		this->is->read(&next, 1);
	} else {
		expected(&c);
	}
}

void Parser::term() {
	char c = look();
	if(isDigit(c)) {
		std::cout << "mov eax, " << c << std::endl;
		this->is->read(&c, 1);
	}
	c = look();
	if(c == '*') {
		multiply();
	} else if(c == '/') {
		divide();
	} else {}
}

void Parser::add() {
	match('+');
	std::cout << "push eax" << std::endl;
	term();
	std::cout << "add eax, DWORD [ss:esp]" << std::endl;
	std::cout << "add esp, 4" << std::endl;
}

void Parser::subtract() {
	match('-');
	std::cout << "push eax" << std::endl;
	term();
	std::cout << "sub eax, DWORD [ss:esp]" << std::endl;
	std::cout << "neg eax" << std::endl;
	std::cout << "add esp, 4" << std::endl;
}

void Parser::multiply() {
	match('*');
	std::cout << "push eax" << std::endl;
	term();
	std::cout << "mul DWORD [ss:esp]" << std::endl;
	std::cout << "add esp, 4" << std::endl;
}

void Parser::divide() {
	match('/');
	
	std::cout << "push eax" << std::endl;
	term();
	std::cout << "mov ebx, DWORD [ss:esp]" << std::endl;
	std::cout << "xchg eax, ebx" << std::endl;
	std::cout << "push edx" << std::endl;
	std::cout << "xor edx, edx" << std::endl;
	std::cout << "div ebx" << std::endl;
	std::cout << "pop edx" << std::endl;
	std::cout << "add esp, 4" << std::endl;
}

void Parser::expression() {
	term();
	char c = look();
	if(c == '+') {
		add();
	} else if(c == '-') {
		subtract();
	} else {
		expected("operator");
	}
}