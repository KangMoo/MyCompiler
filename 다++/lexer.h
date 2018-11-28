#pragma once
#include "stdafx.h"

class lexer
{
private:
	vector<Token> _vToken;
	int _nowTokenNum;
	string argv;
	bool isEof = false;
	int lineNum = 1;
	void tokenize(string str, vector<Token> &vtoken);
	void tokenCheck(vector<Token> &vtoken);

	bool isThereIllegalChar(string str);
	bool isDigit(char ch);
	bool isDigit(string str);
	bool isItArrayDeclare(string str);
	bool isItArrayFactor(string str);
public:
	void makeToken();
	void showAllToken();
	int getNowTokenNum();
	vector<Token> getTokenVector();
	Token getNowToken();
	lexer(const char* argv);
	lexer();
	~lexer();
};

