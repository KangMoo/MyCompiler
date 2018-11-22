#pragma once
enum TokenType {
	Bool, Char, Else, False, Float,
	If, ElseIf, Int, Main, True, While,Eol,
	Eof, LeftBrace, RightBrace, LeftBracket, RightBracket,
	LeftParen, RightParen, Semicolon, Comma, Assign,
	Equals, Less, LessEqual, Greater, GreaterEqual,
	Not, NotEqual, Plus, Minus, Multiply,
	Divide, And, Or, Identifier, IntLiteral,
	FloatLiteral, CharLiteral, Period, SingleQuotation
};
