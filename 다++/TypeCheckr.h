#pragma once
#include "stdafx.h"
struct TypeMap {
	TokenType type;
	string name;
};
class TypeCheckr
{
private:
	vector<TypeMap> _typeMap;

	void typeCheck(Expression* e);
	//TokenType typeof(Expression* e);
	bool isInTypeMap(Expression* e);
	bool isInTypeMap(string str);
public:
	//void typeCheck(Block* b, const int tsp);
	void typeCheck(Statement* s, const int tsp);
	void showTypeMap();
	TypeCheckr();
	~TypeCheckr();
};

