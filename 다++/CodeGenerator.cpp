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
	else if (s->getStatementName() == "Command_Input")
	{
		str += generateCommandInput((Command_Input*)s, tab);
	}
	else if (s->getStatementName() == "Command_Output")
	{
		str += generateCommandOutput((Command_Output*)s, tab);
	}
	else if (s->getStatementName() == "Command_ArrPushBack")
	{
		str += generateCommandArrPB((Command_ArrPushBack*)s, tab);
	}
	else if (s->getStatementName() == "Command_ArrErase")
	{
		str += generateCommandArrE((Command_ArrErase*)s, tab);
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
	for (auto i : a->target->arrNum)
	{
		str += "[";
		str += to_string(i);
		str += "]";
	}
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

	for (auto i : c->elseIfBranch)
	{
		str += generateEIConditional(i, tab);
	}

	if (c->isThereElsebranch)
	{
		str += tabSet(tab) + "else\n";
		//str += tabSet(tab++) + "{\n";
		str += generateStatement(c->elseBranch, tab);
		//str += tabSet(--tab) + "}\n";
	}
	return str;
}

string CodeGenerator::generateEIConditional(Conditional * ec, int tab)
{
	string str = "";
	str += tabSet(tab);
	str += "else if";
	str += "(";
	str += generateExpression(ec->test);
	str += ")\n";
	//str += tabSet(tab++) + "{\n";
	str += generateStatement(ec->thenBranch, tab);
	//str += tabSet(--tab) + "}\n";
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
	for (int i = 0; i < d->arrDemension; i++)
	{
		str += "vector<";
	}

	switch (d->valueType)
	{
	case (TokenType)Int:
		str += "int";
		break;
	case (TokenType)Float:
		str += "float";
		break;
	case (TokenType)Char:
		str += "char";
		break;
	case (TokenType)String:
		str += "string";
		break;
	case (TokenType)Bool:
		str += "bool";
		break;
	}

	for (int i = 0; i < d->arrDemension; i++)
	{
		str += ">";
	}
	str += " ";
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

string CodeGenerator::generateBlock(Block * b, int tab, bool mainBlock)
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
		if (mainBlock)
		{
			str += tabSet(tab) + "system(\"pause\");\n";
			str += tabSet(tab) + "return 0;\n";
		}
		str += tabSet(--tab) + "}\n";
	}
	return str;
}

string CodeGenerator::generateCommandInput(Command_Input * c, int tab)
{
	string str = "";
	str += tabSet(tab) + "cin>>";
	for (int i = 0; i < c->vars.size(); i++)
	{
		str += c->vars[i]->id;
		for (auto i : c->vars[i]->arrNum)
		{
			str += "[";
			str += to_string(i);
			str += "]";
		}
		if (i != c->vars.size() - 1) str += ">>";
	}
	str += ";\n";
	return str;
}

string CodeGenerator::generateCommandOutput(Command_Output * c, int tab)
{
	string str = "";
	str += tabSet(tab) + "cout<<";
	for (int i = 0; i < c->expressions.size(); i++)
	{
		str += generateExpression(c->expressions[i]);

		if (i != c->expressions.size() - 1) str += "<<";
		else str += "<<endl;\n";
	}
	return str;
}

string CodeGenerator::generateCommandArrPB(Command_ArrPushBack * c, int tab)
{
	string str = "";
	for (auto i : c->expressions)
	{
		str += tabSet(tab);
		str += c->var->id;
		for (auto j : c->var->arrNum)
		{
			str += "[";
			str += to_string(j);
			str += "]";
		}
		str += ".push_back(";
		str += generateExpression(i);
		str += ");\n";
	}
	return str;
}

string CodeGenerator::generateCommandArrE(Command_ArrErase * c, int tab)
{
	string str = "";

	str += tabSet(tab);
	str += c->var->id;
	for (auto i : c->var->arrNum)
	{
		str += "[";
		str += to_string(i);
		str += "]";
	}
	str += ".erase(";
	str += c->var->id;
	for (auto i : c->var->arrNum)
	{
		str += "[";
		str += to_string(i);
		str += "]";
	}
	str += ".begin()+";
	str += to_string(c->popStart);
	str += ",";
	str += c->var->id;
	for (auto i : c->var->arrNum)
	{
		str += "[";
		str += to_string(i);
		str += "]";
	}
	str += ".begin()+";
	str += to_string(c->popEnd);
	str += ");\n";
	return str;
}

string CodeGenerator::generateVariable(Variable * v)
{
	string str = "";
	str += v->id;
	for (auto i : v->arrNum)
	{
		str += "[";
		str += to_string(i);
		str += "]";
	}
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
	else if (v->type == StringLiteral)
	{
		str += "\"" + v->stringValue + "\"";
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

void CodeGenerator::mkCode(bool mkcpp)
{
	if (mkcpp)
	{
		_file.open(_fileName);
		int tab = 0;
		_file << mkCodeStart(mkcpp);
		string str = generateBlock(_block, 0, true);
		_file << str;
		_file.close();
	}
	else
	{
		_file.open(_fileName);
		int tab = 0;
		_file << mkCodeStart();
		string str = generateBlock(_block, 0, true);
		_file << str;
		_file.close();
	}
}

string CodeGenerator::mkCodeStart(bool mkcpp)
{
	string str = "";
	if (mkcpp)
	{
		str += "#include \"generatedCode.h\"\n";
		str += "using namespace std;\n";
		str += "int generatedCode::main()\n";
	}
	else
	{
		str += "#include <iostream>\n";
		str += "#include <vector>\n";
		str += "using namespace std;\n";
		str += "int main()\n";
	}
	return str;
}
