#pragma once
#include "stdafx.h"
//using namespace std;

class lexer
{
private:
	vector<Token> _vToken;
	int _nowTokenNum;
	string argv;
	bool isEof = false;
	int lineNum = 1;
	void tokenize(const string str, vector<Token> &vtoken);
public:
	bool isThereIllegalChar(string str);
	bool isDigit(char ch);
	bool isDigit(string str);
	void makeToken();
	void showAllToken();
	int getNowTokenNum();
	vector<Token> getTokenVector();
	Token getNowToken();
	lexer(const char* argv);
	lexer();
	~lexer();
};

