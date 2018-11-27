#include "CodeGenerator.h"



string CodeGenerator::tabSet(int c)
{
	string str;
	for (int i = 0; i < c; i++)
	{
		str += "\t";
	}
	return str;
}

CodeGenerator::CodeGenerator()
{
}


CodeGenerator::~CodeGenerator()
{
}

string CodeGenerator::generateStatement(Statement * s, int tab)
{
	string str = "";
	if (s->getStatementName() == "Expression")
	{
		str += generateExpression((Expression*)s);
	}
	else if (s->getStatementName() == "Block")
	{
		str += generateBlock((Block*)s, tab);
	}
	else if (s->getStatementName() == "Assignment")
	{
		str += generateAssignment((Assignment*)s, tab);
	}
	else if (s->getStatementName() == "Conditional")
	{
		str += generateConditional((Conditional*)s, tab);
	}
	else if (s->getStatementName() == "Loop")
	{
		str += generateLoop((Loop*)s, tab);
	}
	else if (s->getStatementName() == "Declaration")
	{
		str += generateDeclaration((Declaration*)s, tab);
	}
	return str;
}

string CodeGenerator::generateExpression(Expression * e)
{
	string str = "";
	if (e->StatementName == "Variable")
	{
		str += generateVariable((Variable*)e);
	}
	else if (e->StatementName == "Value")
	{
		str += generateValue((Value*)e);
	}
	else if (e->StatementName == "Binary")
	{
		str += generateBinary((Binary*)e);
	}
	else if (e->StatementName == "Unary")
	{
		str += generateUnary((Unary*)e);
	}
	else
	{

	}
	return str;
}

string CodeGenerator::generateAssignment(Assignment * a, int tab)
{
	string str = "";
	str += tabSet(tab);
	str += a->target->id;
	str += "=";
	str += generateExpression(a->source);
	str += ";\n";
	return str;
}

string CodeGenerator::generateConditional(Conditional * c, int tab)
{
	string str = "";

	str += tabSet(tab);
	str += "if";
	str += "(";
	str += generateExpression(c->test);
	str += ")\n";
	//str += tabSet(tab++) + "{\n";
	str += generateStatement(c->thenBranch, tab);
	//str += tabSet(--tab) + "}\n";

	if (c->isTherElsebranch)
	{
		str += tabSet(tab) + "else";
		//str += tabSet(tab++) + "{\n";
		str += generateStatement(c->elseBranch, tab);
		//str += tabSet(--tab) + "}\n";
	}
	return str;
}

string CodeGenerator::generateLoop(Loop * l, int tab)
{
	string str = "";
	str += tabSet(tab) + "while" + "(";
	str += generateExpression(l->test) + ")\n";
	str += tabSet(tab++) + "{\n";
	generateStatement(l->body, tab);
	str += tabSet(--tab) + "}\n";

	return str;
}

string CodeGenerator::generateDeclaration(Declaration * d, int tab)
{
	string str = "";

	str += tabSet(tab);
	switch (d->valueType)
	{
	case (TokenType)Int:
		str += "int ";
		break;
	case (TokenType)Float:
		str += "float ";
		break;
	case (TokenType)Char:
		str += "char ";
		break;
	case (TokenType)Bool:
		str += "bool ";
		break;
	}

	for (int i = 0; i < d->valueName.size(); i++)
	{
		str += d->valueName[i];
		if (i != d->valueName.size() - 1)
		{
			str += ", ";
		}
	}
	str += ";\n";
	return str;
}

string CodeGenerator::generateBlock(Block * b, int tab)
{
	string str = "";
	if (b->isThereBrace)
	{
		str += tabSet(tab++) + "{\n";
	
	}
	for (int i = 0; i < b->members.size(); i++)
	{
		str += generateStatement(b->members[i], tab);
	}
	if (b->isThereBrace)
	{
		str += tabSet(--tab) + "}\n";
	}
	return str;
}

string CodeGenerator::generateVariable(Variable * v)
{
	string str = "";
	str += v->id;
	return str;
}

string CodeGenerator::generateValue(Value * v)
{
	string str = "";
	if (v->type == IntLiteral)
	{
		str += to_string(v->intValue);
	}
	else if (v->type == FloatLiteral)
	{
		str += to_string(v->floatValue);
	}
	else if (v->type == CharLiteral)
	{
		str += to_string(v->charValue);
	}
	else if (v->type == True)
	{
		str += "true";
	}
	else if (v->type == False)
	{
		str += "false";
	}
	return str;
}

string CodeGenerator::generateBinary(Binary * b)
{
	string str = "";
	str += "(";
	str += generateExpression(b->term1);
	str += b->op->val;
	str += generateExpression(b->term2);
	str += ")";
	return str;
}

string CodeGenerator::generateUnary(Unary * u)
{
	string str = "";
	str += "(";
	str += u->op->val;
	str += generateExpression(u->term);
	str += ")";
	return str;
}

void CodeGenerator::mkCode()
{
	_file.open(_fileName);
	int tab = 0;
	_file << mkCodeStart();
	string str = generateBlock(_block,0);
	_file << str;
	_file.close();
}

string CodeGenerator::mkCodeStart()
{
	string str = "";
	str += "#include <iostream>\n";
	str += "int main()\n";
	return str;
}
