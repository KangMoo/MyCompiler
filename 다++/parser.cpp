#include "parser.h"
#include "stdafx.h"


void parser::init()
{
	_nowTokenNum;
	_token = _vToken[_nowTokenNum];
}

void parser::setTokenVector(vector<Token> vtoken)
{
	_nowTokenNum = 0;
	_vToken = vtoken;
	_token = _vToken[_nowTokenNum];
}

bool parser::isStatement()
{
	if (_token.TokenType == (TokenType)Int || _token.TokenType == (TokenType)Char || _token.TokenType == (TokenType)String || _token.TokenType == (TokenType)Bool || _token.TokenType == (TokenType)Float || _token.TokenType == (TokenType)Array ||
		_token.TokenType == (TokenType)Semicolon || _token.TokenType == (TokenType)LeftBrace || _token.TokenType == (TokenType)If ||
		_token.TokenType == (TokenType)While || _token.TokenType == (TokenType)Identifier ||
		_token.TokenType == (TokenType)CommandInput || _token.TokenType == (TokenType)CommandOutput || _token.TokenType == (TokenType)CommandArrPushBack|| _token.TokenType == (TokenType)CommandArrPop)
	{
		return true;
	}
	return false;
}

Block * parser::getBlock()
{
	return _block;
}

void parser::parse()
{
	match((TokenType)Int);
	match((TokenType)Main);
	match((TokenType)LeftParen);
	match((TokenType)RightParen);
	match((TokenType)LeftBrace);
	//
	_block = Statements();
	_block->isThereBrace = true;
	//
	match((TokenType)RightBrace);
}

Block* parser::Statements()
{
	bool isThereLeftBrace = false;
	if (_token.TokenType == (TokenType)LeftBrace)
	{
		match((TokenType)LeftBrace,"(Statements)");
		isThereLeftBrace = true;
	}
	Block *b = new Block(isThereLeftBrace);
	while (isStatement())
	{
		Statement* s = new Statement();
		s = statementf();
		b->members.push_back(s);
	}
	if (isThereLeftBrace)
	{
		match((TokenType)RightBrace, "(Statements)");
	}
	return b;
}

Statement * parser::statementf()
{
	Statement *temp = new Statement();
	if (_token.TokenType == (TokenType)Int ||
		_token.TokenType == (TokenType)Char ||
		_token.TokenType == (TokenType)String ||
		_token.TokenType == (TokenType)Float ||
		_token.TokenType == (TokenType)Bool)
	{
		temp = declaration();
	}
	else if (_token.TokenType == (TokenType)Array)
	{
		temp = arrDeclaration();
	}
	else if (_token.TokenType == (TokenType)Semicolon)
	{
		temp = Skip();
	}
	else if (_token.TokenType == (TokenType)LeftBrace)
	{
		temp = Statements();
	}
	else if (_token.TokenType == (TokenType)If)
	{
		temp = Ifstatement();
	}
	else if (_token.TokenType == (TokenType)While)
	{
		temp = WhileStatement();
	}
	else if (_token.TokenType == (TokenType)Identifier)
	{
		temp = assignment();
	}
	else if (_token.TokenType == (TokenType)CommandInput)
	{
		temp = command_input();
	}
	else if (_token.TokenType == (TokenType)CommandOutput)
	{
		temp = command_output();
	}
	else if (_token.TokenType == (TokenType)CommandArrPushBack)
	{
		temp = command_arrpushback();
	}
	else if(_token.TokenType == (TokenType)CommandArrPop)
	{
		temp = command_arrpop();
	}
	else
	{
		cout << "TokenNum:" << _nowTokenNum << ", TokenName:" << _token.TokenName << endl;
		assert(false && "Statement Error (TokenType MissMatch)");
	}
	return temp;
}

Assignment* parser::assignment()
{
	Variable* var = new Variable(_token);
	match((TokenType)Identifier, "(assignment)");
	while (_token.TokenType == (TokenType)LeftBracket)
	{
		match((TokenType)LeftBracket, "(assignment)");
		if (_token.TokenType == IntLiteral)
		{
			int num = stoi(_token.TokenValue);
			if (num < 0) assert(false && "Should Be Greater Than '0'");
			var->arrNum.push_back(num);
			match((TokenType)IntLiteral, "(assignment)");
		}
		else
		{
			assert(false && "Should be Int literal In Brackey");
		}
		match((TokenType)RightBracket, "(assignment)");
	}
	match((TokenType)Assign, "(assignment)");
	Expression* source = expression();
	match((TokenType)Semicolon, "(assignment)");
	return new Assignment(var, source);
}

