#ifndef TOKEN_H
#define TOKEN_H

#include <string>


enum TokenType
{
	USE,
	INTEGER,
	STRING,
	IDENTIFIER,
	COLON_TYPE,
	OPERATOR,
	DEFINE,
	VAR,
	END,
	SEMICOLON,
	PARAN_OPEN,
	PARAN_CLOSE,
	ENDOFFILE,
	NO_TOKEN
};

class Token
{
	TokenType type;
	int _integer;
	char _char;
	std::string _string;
public:

	Token(TokenType _type);
	Token(TokenType _type, int _value);
	Token(TokenType _type, char _value);
	Token(TokenType _type, std::string _value);
	~Token();


	TokenType getType();
	int getInteger();
	char getChar();
	std::string getString();
	
};

#endif