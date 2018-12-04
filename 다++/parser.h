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
	Assignment* arrAssignmnet();
	Statement* Skip();
	Conditional* Ifstatement();
	Conditional* ElseIfstatement();
	Loop* WhileStatement();
	Block* declaration();
	Block* arrDeclaration();
	Command_Input* command_input();
	Command_Output* command_output();
	Command_ArrPushBack* command_arrpushback();
	Command_ArrErase* command_arrpop();
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
	int getArrayDemension(string str);
	
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

