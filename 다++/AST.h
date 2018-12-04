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
	void displayICommand(Command_Input* c, int tab);
	void displayOCommand(Command_Output* c, int tab);
	void displayArrPBCommand(Command_ArrPushBack* c, int tab);
	void displayArrPCommand(Command_ArrErase* c, int tab);
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