Assignment * parser::arrAssignmnet()
{
	return nullptr;
}

Statement* parser::Skip()
{
	Statement *temp = NULL;
	return temp;
}

Conditional* parser::Ifstatement()
{
	bool isThereElseIf = false;
	match((TokenType)If, "(Ifstatement)");
	match((TokenType)LeftParen, "(Ifstatement)");
	Expression *ex = expression();
	match((TokenType)RightParen, "(Ifstatement)");
	Statement* state = statementf();
	vector<Conditional*> vc;
	while (_token.TokenType == (TokenType)ElseIf)
	{
		vc.push_back(ElseIfstatement());
		isThereElseIf = true;
	}
	int stop = 0;
	if (_token.TokenType == (TokenType)Else)
	{
		match((TokenType)Else, "(Ifstatement)");
		if (isThereElseIf)
		{
			return new Conditional(ex, state, statementf(), vc);
		}
		return new Conditional(ex, state, statementf());
	}
	if (isThereElseIf)
		return new Conditional(ex, state, vc);
	return new Conditional(ex, state);
}

Conditional * parser::ElseIfstatement()
{
	match((TokenType)ElseIf, "(ElseIfstatement)");
	match((TokenType)LeftParen, "(ElseIfstatement)");
	Expression *ex = expression();
	match((TokenType)RightParen, "(ElseIfstatement)");
	Statement* state = statementf();
	return new Conditional(ex, state);
}

Loop* parser::WhileStatement()
{
	match((TokenType)While, "(WhileStatement)");
	match((TokenType)LeftParen, "(WhileStatement)");
	Expression* ex = expression();
	match((TokenType)RightParen, "(WhileStatement)");
	return new Loop(ex, statementf());
}

Block* parser::declaration()
{
	Block* b = new Block(false);
	Block* b2 = new Block(false);
	Declaration* d = new Declaration();
	Assignment* a = new Assignment();
	if (_token.TokenType == (TokenType)Int ||
		_token.TokenType == (TokenType)Char ||
		_token.TokenType == (TokenType)Float ||
		_token.TokenType == (TokenType)String)
	{
		d->valueType = match(_token.TokenType);
	}
	// 선언 ~
	if (_token.TokenType == (TokenType)Identifier)
	{
		d->valueName.push_back(_token.TokenName);
		Variable* vartemp = new Variable(_token);
		match(_token.TokenType);

		// 선언 및 정의 ~
		if (_token.TokenType == (TokenType)Assign)
		{
			match(_token.TokenType);
			if (_token.TokenType == (TokenType)IntLiteral ||
				_token.TokenType == (TokenType)FloatLiteral ||
				_token.TokenType == (TokenType)CharLiteral ||
				_token.TokenType == (TokenType)StringLiteral)
			{
				Value* valtemp = new Value(_token);
				match(_token.TokenType);
				a = new Assignment(vartemp, valtemp);
				b2->members.push_back(a);
			}
		}
		// ~ 선언 및 정의

		// 다중선언 ~
		while (_token.TokenType == (TokenType)Comma)
		{
			match(_token.TokenType);
			// 선언 ~
			if (_token.TokenType == (TokenType)Identifier)
			{
				d->valueName.push_back(_token.TokenName);
				Variable* vartemp = new Variable(_token);
				match(_token.TokenType);
				// 선언 및 정의 ~
				if (_token.TokenType == (TokenType)Assign)
				{
					match(_token.TokenType);
					if (_token.TokenType == (TokenType)IntLiteral ||
						_token.TokenType == (TokenType)FloatLiteral ||
						_token.TokenType == (TokenType)CharLiteral ||
						_token.TokenType == (TokenType)StringLiteral)
					{
						Value* valtemp = new Value(_token);
						match(_token.TokenType);
						a = new Assignment(vartemp, valtemp);
						b2->members.push_back(a);
					}
				}
				// ~ 선언 및 정의
			}
			// ~ 선언
		}
		// ~ 다중선언
		match((TokenType)Semicolon, "(Declaration)");
	}
	// ~ 선언
	else
	{
		assert(false && "Declaration Error");
	}
	b->members.push_back(d);
	b->members.push_back(b2);
	return b;
}

