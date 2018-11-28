#pragma once
#include "stdafx.h"
class AST
{
	Block* _block;

	//displaystatement
	void displayStatement(Statement* s, int tab);
	void displayExpression(Expression* e);
	void displayAssignment(Assignment* a, int tab);
	void displayConditional(Conditional* c, int tab);
	void displayEIConditional(Conditional* c, int tab);
	void displayLoop(Loop* l, int tab);
	void displayDeclaration(Declaration* d, int tab);

	//displayexpression
	void displayVariable(Variable* v);
	void displayValue(Value* v);
	void displayBinary(Binary* b);
	void displayUnary(Unary* u);
public:
	void setBlock(Block* block);
	void displayBlock(Block* b, int tab);
	void display();
	

	void tabSet(int c);
	AST();
	~AST();
};

