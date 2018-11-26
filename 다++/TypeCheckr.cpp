#include "TypeCheckr.h"

void TypeCheckr::typeCheck(Statement * s, const int tsp)
{
	int nowTypeMapSP = tsp;
	if (s->StatementName == "Assignment")
	{
		Assignment* a = (Assignment*)s;
		typeCheck(a->target);
		typeCheck(a->source);
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
		if (c->isTherElsebranch) 
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
			_typeMap.push_back(ttemp);
			nowTypeMapSP++;
		}
	}
	else if(s->StatementName == "Block")
	{
		Block* b = (Block*)s;
		for (auto i : b->members)
		{
			typeCheck(i, nowTypeMapSP);
		}
		_typeMap.erase(_typeMap.begin() + nowTypeMapSP+1, _typeMap.end());
	}
	else if (s->StatementName == "Expression")
	{
		typeCheck(((Expression*)s));
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