Block * parser::arrDeclaration()
{
	int arrnum = getArrayDemension(_token.TokenValue);
	match(_token.TokenType);

	Block* b = new Block(false);
	Block* b2 = new Block(false);
	Declaration* d = new Declaration(arrnum);
	Assignment* a = new Assignment();
	if (_token.TokenType == (TokenType)Int ||
		_token.TokenType == (TokenType)Char ||
		_token.TokenType == (TokenType)Float ||
		_token.TokenType == (TokenType)String)
	{
		d->valueType = match(_token.TokenType);
	}
	else
	{
		assert(false && "Array Declaration Error");
	}
	// 선언 ~
	if (_token.TokenType == (TokenType)Identifier)
	{
		d->valueName.push_back(_token.TokenName);
		Variable* vartemp = new Variable(_token, arrnum);
		match(_token.TokenType);

		// 선언 및 정의 ~
		//if (_token.TokenType == (TokenType)Assign)
		//{
		//	match(_token.TokenType);
		//	if (_token.TokenType == (TokenType)IntLiteral ||
		//		_token.TokenType == (TokenType)FloatLiteral ||
		//		_token.TokenType == (TokenType)CharLiteral ||
		//		_token.TokenType == (TokenType)StringLiteral)
		//	{
		//		Value* valtemp = new Value(_token);
		//		match(_token.TokenType);
		//		a = new Assignment(vartemp, valtemp);
		//		b2->members.push_back(a);
		//	}
		//}
		// ~ 선언 및 정의

		// 다중선언 ~
		while (_token.TokenType == (TokenType)Comma)
		{
			match(_token.TokenType);
			// 선언 ~
			if (_token.TokenType == (TokenType)Identifier)
			{
				d->valueName.push_back(_token.TokenName);
				Variable* vartemp = new Variable(_token, arrnum);
				match(_token.TokenType);
				// 선언 및 정의 ~
				//if (_token.TokenType == (TokenType)Assign)
				//{
				//	match(_token.TokenType);
				//	if (_token.TokenType == (TokenType)IntLiteral ||
				//		_token.TokenType == (TokenType)FloatLiteral ||
				//		_token.TokenType == (TokenType)CharLiteral ||
				//		_token.TokenType == (TokenType)StringLiteral)
				//	{
				//		Value* valtemp = new Value(_token);
				//		match(_token.TokenType);
				//		a = new Assignment(vartemp, valtemp);
				//		b2->members.push_back(a);
				//	}
				//}
				// ~ 선언 및 정의
			}
			// ~ 선언
		}
		// ~ 다중선언
		match((TokenType)Semicolon,"(arrDeclaration)");
	}
	// ~ 선언
	else
	{
		assert(false && "Declaration Error");
	}
	b->members.push_back(d);
	b->members.push_back(b2);
	return b;
}

Command_Input * parser::command_input()
{
	Command_Input* c = new Command_Input();
	c->valueType = _token.TokenType;
	match(_token.TokenType);
	Variable* v = new Variable(_token);
	match((TokenType)Identifier,"(command_input)");
	
	while (_token.TokenType == (TokenType)LeftBracket)
	{
		match((TokenType)LeftBracket,"(command_input)");
		assert(_token.TokenType == (TokenType)IntLiteral && "Should be IntLiteral Between Brackets");
		v->arrNum.push_back(stoi(_token.TokenValue));
		match((TokenType)IntLiteral,"(command_input)");
		match((TokenType)RightBracket,"(command_input)");
	}
	c->vars.push_back(v);
	while (_token.TokenType == (TokenType)Comma)
	{
		match((TokenType)Identifier,"(command_input)");
		Variable* v = new Variable(_token);

		while (_token.TokenType == (TokenType)LeftBracket)
		{
			match((TokenType)LeftBracket, "(command_input)");
			assert(_token.TokenType == (TokenType)IntLiteral && "Should be IntLiteral Between Brackets");
			v->arrNum.push_back(stoi(_token.TokenValue));
			match((TokenType)IntLiteral, "(command_input)");
			match((TokenType)RightBracket, "(command_input)");
		}
		c->vars.push_back(v);
	}
	match((TokenType)Semicolon, "(command_input)");
	return c;
}
Command_Output * parser::command_output()
{
	Command_Output* c = new Command_Output();
	c->valueType = _token.TokenType;
	match(_token.TokenType);
	Expression* e = nullptr;
	e = expression();
	c->expressions.push_back(e);
	while (_token.TokenType == (TokenType)Comma)
	{
		match((TokenType)Comma, "(command_output)");
		e = expression();
		c->expressions.push_back(e);
	}
	match((TokenType)Semicolon, "(command_output)");
	return c;
}

