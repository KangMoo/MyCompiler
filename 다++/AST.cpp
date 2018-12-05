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
	else if (s->getStatementName() == "Command_Input")
	{
		displayICommand((Command_Input*)s, tab);
	}
	else if (s->getStatementName() == "Command_Output")
	{
		displayOCommand((Command_Output*)s, tab);
	}
	else if (s->getStatementName() == "Command_ArrPushBack")
	{
		displayArrPBCommand((Command_ArrPushBack*)s, tab);
	}
	else if (s->getStatementName() == "Command_ArrErase")
	{
		displayArrPCommand((Command_ArrErase*)s, tab);
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
	tabSet(tab + 1); cout << "Target :" << a->target->id;
	for (auto i : a->target->arrNum)
	{
		cout << "[" << i << "]";
	}
	cout << endl;
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
	for (auto i : c->elseIfBranch)
	{
		displayEIConditional(i, tab);
	}
	if (c->isThereElsebranch)
	{
		tabSet(tab); cout << "<Else>" << endl;
		tabSet(tab++); cout << "{" << endl;
		displayStatement(c->elseBranch, tab);
		tabSet(--tab); cout << "}" << endl;
	}
}

void AST::displayEIConditional(Conditional * c, int tab)
{
	tabSet(tab); cout << "<Else If>" << endl;
	tabSet(tab); cout << "Else If Condition:"; displayExpression(c->test); cout << endl;
	tabSet(tab++); cout << "{" << endl;
	displayStatement(c->thenBranch, tab);
	tabSet(--tab); cout << "}" << endl;

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
	case (TokenType)String:
		cout << "String";
		break;
	case (TokenType)Bool:
		cout << "Bool";
		break;
	}
	if (d->arrDemension > 0)
	{
		cout << " (Array Demension : " << d->arrDemension<<")";
	}
	cout << endl;

	for (auto i : d->valueName)
	{
		tabSet(tab + 1); cout << "Name : " << i << endl;
	}
	
}

void AST::displayICommand(Command_Input * c, int tab)
{
	tabSet(tab); cout << "<Command>" << endl;

	tabSet(tab + 1); cout << "Input : ";
	for (auto i : c->vars)
	{
		cout << i->id;
		for (auto j : i->arrNum)
		{
			cout << "["<<j<< "]";
		}
		cout << " ";
	}
	cout << endl;
}
void AST::displayOCommand(Command_Output * c, int tab)
{
	tabSet(tab); cout << "<Command>" << endl;

	tabSet(tab + 1); cout << "Output : ";
	for (auto i : c->expressions)
	{
		displayExpression(i);
		cout << " ";
	}
	cout << endl;
}
void AST::displayArrPBCommand(Command_ArrPushBack * c, int tab)
{
	tabSet(tab); cout << "<Array Push_Back>" << endl;
	tabSet(tab + 1); cout << "Target : "; displayExpression(c->var); cout << endl;
	for (auto i : c->expressions)
	{
		tabSet(tab + 1); cout << "Source : "; displayExpression(i); cout << endl;
	}
	cout << endl;
}
void AST::displayArrPCommand(Command_ArrErase * c, int tab)
{

	tabSet(tab); cout << "<Array Erase>" << endl;
	tabSet(tab + 1); cout << "Target : "; displayExpression(c->var); cout << endl;
	tabSet(tab + 1); cout << "Erase : "; cout << c->popStart << " ~ " << c->popEnd << endl;
	cout << endl;
}

void AST::displayVariable(Variable * v)
{

	cout << "(Var:" << v->id;
	for (auto i : v->arrNum)
	{
		if (i->getStatementName() == "Value")
		{
			cout << "[" << i << "]";
		}
		else if (i->getStatementName() == "Variable")
		{
			cout << "[(Var:" << ((Variable*)i)->id<< ")]";
		}
	}
	cout << ")";

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
	else if (v->type == StringLiteral)
	{
		cout << "(string:" << v->stringValue << ")";
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
