#include <iostream>

class Parser
{
std::istream* is;
int lineCounter;
int errorState;
public:
	Parser(std::istream *is);
	~Parser();

	int isErrorState();
	void expected(char* c);
	bool isDigit(char c);
	char look();
	void line();
	void match(char c);
	void term();
	void add();
	void subtract();
	void multiply();
	void divide();
	void expression();

};