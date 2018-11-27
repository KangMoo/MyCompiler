#include "lexer.h"
#include "stdafx.h"


lexer::lexer(const char * argv)
{

	_vToken.clear();
	_nowTokenNum = 0;
	this->argv = argv;
	lineNum = 1;
}

lexer::lexer()
{
	_vToken.clear();
	_nowTokenNum = 0;
	lineNum = 1;
}


lexer::~lexer()
{
}

void lexer::tokenize(const string str, vector<Token> &vtoken)
{
	if (str.size() == 0) return;
	vector<int> tkp;
	vector<string> tokstr;
	//strtok_s 함수 이용

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == ',' || str[i] == '.' ||
			str[i] == ';' || str[i] == '{' || str[i] == '}' || str[i] == '(' || str[i] == ')' || str[i] == '&' ||
			str[i] == '|' || str[i] == '!' || str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '?' || str[i] == '='
			|| str[i] == '<' || str[i] == '>' || str[i] == '\'' || str[i] == '\"')
		{
			tkp.push_back(i);
		}
	}

	int j = 0;
	for (int i = 0; i < tkp.size(); i++)
	{
		if (tkp[i] == 0 || tkp[i] - tkp[j] == 1)
		{
			tokstr.push_back(string(str.begin() + tkp[i], str.begin() + tkp[i] + 1));
			if (i == tkp.size() - 1)
			{
				tokstr.push_back(string(str.begin() + tkp[i] + 1, str.end()));
			}
		}
		else
		{
			if (i == 0)
			{
				tokstr.push_back(string(str.begin(), str.begin() + tkp[i]));
				tokstr.push_back(string(str.begin() + tkp[i], str.begin() + tkp[i] + 1));
			}
			else if (i == tkp.size() - 1)
			{
				if (tkp[i] == str.size() - 1)
				{
					tokstr.push_back(string(str.begin() + tkp[i - 1] + 1, str.begin() + tkp[i]));
					tokstr.push_back(string(str.begin() + tkp[i], str.begin() + tkp[i] + 1));
				}
				else
				{
					tokstr.push_back(string(str.begin() + tkp[i - 1] + 1, str.begin() + tkp[i]));
					tokstr.push_back(string(str.begin() + tkp[i], str.begin() + tkp[i] + 1));
					tokstr.push_back(string(str.begin() + tkp[i] + 1, str.end()));
				}
			}
			else
			{
				tokstr.push_back(string(str.begin() + tkp[i - 1] + 1, str.begin() + tkp[i]));
				tokstr.push_back(string(str.begin() + tkp[i], str.begin() + tkp[i] + 1));
			}
		}
		j = i;
	};

	//연결찾기
	for (int i = 0; i < tokstr.size() - 1; i++)
	{
		if ((tokstr[i] == ">" || tokstr[i] == "<") && tokstr[i + 1] == "=")
			//if ((tokstr[i] == "+" || tokstr[i] == "-" || tokstr[i] == "*" || tokstr[i] == "/" || tokstr[i] == ">" || tokstr[i] == "<" || tokstr[i] == "!") && tokstr[i + 1] == "=")
		{
			tokstr[i].append("=");
			tokstr.erase(tokstr.begin() + i + 1);
			i = 0;
		}
		else if (tokstr[i] == "&"  && tokstr[i + 1] == "&")
		{

			tokstr[i].append("&");
			tokstr.erase(tokstr.begin() + i + 1);
			i = 0;
		}
		else if (tokstr[i] == "|"  && tokstr[i + 1] == "|")
		{

			tokstr[i].append("|");
			tokstr.erase(tokstr.begin() + i + 1);
			i = 0;
		}
		else if (tokstr[i] == "/"  && tokstr[i + 1] == "/")
		{

			tokstr[i].append("/");
			tokstr.erase(tokstr.begin() + i + 1);
			i = 0;
		}
	}

	//빈칸 지우기
	for (int i = 0; i < tokstr.size(); i++)
	{
		if (tokstr[i] == " " || tokstr[i] == "\r" || tokstr[i] == "\t" || tokstr[i] == "")
		{
			tokstr.erase(tokstr.begin() + i);
		}
	}
	//연결찾기
	for (int i = 0; i < tokstr.size() - 1; i++)
	{
		if (tokstr[i] == "아니면")
		{
			if (tokstr[i + 1] == "또는")
			{
				tokstr[i] = "아니면 또는";
				tokstr.erase(tokstr.begin() + i + 1);
				i = 0;
			}
		}
	}

	//주석 지우기
	for (int i = 0; i < tokstr.size(); i++)
	{
		if (tokstr[i] == "//")
		{
			tokstr.erase(tokstr.begin() + i, tokstr.end());
			break;
		}
	}


	Token temptoken;
	//토큰화
	for (int i = 0; i < tokstr.size(); i++)
	{
		if (tokstr[i] == "참거짓" || tokstr[i] == "bool")
		{
			temptoken.TokenType = Bool;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "문자" || tokstr[i] == "char")
		{
			temptoken.TokenType = Char;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "아니면" || tokstr[i] == "else")
		{
			temptoken.TokenType = Else;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "거짓" || tokstr[i] == "false")
		{
			temptoken.TokenType = False;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "실수" || tokstr[i] == "float")
		{
			temptoken.TokenType = Float;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "만약" || tokstr[i] == "if")
		{
			temptoken.TokenType = If;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "아니면 또는" || tokstr[i] == "else if")
		{
			temptoken.TokenType = ElseIf;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "정수" || tokstr[i] == "int")
		{
			temptoken.TokenType = Int;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "메인" || tokstr[i] == "main")
		{
			temptoken.TokenType = Main;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "참" || tokstr[i] == "true")
		{
			temptoken.TokenType = True;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "반복" || tokstr[i] == "while")
		{
			temptoken.TokenType = While;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "{")
		{
			temptoken.TokenType = LeftBrace;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "}")
		{
			temptoken.TokenType = RightBrace;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "[")
		{
			temptoken.TokenType = LeftBracket;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "]")
		{
			temptoken.TokenType = RightBracket;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "(")
		{
			temptoken.TokenType = LeftParen;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == ")")
		{
			temptoken.TokenType = RightParen;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == ";")
		{
			temptoken.TokenType = Semicolon;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == ",")
		{
			temptoken.TokenType = Comma;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "=")
		{
			temptoken.TokenType = Assign;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "==")
		{
			temptoken.TokenType = Equals;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "<")
		{
			temptoken.TokenType = Less;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "<=")
		{
			temptoken.TokenType = LessEqual;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == ">")
		{
			temptoken.TokenType = Greater;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == ">=")
		{
			temptoken.TokenType = GreaterEqual;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "!")
		{
			temptoken.TokenType = Not;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "!=")
		{
			temptoken.TokenType = NotEqual;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "+")
		{
			temptoken.TokenType = Plus;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "-")
		{
			temptoken.TokenType = Minus;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "*")
		{
			temptoken.TokenType = Multiply;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "/")
		{
			temptoken.TokenType = Divide;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "&&")
		{
			temptoken.TokenType = And;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "||")
		{
			temptoken.TokenType = Or;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == ".")
		{
			temptoken.TokenType = Period;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "\'")
		{
			temptoken.TokenType = SingleQuotation;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "문자열" || tokstr[i] == "string")
		{
			temptoken.TokenType = (TokenType)String;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "\"")
		{
		temptoken.TokenType = DoubleQuotation;
		temptoken.TokenName = tokstr[i];
		temptoken.TokenValue = tokstr[i];
		}
		else
		{
			if (isDigit(tokstr[i]))
			{
				temptoken.TokenType = IntLiteral;
				temptoken.TokenName = tokstr[i];
				temptoken.TokenValue = tokstr[i];
			}
			else
			{
				temptoken.TokenType = Identifier;
				temptoken.TokenName = tokstr[i];
				temptoken.TokenValue = tokstr[i];
			}
		}
		vtoken.push_back(temptoken);
	}

	//잘못된 문자 검출
	for (auto i : vtoken)
	{
		if (i.TokenType == Identifier)
		{
			if (isThereIllegalChar(i.TokenName))
			{
				cout << "Identifier\t" << i.TokenName << endl;
				assert(false && "IllegalChar Detected");
			}
		}
	}


	//실수
	for (int i = 0; i < vtoken.size(); i++)
	{
		if (vtoken[i].TokenType == Period)
		{
			assert((i < vtoken.size() - 1 && i != 0) && "Lexer 오류 : <.> 토큰 오류");
			if (vtoken[i - 1].TokenType == IntLiteral && vtoken[i + 1].TokenType == IntLiteral)
			{
				vtoken[i].TokenName = vtoken[i - 1].TokenName + "." + vtoken[i + 1].TokenName;
				vtoken[i].TokenValue = vtoken[i - 1].TokenValue + "." + vtoken[i + 1].TokenValue;
				vtoken[i].TokenType = FloatLiteral;
				vtoken.erase(vtoken.begin() + i + 1);
				vtoken.erase(vtoken.begin() + i - 1);

				i = 0;
			}
		}
	}

	//문자
	for (int i = 0; i < vtoken.size(); i++)
	{
		if (i >= vtoken.size() - 3) break;
		if (vtoken[i].TokenType == SingleQuotation)
		{
			assert((vtoken[i + 2].TokenType == SingleQuotation) && "SingleQuotation 오류");
			Token temp;
			temp.TokenType = (TokenType)CharLiteral;
			temp.TokenName = vtoken[i + 1].TokenName;
			temp.TokenValue = vtoken[i + 1].TokenName;
			vtoken[i] = temp;
			vtoken.erase(vtoken.begin() + i + 1, vtoken.begin() + i + 3);
			i = 0;
		}
	}

	//문자열
	for (int i = 0; i < vtoken.size(); i++)
	{
		if (i >= vtoken.size() - 3) break;
		if (vtoken[i].TokenType == DoubleQuotation)
		{
			assert((vtoken[i + 2].TokenType == DoubleQuotation) && "DoubleQuotation 오류");
			Token temp;
			temp.TokenType = (TokenType)StringLiteral;
			temp.TokenName = vtoken[i + 1].TokenName;
			temp.TokenValue = vtoken[i + 1].TokenName;
			vtoken[i] = temp;
			vtoken.erase(vtoken.begin() + i + 1, vtoken.begin() + i + 3);
			i = 0;
		}
	}

	//선언&정의
	//bool secondTry = false;
	//for (int i = 0; i < vtoken.size(); i++)
	//{
	//	if (secondTry||
	//		(vtoken[i].TokenType == (TokenType)Int ||
	//		vtoken[i].TokenType == (TokenType)Char ||
	//		vtoken[i].TokenType == (TokenType)Bool ||
	//		vtoken[i].TokenType == (TokenType)Float))
	//	{
	//		if (vtoken[i + 1].TokenType == (TokenType)Identifier)
	//		{
	//			Token idtemp = vtoken[i + 1];
	//			if (vtoken[i + 2].TokenType == (TokenType)Assign)
	//			{
	//				Token atemp = vtoken[i + 2];
	//				if (vtoken[i + 3].TokenType == (TokenType)IntLiteral ||
	//					vtoken[i + 3].TokenType == (TokenType)FloatLiteral ||
	//					vtoken[i + 3].TokenType == (TokenType)CharLiteral)
	//				{
	//					Token literaltemp = vtoken[i + 3];
	//					vtoken.erase(vtoken.begin() + i+2 , vtoken.begin() + i + 4);
	//	
	//					vtoken.push_back(idtemp);
	//					vtoken.push_back(atemp);
	//					vtoken.push_back(literaltemp);
	//	
	//					Token temptoken;
	//					temptoken.TokenType = Semicolon;
	//					temptoken.TokenName = ";";
	//					temptoken.TokenValue = ";";
	//					vtoken.push_back(temptoken);
	//
	//					i -= 0;
	//					secondTry = true;
	//				}
	//			}
	//		}
	//	}
	//}

	//temptoken.TokenName = "Eol";
	//temptoken.TokenValue = "Eol";
	//temptoken.TokenType = Eol;
	//vtoken.push_back(temptoken);


}


