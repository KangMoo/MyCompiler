#include "stdafx.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"
#include "TypeCheckr.h"
#include "CodeGenerator.h"

#include <Windows.h>
#include <sstream>
//#include "generatedCode.h"
using namespace std;

int main()
{
	char filename[] = "test.txt";

	//렉서
	lexer* _lexer = new lexer(filename);
	_lexer->makeToken();
	_lexer->showAllToken();

	//파서
	parser* _parser = new parser();
	_parser->setTokenVector(_lexer->getTokenVector());
	_parser->parse();

	//AST
	AST* _ast = new AST();
	_ast->setBlock(_parser->getBlock());
	_ast->display();

	//타입체커
	TypeCheckr* _tc = new TypeCheckr();
	_tc->typeCheck((Statement*)_parser->getBlock(),0);
	//_tc->showTypeMap();

	//코드생성
	CodeGenerator* _cg = new CodeGenerator();
	_cg->setFileName("generatedCode.txt");
	_cg->setBodyBlock(_parser->getBlock());
	_cg->mkCode();

	//_cg->setFileName("generatedCode.cpp");
	//_cg->setBodyBlock(_parser->getBlock());
	//_cg->mkCode(true);

	//string st;
	//cout << "코드가 생성되었습니다. 실행시키려면 y를 입력하십시오." << endl;
	//cin >> st;


	//if (st == "y")
	//{
	//	generatedCode* _gc = new generatedCode();
	//	_gc->main();
	//}


	system("pause");
	return 0;
}
