#include "TypeCheckr.h"

void TypeCheckr::typeCheck(Statement * s, const int tsp)
{
	int nowTypeMapSP = tsp;
	if (s->StatementName == "Assignment")
	{
		Assignment* a = (Assignment*)s;
		typeCheck(a->target);
		typeCheck(a->source);
		int temp1 = getDemension(a->target);
		int temp2 = getDemension(a->source);
		assert(temp1 == temp2 && "Assignment Error : Different Demension Calculation");
	}
	else if (s->StatementName == "Loop")
	{
		Loop* l = (Loop*)s;
		typeCheck(l->test);
		typeCheck(l->body, nowTypeMapSP);
	}
	else if (s->StatementName == "Conditional")
	{
		Conditional* c = (Conditional*)s;
		typeCheck(c->test);
		typeCheck(c->thenBranch, nowTypeMapSP);
		if (c->isThereElsebranch)
		{
			typeCheck(c->elseBranch, nowTypeMapSP);
		}
	}
	else if (s->StatementName == "Declaration")
	{
		Declaration* d = (Declaration*)s;
		TokenType t = d->valueType;
		for (auto j : d->valueName)
		{
			if (isInTypeMap(j))
			{
				assert(false && "TypeMap Error (Duplicate declaration)");
			}
			TypeMap ttemp;
			ttemp.type = t;
			ttemp.name = j;
			ttemp.arrDemension = d->arrDemension;
			_typeMap.push_back(ttemp);
			nowTypeMapSP++;
		}
	}
	else if (s->StatementName == "Block")
	{
		Block* b = (Block*)s;
		for (auto i : b->members)
		{
			typeCheck(i, nowTypeMapSP);
		}
		if (b->isThereBrace)
		{
			if (_typeMap.size() < nowTypeMapSP)
				_typeMap.erase(_typeMap.begin() + nowTypeMapSP + 1, _typeMap.end());
		}
	}
	else if (s->StatementName == "Expression")
	{
		typeCheck(((Expression*)s));
	}
	else if (s->StatementName == "Command_Input")
	{
		for (auto i : ((Command_Input*)s)->vars)
		{
			assert(isArrDemensionZero(i) && "Demension Error (Should be 'Zero' Demension)");
		}
	}
	else if (s->StatementName == "Command_Output")
	{
		for (auto i : ((Command_Output*)s)->expressions)
		{
			if (i->StatementName == "Variable")
				assert(isArrDemensionZero((Variable*)i) && "Demension Error (Should be 'Zero' Demension)");
		}
	}
	else if (s->StatementName == "Command_ArrPushBack")
	{
		//assert(isArrDemensionOne(((Command_ArrPushBack*)s)->var) && "Demension Error");
		//for (auto i : ((Command_ArrPushBack*)s)->expressions)
		//{
		//	if (i->StatementName == "Variable")
		//		assert(isArrDemensionZero((Variable*)i) && "Demension Error (Should be 'Zero' Demension)");
		//}
	}
	else if (s->StatementName == "Command_ArrErase")
	{
		//assert(isArrDemensionOne(((Command_ArrErase*)s)->var) && "Demension Error");
	}
}

void TypeCheckr::typeCheck(Expression * e)
{
	if (e->StatementName == "Value")
	{
	}
	else if (e->StatementName == "Variable")
	{
		if (!isInTypeMap(((Variable*)e)->id))
		{
			assert(false && "TypeMap Error (Undefined Variable used)");
		}
	}
	else if (e->StatementName == "Binary")
	{
		Binary* b = ((Binary*)e);
		typeCheck(b->term1);
		typeCheck(b->term2);
		int temp1 = getDemension(b->term1);
		int temp2 = getDemension(b->term2);
		assert(temp1 == temp2 && "Binary Error : Different Demension Calculation");
	}
	else if (e->StatementName == "Unary")
	{
		Unary* u = ((Unary*)e);
		typeCheck(u->term);
	}
	return;
}

bool TypeCheckr::isInTypeMap(Expression * e)
{
	for (auto i : _typeMap)
	{
		if (i.name == ((Variable*)e)->id)
		{
			return true;
		}
	}
	return false;
}
bool TypeCheckr::isInTypeMap(string str)
{
	for (auto i : _typeMap)
	{
		if (i.name == str)
		{
			return true;
		}
	}
	return false;
}

bool TypeCheckr::isArrDemensionZero(Variable * v)
{

	for (auto i : _typeMap)
	{
		if (i.name == v->id)
		{
			if (i.arrDemension == v->arrNum.size()) return true;
		}
	}
	return false;
}

bool TypeCheckr::isArrDemensionOne(Variable * v)
{
	for (auto i : _typeMap)
	{
		if (i.name == v->id)
		{
			if (i.arrDemension - v->arrNum.size() == 1) return true;
		}
	}
	return false;
}

int TypeCheckr::getDemension(Expression * e)
{
	int demension = 0;

	if (e->StatementName == "Value")
	{
		demension = 0;
	}
	else if (e->StatementName == "Variable")
	{
		if (!isInTypeMap(((Variable*)e)->id))
		{
			assert(false && "TypeMap Error (Undefined Variable used)");
		}
		int tempdemension = 0;
		for (auto i : _typeMap)
		{
			if (i.name == ((Variable*)e)->id)
				tempdemension = i.arrDemension;
		}

		demension = tempdemension - ((Variable*)e)->arrNum.size();
	}
	else if (e->StatementName == "Binary")
	{
		Binary* b = ((Binary*)e);
		int temp1 = getDemension(b->term1);
		int temp2 = getDemension(b->term2);
		assert(temp1 == temp2 && "Binary Error : Different Demension Calculation");
	}
	else if (e->StatementName == "Unary")
	{
		Unary* u = ((Unary*)e);
		demension = getDemension(u->term);
	}
	return demension;
}

//void TypeCheckr::typeCheck(Block * b, const int tsp)
//{
//	int nowTypeMapSP = tsp;
//	for (auto i : b->members)
//	{
//		typeCheck(i, nowTypeMapSP + 1);
//	}
//	_typeMap.erase(_typeMap.begin() + tsp, _typeMap.end());
//}



void TypeCheckr::showTypeMap()
{
	//for (auto i : _typeMap) {
	//	cout << "<" << i.name;
	//	switch (i.type)
	//	{
	//	case (TokenType)Int:
	//		cout << ",Int>" << endl;
	//		break;
	//	case (TokenType)Float:
	//		cout << ",Float>" << endl;
	//		break;
	//	case (TokenType)Char:
	//		cout << ",Char>" << endl;
	//		break;
	//	case (TokenType)Bool:
	//		cout << ",Bool>" << endl;
	//		break;
	//	}
	//}
}

TypeCheckr::TypeCheckr()
{
	_typeMap.clear();
}


TypeCheckr::~TypeCheckr()
{
}
