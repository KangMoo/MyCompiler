#include "AST.h"



void AST::setBlock(Block * block)
{
	_block = block;
}

void AST::display()
{
	cout << "<Program>" << endl;;
	displayBlock(_block, 0);
}

void AST::displayBlock(Block * b, int tab)
{
	if (b->isThereBrace)
	{
		tabSet(tab); cout << "<Block>" << endl;
		tabSet(tab++); cout << "{" << endl;
		
	}
	for (int i = 0; i < b->members.size(); i++)
	{
		displayStatement(b->members[i], tab);
	}
	if (b->isThereBrace)
	{
		tabSet(--tab); cout << "}" << endl;
	}
	
}

void AST::displayStatement(Statement * s, int tab)
{
	if (s->getStatementName() == "Expression")
	{
		displayExpression((Expression*)s);
	}
	else if (s->getStatementName() == "Block")
	{
		displayBlock((Block*)s, tab);
	}
	else if (s->getStatementName() == "Assignment")
	{
		displayAssignment((Assignment*)s, tab);
	}
	else if (s->getStatementName() == "Conditional")
	{
		displayConditional((Conditional*)s, tab);
	}
	else if (s->getStatementName() == "Loop")
	{
		displayLoop((Loop*)s, tab);
	}
	else if (s->getStatementName() == "Declaration")
	{
		displayDeclaration((Declaration*)s, tab);
	}
}

void AST::displayExpression(Expression * e)
{

	if (e->StatementName == "Variable")
	{
		displayVariable((Variable*)e);
	}
	else if (e->StatementName == "Value")
	{
		displayValue((Value*)e);
	}
	else if (e->StatementName == "Binary")
	{
		displayBinary((Binary*)e);
	}
	else if (e->StatementName == "Unary")
	{
		displayUnary((Unary*)e);
	}
	else
	{

	}
}

void AST::displayAssignment(Assignment * a, int tab)
{
	tabSet(tab); cout << "<Assignment>" << endl;;
	tabSet(tab + 1); cout << "Target :" << a->target->id << endl;
	tabSet(tab + 1); cout << "Source :";
	displayExpression(a->source);
	cout << endl;
}

void AST::displayConditional(Conditional * c, int tab)
{
	tabSet(tab); cout << "<If>" << endl;
	tabSet(tab); cout << "If Condition:"; displayExpression(c->test); cout << endl;
	tabSet(tab++); cout << "{" << endl;
	displayStatement(c->thenBranch, tab);
	tabSet(--tab); cout << "}" << endl;
	if (c->isTherElsebranch)
	{
		tabSet(tab); cout << "<Else>" << endl;
		tabSet(tab++); cout << "{" << endl;
		displayStatement(c->elseBranch, tab);
		tabSet(--tab); cout << "}" << endl;
	}
}

void AST::displayLoop(Loop * l, int tab)
{
	tabSet(tab); cout << "<Loop>" << endl;
	tabSet(tab); cout << "Loop Condition:"; displayExpression(l->test); cout << endl;
	tabSet(tab++); cout << "{" << endl;
	displayStatement(l->body, tab);
	tabSet(--tab); cout << "}" << endl;
}

void AST::displayDeclaration(Declaration* d, int tab)
{
	tabSet(tab); cout << "<Declaration>" << endl;

	tabSet(tab + 1); cout << "Type : ";
	switch (d->valueType)
	{
	case (TokenType)Int:
		cout << "Int";
		break;
	case (TokenType)Float:
		cout << "Float";
		break;
	case (TokenType)Char:
		cout << "Char";
		break;
	case (TokenType)Bool:
		cout << "Bool";
		break;
	}
	cout << endl;

	for (auto i : d->valueName)
	{
		tabSet(tab + 1); cout << "Name : " << i << endl;
	}
}

void AST::displayVariable(Variable * v)
{
	cout << "(Var:" << v->id << ")";
}

void AST::displayValue(Value * v)
{
	if (v->type == IntLiteral)
	{
		cout << "(Int:" << v->intValue << ")";
	}
	else if (v->type == FloatLiteral)
	{
		cout << "(Float:" << v->floatValue << ")";
	}
	else if (v->type == CharLiteral)
	{
		cout << "(char:" << v->charValue << ")";
	}
	else if (v->type == True)
	{
		cout << "(bool:" << "True" << ")";
	}
	else if (v->type == False)
	{
		cout << "(bool:" << "False" << ")";
	}
}

void AST::displayBinary(Binary * b)
{
	cout << "(";
	displayExpression(b->term1);
	cout << b->op->val;
	displayExpression(b->term2);
	cout << ")";
}

void AST::displayUnary(Unary * u)
{
	cout << "((" << u->op->val << ")";
	displayExpression(u->term);
	cout << ")";
}

void AST::tabSet(int c)
{
	for (int i = 0; i < c; i++)
	{
		cout << "\t";
	}
}


AST::AST()
{
	_block = new Block(false);
}


AST::~AST()
{
}