bool lexer::isThereIllegalChar(string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if ((str[i] & 0x80) == 0x80) {}
		else if (('a' <= str[i] && str[i] <= 'z') || ('A' <= str[i] && str[i] <= 'Z')) {}
		else return true;
	}
	return false;
}

bool lexer::isDigit(char ch)
{
	return '0' <= ch && ch <= '9';
}
bool lexer::isDigit(string str)
{
	for (int i = 0; i < str.size(); i++)
	{
		if (!isDigit(str[i]))
		{
			return false;
		}
	}
	return true;
}


void lexer::makeToken()
{
	ifstream file;
	file.open(argv);
	int line = 1;
	while (!file.eof())
	{
		char temp[1000];
		file.getline(temp, 1000);
		tokenize(temp, _vToken);
	}
	file.close();


	//Token ttemp;
	//ttemp.TokenName = "Eof";
	//ttemp.TokenValue = "Eof";
	//ttemp.TokenType = Eof;
	//_vToken.push_back(ttemp);
	for (int i = 0; i < _vToken.size(); i++)
	{
		if (_vToken[i].TokenType == Eol || _vToken[i].TokenType == Eof)
		{
			_vToken.erase(_vToken.begin() + i);
			i = 0;
		}
	}
}

void lexer::showAllToken() {
	for (auto i : _vToken)
	{
		if (i.TokenType == Identifier)
		{
			cout << "Identifier\t" << i.TokenName << endl;
		}
		else if (i.TokenType == IntLiteral)
		{
			cout << "IntLiteral\t" << i.TokenName << endl;
		}
		else if (i.TokenType == CharLiteral)
		{
			cout << "CharLiteral\t" << i.TokenName << endl;
		}
		else if (i.TokenType == FloatLiteral)
		{
			cout << "FloatLiteral\t" << i.TokenName << endl;
		}
		else if (i.TokenType == Eol)
		{

		}
		else
		{
			cout << i.TokenName << endl;
		}
	}
}

int lexer::getNowTokenNum()
{
	return _nowTokenNum;
}

vector<Token> lexer::getTokenVector()
{
	return _vToken;
}

Token lexer::getNowToken()
{
	return _vToken[_nowTokenNum];
}
