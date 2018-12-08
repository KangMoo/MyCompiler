#pragma once
#include "stdafx.h"
struct TypeMap {
	TokenType type;
	string name;
	int arrDemension;
};
class TypeCheckr
{
private:

	void typeCheck(Expression* e, vector<TypeMap> tm[2]);
	//TokenType typeof(Expression* e);
	bool isInTypeMap(Expression* e, vector<TypeMap> tm[2]);
	bool isInTypeMap(Expression* e, vector<TypeMap> tm);
	bool isInTypeMap(string str, vector<TypeMap> tm[2]);
	bool isInTypeMap(string str, vector<TypeMap> tm);
	bool isArrDemensionZero(Variable* v, vector<TypeMap> tm[2]);
	bool isArrDemensionZero(Variable* v, vector<TypeMap> tm);
	bool isArrDemensionOne(Variable* v, vector<TypeMap> tm[2]);
	bool isArrDemensionOne(Variable* v, vector<TypeMap> tm);
	int getDemension(Expression* e, vector<TypeMap> tm[2]);
	int getDemension(Expression* e, vector<TypeMap> tm);
	vector<TypeMap> combine2TypeMap(vector<TypeMap> tm[2]);
public:
	//void typeCheck(Block* b, const int tsp);
	vector<TypeMap> typeCheck(Statement* s, vector<TypeMap> upperTM);
	vector<TypeMap> typeCheck(Statement* s);
	void showTypeMap();
	TypeCheckr();
	~TypeCheckr();
};