Command_ArrPushBack * parser::command_arrpushback()
{
	Command_ArrPushBack* c = new Command_ArrPushBack();
	c->valueType = _token.TokenType;
	match(_token.TokenType);
	c->var = new Variable(_token);
	match((TokenType)Identifier, "(command_arrpushback)");
	
	while (_token.TokenType == (TokenType)LeftBracket)
	{
		match((TokenType)LeftBracket, "(command_arrpushback)");
		assert(_token.TokenType == (TokenType)IntLiteral && "Should be IntLiteral Between Brackets");
		c->var->arrNum.push_back(stoi(_token.TokenValue));
		match((TokenType)IntLiteral, "(command_arrpushback)");
		match((TokenType)RightBracket, "(command_arrpushback)");
	}

	Expression* e = nullptr;
	match((TokenType)LeftParen, "(command_arrpushback)");
	e = expression();
	c->expressions.push_back(e);
	while (_token.TokenType == (TokenType)Comma)
	{
		match((TokenType)Comma, "(command_arrpushback)");
		e = expression();
		c->expressions.push_back(e);
	}
	match((TokenType)RightParen, "(command_arrpushback)");
	match((TokenType)Semicolon, "(command_arrpushback)");
	return c;
}

Command_ArrErase * parser::command_arrpop()
{
	Command_ArrErase* c = new Command_ArrErase();
	c->valueType = _token.TokenType;
	match(_token.TokenType);
	c->var = new Variable(_token);
	match((TokenType)Identifier, "(command_arrpop)");

	while (_token.TokenType == (TokenType)LeftBracket)
	{
		match((TokenType)LeftBracket, "(command_arrpop)");
		assert(_token.TokenType == (TokenType)IntLiteral && "Should be IntLiteral Between Brackets");
		c->var->arrNum.push_back(stoi(_token.TokenValue));
		match((TokenType)IntLiteral, "(command_arrpop)");
		match((TokenType)RightBracket, "(command_arrpop)");
	}
	match((TokenType)LeftParen, "(command_arrpop)");
	c->popStart = stoi(_token.TokenValue);
	match((TokenType)IntLiteral, "(command_arrpop)");
	match((TokenType)Comma, "(command_arrpop)");
	c->popEnd = stoi(_token.TokenValue);
	match((TokenType)IntLiteral, "(command_arrpop)");
	match((TokenType)RightParen, "(command_arrpop)");
	match((TokenType)Semicolon, "(command_arrpop)");
	return c;
}

Expression * parser::expression()
{
	Expression* c = nullptr;
	c = conjunction();
	while (_token.TokenType == (TokenType)Or)
	{
		Operator* op = new Operator(_token.TokenValue);
		match(_token.TokenType);
		return new Binary(op, c, expression());
	}
	return c;
}

Expression * parser::conjunction()
{
	Expression* eq = equality();
	while (_token.TokenType == (TokenType)And)
	{
		Operator* op = new Operator(_token.TokenValue);
		match(_token.TokenType);
		return new Binary(op, eq, conjunction());
	}
	return eq;
}

Expression * parser::equality()
{
	Expression* rel = relation();
	while (_token.TokenType == (TokenType)Equals || _token.TokenType == (TokenType)NotEqual)
	{
		Operator* op = new Operator(_token.TokenValue);
		match(_token.TokenType);
		return new Binary(op, rel, relation());
	}
	return rel;
}

