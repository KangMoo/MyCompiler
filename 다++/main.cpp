#include "stdafx.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"
#include "TypeCheckr.h"
#include "CodeGenerator.h"
using namespace std;

int main()
{
	char filename[] = "test.txt";

	//렉서
	lexer* _lexer = new lexer(filename);
	_lexer->makeToken();
	//_lexer->showAllToken();

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
	system("pause");
	return 0;
}
