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

void lexer::tokenize(string str, vector<Token> &vtoken)
{
	if (str.size() == 0)
	{
		return;
	}
	vector<int> tkp;
	vector<string> tokstr;
	//strtok_s 함수 이용

	for (int i = 0; i < str.size(); i++)
	{
		if (str[i] == '+' || str[i] == '-' || str[i] == '*' || str[i] == '/' || str[i] == ',' || str[i] == '.' ||
			str[i] == ';' || str[i] == '{' || str[i] == '}' || str[i] == '(' || str[i] == ')' || str[i] == '&' ||
			str[i] == '|' || str[i] == '!' || str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '?' || str[i] == '='
			|| str[i] == '<' || str[i] == '>' || str[i] == '\'' || str[i] == '\"' || str[i] == '[' || str[i] == ']')
		{
			tkp.push_back(i);
		}
	}
	int j = 0;

	if (tkp.size() == 0)
	{
		tokstr.push_back(str);
	}
	else
	{
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
	}

	//연결찾기
	for (int i = 0; i < tokstr.size(); i++)
	{
		if (i >= tokstr.size() - 1) break;
		if ((tokstr[i] == ">" || tokstr[i] == "<" || tokstr[i] == "=") && tokstr[i + 1] == "=")
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
	for (int i = 0; i < tokstr.size(); i++)
	{
		if (tokstr[i] == "아니면")
		{
			if (i != tokstr.size() - 1)
			{
				if (tokstr[i + 1] == "또는")
				{
					tokstr[i] = "아니면 또는";
					tokstr.erase(tokstr.begin() + i + 1);
					i = 0;
				}
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
		else if (tokstr[i] == "차원")
		{
			temptoken.TokenType = (TokenType)Array;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "입력")
		{
			temptoken.TokenType = (TokenType)CommandInput;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "출력")
		{
			temptoken.TokenType = (TokenType)CommandOutput;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "넣기")
		{
			temptoken.TokenType = (TokenType)CommandArrPushBack;
			temptoken.TokenName = tokstr[i];
			temptoken.TokenValue = tokstr[i];
		}
		else if (tokstr[i] == "뽑기")
		{
			temptoken.TokenType = (TokenType)CommandArrPop;
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


}

void lexer::tokenCheck(vector<Token>& vtoken)
{
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

	//배열 선언 체크
	for (int i = 0; i < vtoken.size(); i++)
	{
		if (vtoken[i].TokenType == Identifier)
		{
			if (isItArrayDeclare(vtoken[i].TokenName))
			{
				vtoken[i].TokenType = (TokenType)Array;
			}
		}
	}


	//배열 인자 체크
	//for (int i = 0; i < vtoken.size(); i++)
	//{
	//	if (vtoken[i].TokenType == LeftBracket)
	//	{
	//		//
	//		if (i < 1) assert(false && "Worng used LeftBracket (Should be Identifier first)");
	//		if (i == vtoken.size() - 2) assert(false && "Worng used LeftBracket (Should be IntLiteral)");
	//		if (i == vtoken.size() - 1) assert(false && "Worng used LeftBracket (Should be RightBracket)");
	//		if (vtoken[i - 1].TokenType == (TokenType)Identifier && vtoken[i + 1].TokenType == IntLiteral && vtoken[i + 2].TokenType == RightBracket)
	//		{
	//			vtoken[i - 1].TokenName = vtoken[i - 1].TokenName + vtoken[i].TokenName + vtoken[i + 1].TokenName + vtoken[i + 2].TokenName;
	//			vtoken[i - 1].TokenValue = vtoken[i - 1].TokenValue + vtoken[i].TokenValue + vtoken[i + 1].TokenValue + vtoken[i + 2].TokenValue;
	//			vtoken.erase(vtoken.begin() + i, vtoken.begin() + i + 3);
	//			i = 0;
	//		}
	//	}
	//}

	//변수 내 잘못된 문자 검출
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
}


bool lexer::isThereIllegalChar(string str)
{
	if ('0' <= str[0] && str[0] <= '9')
	{
		return true;
	}
	for (int i = 1; i < str.size(); i++)
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

bool lexer::isItArrayDeclare(string str)
{
	int arrNum = 0;
	if (!('1' <= str[0] && str[0] <= '9'))
	{
		return false;
	}
	for (int i = 0; i < str.size(); i++)
	{
		if ('0' <= str[i] && str[i] <= '9')
		{
			arrNum = i;
		}
		else
		{
			break;
		}
	}
	if (str.substr(arrNum + 1, str.size()) == "차원")
	{
		return true;
	}
	return false;
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
	tokenCheck(_vToken);
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
		else if (i.TokenType == StringLiteral)
		{
			cout << "StringLiteral\t" << i.TokenName << endl;
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
