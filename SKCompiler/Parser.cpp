#include <string>
#include <vector>

#include "Parser.h"
#include "Token.h"

std::string assignMemory;

std::string tokenTypeToString(TokenType t) {
	switch(t) {
		case USE:
			return std::string("use token");
		case INTEGER:
			return std::string("integer constant");
		case IDENTIFIER:
			return std::string("identifier");
		case COLON_TYPE:
			return std::string("type identifier");
		case OPERATOR:
			return std::string("operator");
		case DEFINE:
			return std::string("define token");
		case VAR:
			return std::string("var token");
		case END:
			return std::string("end token");
		case SEMICOLON:
			return std::string("end of statement");
		case PARAN_OPEN:
			return std::string("(");
		case PARAN_CLOSE:
			return std::string(")");
		case ENDOFFILE:
			return std::string("End of file");
		case NO_TOKEN:
			return std::string("No token...");
	}
}

Parser::Parser(std::vector<Token*>* v) {
	this->tokens = v;
	this->it = v->begin();

	this->errorState = 0;

	this->outFile = new std::ofstream("out.asm");

	*this->outFile << "global go" << std::endl;
	*this->outFile << "extern _ExitProcess@4" << std::endl;
	*this->outFile << "extern _GetStdHandle@4" << std::endl;
	*this->outFile << "extern _WriteConsoleA@20" << std::endl;
	*this->outFile << std::endl;

}

Parser::~Parser() {
	delete this->outFile;
}

void Parser::error(std::string err) {
	std::cout << "Parse error : " << err << std::endl;
	this->errorState++;
}

void Parser::expected(std::string str) {
	error("Expected : " + str);
}

TokenType Parser::look() {
	return (*it)->getType();
}

void Parser::match(TokenType t) {
	if((*it)->getType() == t) {
		it++;
	} else {
		expected(tokenTypeToString(t));
	}
}

void Parser::declareVar() {
	match(VAR);
	variables.push_back((*it)->getString());
	data << (*it)->getString() << ":";
	match(IDENTIFIER);
	if(look() == COLON_TYPE) {
		if((*it)->getString() == "int") {
			types.push_back(_INTEGER);
			data << " dw";
		}
		//TODO add more types
		match(COLON_TYPE);
	}
	if(look() == OPERATOR) {
		if((*it)->getString() == "<-") {
			match(OPERATOR);
			data << " " << (*it)->getInteger();
		} else {
			error("Unexpected operator " + (*it)->getString());
		}
		//TODO
	} else {
		data << " 0";
	}
	data << std::endl;
}

void Parser::term() {
	if(look() == INTEGER) {
		code << "mov eax, " << (*it)->getInteger() << std::endl;
		match(INTEGER);
	} else if(look() == IDENTIFIER) {
		std::string vName = (*it)->getString();
		match(IDENTIFIER);
		if(look() == OPERATOR && (*it)->getString() == "<-") {
			//assignment to a variable
			assignMemory = vName;
		} else {
			code << "mov eax, " << vName << std::endl;
		}
		
	}
	if(look() == OPERATOR) {
		if((*it)->getString() == "*") {
			multiply();
		} else if((*it)->getString() == "/") {
			divide();
		} else if((*it)->getString() == "%") {
			modulo();
		}
	}
	if(look() == PARAN_OPEN) {
		factor();
	}
}

void Parser::add() {
	match(OPERATOR);
	code << "push eax" << std::endl;
	term();
	code << "add eax, DWORD [ss:esp]" << std::endl;
	code << "add esp, 4" << std::endl;
}

void Parser::subtract() {
	match(OPERATOR);
	code << "push eax" << std::endl;
	term();
	code << "sub eax, DWORD [ss:esp]" << std::endl;
	code << "neg eax" << std::endl;
	code << "add esp, 4" << std::endl;
}

void Parser::multiply() {
	match(OPERATOR);
	code << "push eax" << std::endl;
	term();
	code << "mul DWORD [ss:esp]" << std::endl;
	code << "add esp, 4" << std::endl;
}

void Parser::divide() {
	match(OPERATOR);
	code << "push eax" << std::endl;
	term();
	code << "mov ebx, eax" << std::endl;
	code << "mov eax, DWORD [ss:esp]" << std::endl;
	code << "xor edx, edx" << std::endl;
	code << "div ebx" << std::endl;
	code << "add esp, 4" << std::endl;
}

void Parser::modulo() {
	match(OPERATOR);
	code << "push eax" << std::endl;
	term();
	code << "mov ebx, eax" << std::endl;
	code << "mov eax, DWORD [ss:esp]" << std::endl;
	code << "xor edx, edx" << std::endl;
	code << "div ebx" << std::endl;
	code << "mov eax, edx" << std::endl;
	code << "add esp, 4" << std::endl;
}

void Parser::assign() {
	match(OPERATOR);
	expression();
	code << "mov [" << assignMemory << "], eax" << std::endl;
}

void Parser::factor() {
	match(PARAN_OPEN);
	expression();
	match(PARAN_CLOSE);
}


void Parser::expression() {
	if(look() == USE) {
		libraries.push_back((*it)->getString());
		expression();
		return;
	}

	if(look() == VAR) {
		declareVar();
	}
	
	term();
	while(look() == OPERATOR) {
		if((*it)->getString() == "<-") {
			assign();
			break;
		} else if((*it)->getString() == "+") {
			add();
		} else if((*it)->getString() == "-") {
			subtract();
		}
	}
	match(SEMICOLON);
	std::cout << "test" << std::endl;
	if(look() != ENDOFFILE) {
		expression();
	} else {

		writeData();
		writeCode();
	}


}

void Parser::writeData() {
	(*this->outFile) << "section .data" << std::endl;
	(*this->outFile) << data.str() << std::endl;
	(*this->outFile) << "handle: db 0" << std::endl;
	(*this->outFile) << "written: db 0" << std::endl;
}

void Parser::writeCode() {
	(*this->outFile) << "go:" << std::endl;
	(*this->outFile) << "section .text" << std::endl;
	(*this->outFile) << "push dword -11" << std::endl;
	(*this->outFile) << "call _GetStdHandle@4" << std::endl;
	(*this->outFile) << "mov [handle], eax" << std::endl;

	(*this->outFile) << code.str() << std::endl;

	(*this->outFile) << "push dword 0" << std::endl;
	(*this->outFile) << "push written" << std::endl;
	(*this->outFile) << "push dword 13" << std::endl;
	(*this->outFile) << "push test" << std::endl;
	(*this->outFile) << "push dword [handle]" << std::endl;
	(*this->outFile) << "call _WriteConsoleA@20" << std::endl;

	(*this->outFile) << "push dword 0" << std::endl;
	(*this->outFile) << "call _ExitProcess@4" << std::endl;
}