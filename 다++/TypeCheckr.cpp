#include "TypeCheckr.h"

vector<TypeMap> TypeCheckr::typeCheck(Statement * s, vector<TypeMap> upperTM)
{
	vector<TypeMap> _typeMap[2];
	_typeMap[0] = upperTM;

	if (s->StatementName == "Assignment")
	{
		Assignment* a = (Assignment*)s;
		typeCheck(a->target, combine2TypeMap(_typeMap));
		typeCheck(a->source, combine2TypeMap(_typeMap));
		int temp1 = getDemension(a->target, _typeMap);
		int temp2 = getDemension(a->source, _typeMap);
		assert(temp1 == temp2 && "Assignment Error : Different Demension Calculation");
	}
	else if (s->StatementName == "Loop")
	{
		Loop* l = (Loop*)s;
		typeCheck(l->test, _typeMap);
		typeCheck(l->body, combine2TypeMap(_typeMap));
	}
	else if (s->StatementName == "Conditional")
	{
		Conditional* c = (Conditional*)s;
		typeCheck(c->test, _typeMap);
		typeCheck(c->thenBranch, combine2TypeMap(_typeMap));
		if (c->isThereElsebranch)
		{
			typeCheck(c->elseBranch, combine2TypeMap(_typeMap));
		}
	}
	else if (s->StatementName == "Declaration")
	{
		Declaration* d = (Declaration*)s;
		for (auto j : d->valueName)
		{
			if (isInTypeMap(j, _typeMap[1]))
			{
				assert(false && "TypeMap Error (Duplicate declaration)");
			}
			TypeMap ttemp;
			ttemp.type = d->valueType;
			ttemp.name = j;
			ttemp.arrDemension = d->arrDemension;
			_typeMap[1].push_back(ttemp);
		}
	}
	else if (s->StatementName == "Block")
	{
		Block* b = (Block*)s;
		vector<TypeMap> temp;

		if (b->isThereBrace)
		{
			for (auto i : b->members)
			{
				temp = typeCheck(i, combine2TypeMap(_typeMap));
				for (auto j : temp)
				{
					_typeMap[1].push_back(j);
				}
			}
		}
		else
		{
			for (auto i : b->members)
			{
				temp = typeCheck(i, combine2TypeMap(_typeMap));
				for (auto j : temp)
				{
					_typeMap[1].push_back(j);
				}
			}
		}
		//	if (_typeMap.size() < nowTypeMapSP)
		//		_typeMap.erase(_typeMap.begin() + nowTypeMapSP + 1, _typeMap.end());
		//}
	}
	else if (s->StatementName == "Expression")
	{
		typeCheck(((Expression*)s),_typeMap);
	}
	else if (s->StatementName == "Command_Input")
	{
		for (auto i : ((Command_Input*)s)->vars)
		{
			assert(isArrDemensionZero(i, _typeMap) && "Demension Error (Should be 'Zero' Demension)");
		}
	}
	else if (s->StatementName == "Command_Output")
	{
		for (auto i : ((Command_Output*)s)->expressions)
		{
			if (i->StatementName == "Variable")
				assert(isArrDemensionZero((Variable*)i, _typeMap) && "Demension Error (Should be 'Zero' Demension)");
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
	return combine2TypeMap(_typeMap);
}

vector<TypeMap> TypeCheckr::typeCheck(Statement * s)
{
	vector<TypeMap> temp;
	temp = typeCheck(s, temp);
	return temp;
}

void TypeCheckr::typeCheck(Expression * e, vector<TypeMap> tm[2])
{
	if (e->StatementName == "Value")
	{
	}
	else if (e->StatementName == "Variable")
	{
		if (!isInTypeMap(((Variable*)e)->id, tm))
		{
			assert(false && "TypeMap Error (Undefined Variable used)");
		}
	}
	else if (e->StatementName == "Binary")
	{
		Binary* b = ((Binary*)e);
		typeCheck(b->term1, tm);
		typeCheck(b->term2, tm);
		int temp1 = getDemension(b->term1, tm);
		int temp2 = getDemension(b->term2, tm);
		assert(temp1 == temp2 && "Binary Error : Different Demension Calculation");
	}
	else if (e->StatementName == "Unary")
	{
		Unary* u = ((Unary*)e);
		typeCheck(u->term, tm);
	}
	return;
}

bool TypeCheckr::isInTypeMap(Expression * e, vector<TypeMap> tm[2])
{
	if (isInTypeMap(e, tm[0]) || isInTypeMap(e, tm[1]))
		return true;
	return false;
}
bool TypeCheckr::isInTypeMap(Expression * e, vector<TypeMap> tm)
{
	for (auto i : tm)
	{
		if (i.name == ((Variable*)e)->id)
		{
			return true;
		}
	}
	return false;
}
bool TypeCheckr::isInTypeMap(string str, vector<TypeMap> tm[2])
{
	if (isInTypeMap(str, tm[0]) || isInTypeMap(str, tm[1]))
		return true;
	return false;
}

bool TypeCheckr::isInTypeMap(string str, vector<TypeMap> tm)
{
	for (auto i : tm)
	{
		if (i.name == str)
		{
			return true;
		}
	}
	return false;
}

bool TypeCheckr::isArrDemensionZero(Variable * v, vector<TypeMap> tm[2])
{
	if (isArrDemensionZero(v, tm[0]) || isArrDemensionZero(v, tm[1]))
		return true;
	return false;
}

bool TypeCheckr::isArrDemensionZero(Variable * v, vector<TypeMap> tm)
{
	for (auto i : tm)
	{
		if (i.name == v->id)
		{
			if (i.arrDemension == v->arrNum.size()) return true;
		}
	}
	return false;
}

bool TypeCheckr::isArrDemensionOne(Variable * v, vector<TypeMap> tm[2])
{
	if (isArrDemensionOne(v, tm[0]) || isArrDemensionOne(v, tm[1]))
		return true;
	return false;
}

bool TypeCheckr::isArrDemensionOne(Variable * v, vector<TypeMap> tm)
{
	for (auto i : tm)
	{
		if (i.name == v->id)
		{
			if (i.arrDemension - v->arrNum.size() == 1) return true;
		}
	}
	return false;
}

int TypeCheckr::getDemension(Expression * e, vector<TypeMap> tm[2])
{
	return getDemension(e, combine2TypeMap(tm));
}


int TypeCheckr::getDemension(Expression * e, vector<TypeMap> tm)
{
	int demension = 0;

	if (e->StatementName == "Value")
	{
		demension = 0;
	}
	else if (e->StatementName == "Variable")
	{
		if (!isInTypeMap(((Variable*)e)->id,tm))
		{
			assert(false && "TypeMap Error (Undefined Variable used)");
		}
		int tempdemension = 0;
		for (auto i : tm)
		{
			if (i.name == ((Variable*)e)->id)
				tempdemension = i.arrDemension;
		}

		demension = tempdemension - ((Variable*)e)->arrNum.size();
	}
	else if (e->StatementName == "Binary")
	{
		Binary* b = ((Binary*)e);
		int temp1 = getDemension(b->term1, tm);
		int temp2 = getDemension(b->term2, tm);
		assert(temp1 == temp2 && "Binary Error : Different Demension Calculation");
	}
	else if (e->StatementName == "Unary")
	{
		Unary* u = ((Unary*)e);
		demension = getDemension(u->term, tm);
	}
	return demension;
}

vector<TypeMap> TypeCheckr::combine2TypeMap(vector<TypeMap> tm[2])
{
	vector<TypeMap> temp = tm[0];
	for (auto i : tm[1])
	{
		temp.push_back(i);
	}
	return temp;
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

}


TypeCheckr::~TypeCheckr()
{
}
