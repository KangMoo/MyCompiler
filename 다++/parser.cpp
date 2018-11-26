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
	if (_token.TokenType == (TokenType)Int || _token.TokenType == (TokenType)Char || _token.TokenType == (TokenType)Bool || _token.TokenType == (TokenType)Float ||
		_token.TokenType == (TokenType)Semicolon || _token.TokenType == (TokenType)LeftBrace || _token.TokenType == (TokenType)If ||
		_token.TokenType == (TokenType)While || _token.TokenType == (TokenType)Identifier)
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
	//
	match((TokenType)RightBrace);
}

Block* parser::Statements()
{
	bool isThereLeftBrace = false;
	if (_token.TokenType == (TokenType)LeftBrace)
	{
		match((TokenType)LeftBrace);
		isThereLeftBrace = true;
	}

	Block *b = new Block();
	while (isStatement())
	{
		Statement* s = new Statement();
		s = statementf();
		b->members.push_back(s);
	}
	if (isThereLeftBrace)
	{
		match((TokenType)RightBrace);
	}
	return b;
}

Statement * parser::statementf()
{
	Statement *temp = new Statement();
	if (_token.TokenType == (TokenType)Int|| _token.TokenType == (TokenType)Char || _token.TokenType == (TokenType)Float || _token.TokenType == (TokenType)Bool)
	{
		temp = declaration();
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
	match((TokenType)Identifier);
	match((TokenType)Assign);
	Expression* source = expression();
	match((TokenType)Semicolon);
	return new Assignment(var, source);
}

Statement* parser::Skip()
{
	Statement *temp = NULL;
	return temp;
}

Conditional* parser::Ifstatement()
{
	match((TokenType)If);
	match((TokenType)LeftParen);
	Expression *ex = expression();
	match((TokenType)RightParen);
	Statement* state = statementf();
	if (_token.TokenType == (TokenType)Else)
	{
		match((TokenType)Else);
		return new Conditional(ex, state, statementf());
	}
	return new Conditional(ex, state);
}

Loop* parser::WhileStatement()
{
	match((TokenType)While);
	match((TokenType)LeftParen);
	Expression* ex = expression();
	match((TokenType)RightParen);
	return new Loop(ex, statementf());
}

Declaration* parser::declaration()
{
	Declaration* temp = new Declaration();
	temp->valueType = match(_token.TokenType);
	if (_token.TokenType == (TokenType)Identifier)
	{
		temp->valueName.push_back(_token.TokenName);
		match(_token.TokenType);
		while (_token.TokenType == (TokenType)Comma)
		{
			match(_token.TokenType);
			if (_token.TokenType == (TokenType)Identifier)
			{
				temp->valueName.push_back(_token.TokenName);
				match(_token.TokenType);
			}
		}
		match((TokenType)Semicolon);
	}
	else
	{
		assert(false && "Declaration Error");
	}
	
	return temp;
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
	}
	else if (_token.TokenType == IntLiteral || _token.TokenType == CharLiteral || _token.TokenType == FloatLiteral || _token.TokenType == True || _token.TokenType == False)
	{
		e = new Value(_token);
		match(_token.TokenType);
	}
	else if (_token.TokenType == (TokenType)LeftParen)
	{
		match(_token.TokenType);
		e = expression();
		match(RightParen);
	}
	else if (_token.TokenType == Int || _token.TokenType == Char || _token.TokenType == Float || _token.TokenType == Bool)
	{
		string val = _token.TokenValue;
		Operator* op = new Operator(val);
		match(_token.TokenType);
		match((TokenType)LeftParen);
		Expression* term = expression();
		match((TokenType)RightParen);
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

TokenType parser::match(TokenType tokenType)
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
	cout << _token.TokenName << endl;
	assert(false && "TokenType Mismatch!!");
}

parser::parser()
{
}


parser::~parser()
{
}
