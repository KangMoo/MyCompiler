#pragma once
#include "stdafx.h"




class parser
{
private:
	vector<Token> _vToken;
	int _nowTokenNum;
	Token _token;

	Block *_block;


	//
	Block* Statements();
	Statement* statementf();
	Assignment* assignment();
	Statement* Skip();
	Conditional* Ifstatement();
	Loop* WhileStatement();
	Block* declaration();
	//
	Expression* expression();
	Expression* conjunction();
	Expression* equality();
	Expression* relation();
	Expression* addition();
	Expression* term();
	Expression* factor();
	Expression* primary();
	//

public:
	void init();
	void parse();
	void setTokenVector(vector<Token> vtoken);
	bool isStatement();
	Block* getBlock();
	TokenType match(TokenType tokenType);
	parser();
	~parser();
};

