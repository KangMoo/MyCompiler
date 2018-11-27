#pragma once
#include "stdafx.h"
class CodeGenerator
{
private:
	ofstream _file;
	string _fileName;
	Block* _block;
	string _string;
	
	//Statement Code Generate
	string generateStatement(Statement* s, int tab);
	string generateExpression(Expression* e);
	string generateAssignment(Assignment* a, int tab);
	string generateConditional(Conditional* c, int tab);
	string generateLoop(Loop* l, int tab);
	string generateDeclaration(Declaration* d, int tab);
	string generateBlock(Block* b, int tab);

	//Expression Code Generate
	string generateVariable(Variable* v);
	string generateValue(Value* v);
	string generateBinary(Binary* b);
	string generateUnary(Unary* u);
	string tabSet(int c);
public:

	void mkCode();
	string mkCodeStart();
	void setBodyBlock(Block* block) { _block = block; };
	void setFileName(string fileName) { _fileName = fileName; };
	CodeGenerator(string fileName, Block* block) { _fileName = fileName; _block = block; };
	CodeGenerator(string fileName) { _fileName = fileName; };
	CodeGenerator();
	~CodeGenerator();
};

