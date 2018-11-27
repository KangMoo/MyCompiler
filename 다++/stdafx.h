#pragma once
#include "TokenType.h"
//#include "Token.h"
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

typedef struct TOKEN {
	TokenType TokenType;
	string TokenName;
	string TokenValue;
}Token;


class Operator {
public:
	string val;
	Operator() {};
	Operator(string str) { val = str; };
};
class Statement {
public:
	string StatementName;
public:
	string getStatementName() { return StatementName; };
};

class Expression : public Statement {
public:
	Expression() { StatementName = "Expression"; };
};
class Variable : public Expression {
public:
	string id;
	TokenType type;
	bool undef = true;
	int intValue;
	float floatValue;
	bool boolValue;
	char charValue;
public:
	Variable() {};
	Variable(Token t) {
		type = t.TokenType;
		id = t.TokenName;
		if (type == IntLiteral)
		{
			intValue = stoi(t.TokenValue);
		}
		else if (type == FloatLiteral)
		{
			floatValue = stof(t.TokenValue);
		}
		else if (type == CharLiteral)
		{
			charValue = t.TokenValue[0];
		}
		else if (type == True)
		{
			boolValue = true;
		}
		else if (type == False)
		{
			boolValue = false;
		}
		StatementName = "Variable";
	}
};
class Value : public Expression {
public:
	TokenType type;
	bool undef = true;
	int intValue;
	float floatValue;
	bool boolValue;
	char charValue;
public:
	Value(Token t) {
		type = t.TokenType;

		if (type == IntLiteral)
		{
			intValue = stoi(t.TokenValue);
		}
		else if (type == FloatLiteral)
		{
			floatValue = stof(t.TokenValue);
		}
		else if (type == CharLiteral)
		{
			charValue = t.TokenValue[0];
		}
		else if (type == True)
		{
			boolValue = true;
		}
		else if (type == False)
		{
			boolValue = false;
		}
		StatementName = "Value";
	}
};
class Binary :public Expression {
public:
	Operator* op;
	Expression* term1;
	Expression* term2;
public:
	Binary(Operator* o, Expression* l, Expression* r) {
		op = o; term1 = l; term2 = r;
		StatementName = "Binary";
	}
};
class Unary :public Expression {
public:
	Operator* op;
	Expression* term;
public:
	Unary(Operator* o, Expression* e) {
		op = o; term = e;
		StatementName = "Unary";
	};

};
class Block : public Statement {
public:
	vector<Statement*> members;
	bool isThereBrace = false;
	Block(bool brace) { StatementName = "Block"; isThereBrace = brace; };
};

class Skip : public Statement {
public:
};
class Assignment : public Statement {
	// Assignment --> Identifier = Expression ;
public:
	Variable* target;
	Expression* source;
	Assignment() { StatementName = "Assignment"; };
	Assignment(Variable* t, Expression* e) { StatementName = "Assignment"; target = t; source = e; };
};
class Conditional : public Statement {
	// IfStatement --> if ( Expression ) Statement [ else Statement ]
public:
	Expression* test;
	Statement* thenBranch;
	Statement* elseBranch;
	bool isTherElsebranch = false;
	Conditional() { StatementName = "Conditional"; };
	Conditional(Expression* t, Statement* tp) { StatementName = "Conditional"; test = t; thenBranch = tp; isTherElsebranch = false; };
	Conditional(Expression* t, Statement* tp, Statement* ep) {
		StatementName = "Conditional"; test = t; thenBranch = tp; elseBranch = ep; isTherElsebranch = true;
	};
};
class Loop : public Statement {
	// WhileStatement --> while ( Expression ) Statement
public:
	Expression* test;
	Statement* body;
	Loop() { StatementName = "Loop"; };
	Loop(Expression* t, Statement* b) { StatementName = "Loop"; test = t; body = b; };
};
class Declaration : public Statement {
public:
	TokenType valueType;
	vector<string> valueName;
	vector<Assignment*> assign;
	Declaration() { StatementName = "Declaration"; };
};

#define ARRAY_LEN(X) (sizeof(X)/sizeof(X[0]))