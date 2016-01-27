#include <iostream>
#include <fstream>
#include "Parser.h"


int main(int argc, char const *argv[])
{
	Parser p(&std::cin);

	char c;
	while(!std::cin.eof() && p.isErrorState() <= 0) {
		c = p.look();
		if(c == '\n')
		{
			p.line();
		}
		p.expression();
	}
	
	

	//std::cout << isDigit(c[0]) << std::endl;
	return 0;
}