Expression * parser::relation()
{
	Expression* add = addition();
	while (_token.TokenType == (TokenType)Less ||
		_token.TokenType == (TokenType)LessEqual ||
		_token.TokenType == (TokenType)Greater ||
		_token.TokenType == (TokenType)GreaterEqual)
	{
		Operator* op = new Operator(_token.TokenValue);
		match(_token.TokenType);
		return new Binary(op, add, addition());
	}
	return add;
}

Expression * parser::addition()
{
	Expression* t = term();
	while (_token.TokenType == (TokenType)Plus || _token.TokenType == (TokenType)Minus)
	{
		Operator* op = new Operator(_token.TokenValue);
		match(_token.TokenType);
		return new Binary(op, t, term());
	}
	return t;
}

Expression * parser::term()
{
	Expression* f = factor();
	while (_token.TokenType == (TokenType)Multiply || _token.TokenType == (TokenType)Divide)
	{
		Operator* op = new Operator(_token.TokenValue);
		match(_token.TokenType);
		return new Binary(op, f, factor());
	}
	return f;
}

Expression * parser::factor()
{
	if (_token.TokenType == (TokenType)Not || _token.TokenType == (TokenType)Minus)
	{
		Operator* op = new Operator(_token.TokenValue);
		match(_token.TokenType);
		return new Unary(op, primary());
	}
	return primary();
}


Expression* parser::primary()
{
	Expression *e = NULL;
	if (_token.TokenType == (TokenType)Identifier)
	{
		e = new Variable(_token);
		match(_token.TokenType);
		while (_token.TokenType == (TokenType)LeftBracket)
		{
			match((TokenType)LeftBracket, "(primary)");
			if (_token.TokenType == IntLiteral)
			{
				if (stoi(_token.TokenValue) < 0) assert(false && "Should be Greater Than 0 in Bracket");
				((Variable*)e)->arrNum.push_back(stoi(_token.TokenValue));
				match((TokenType)IntLiteral, "(primary)");
			}
			else
			{
				assert(false && "Should be IntLiteral Between Bracket");
			}
			match((TokenType)RightBracket, "(primary)");
		}
		//((Variable*)e)->arrDemension = arrnum;
	}
	else if (_token.TokenType == IntLiteral || _token.TokenType == CharLiteral || _token.TokenType == (TokenType)StringLiteral || _token.TokenType == FloatLiteral || _token.TokenType == True || _token.TokenType == False)
	{
		e = new Value(_token);
		match(_token.TokenType);
	}
	else if (_token.TokenType == (TokenType)LeftParen)
	{
		match(_token.TokenType);
		e = expression();
		match(RightParen, "(primary)");
	}
	else if (_token.TokenType == Int || _token.TokenType == Char || _token.TokenType == Float || _token.TokenType == (TokenType)String || _token.TokenType == Bool)
	{
		string val = _token.TokenValue;
		Operator* op = new Operator(val);
		match(_token.TokenType);
		match((TokenType)LeftParen, "(primary)");
		Expression* term = expression();
		match((TokenType)RightParen, "(primary)");
		e = new Unary(op, term);
	}
	else
	{
		cout << _nowTokenNum << endl;
		cout << _token.TokenName << endl;
		assert(false && "Primary Error");
	}
	return e;
}

int parser::getArrayDemension(string str)
{
	string temp = "";
	for (int i = 0; i < str.size(); i++)
	{
		if ('0' <= str[i] && str[i] <= '9')
		{
			temp = temp + str[i];
		}
		else
			break;
	}
	return stoi(temp);
}

TokenType parser::match(TokenType tokenType,string mes)
{
	if (_token.TokenType == tokenType)
	{
		if (_nowTokenNum == _vToken.size() - 1)
			return tokenType;
		_nowTokenNum++;
		_token = _vToken[_nowTokenNum];
		return tokenType;
	}
	cout << _nowTokenNum << endl;
	cout << _token.TokenType << endl;
	cout << _token.TokenName << endl;
	cout << mes << endl;
	assert(false && "TokenType Mismatch!!");
}

parser::parser()
{
}


parser::~parser()
{
}
