#pragma once
enum TokenType {
	Bool, Char, Else, False, Float, String, Array,
	If, ElseIf, Int, Main, True, While,Eol,
	Eof, LeftBrace, RightBrace, LeftBracket, RightBracket,
	LeftParen, RightParen, Semicolon, Comma, Assign,
	Equals, Less, LessEqual, Greater, GreaterEqual,
	Not, NotEqual, Plus, Minus, Multiply,
	Divide, And, Or, Identifier, IntLiteral, StringLiteral,
	FloatLiteral, CharLiteral, Period, SingleQuotation, DoubleQuotation
};
