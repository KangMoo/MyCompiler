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

	//����
	lexer* _lexer = new lexer(filename);
	_lexer->makeToken();
	_lexer->showAllToken();

	//�ļ�
	parser* _parser = new parser();
	_parser->setTokenVector(_lexer->getTokenVector());
	_parser->parse();

	//AST
	AST* _ast = new AST();
	_ast->setBlock(_parser->getBlock());
	_ast->display();

	//Ÿ��üĿ
	TypeCheckr* _tc = new TypeCheckr();
	_tc->typeCheck((Statement*)_parser->getBlock(),0);
	//_tc->showTypeMap();

	//�ڵ����
	CodeGenerator* _cg = new CodeGenerator();
	_cg->setFileName("generatedCode.txt");
	_cg->setBodyBlock(_parser->getBlock());
	_cg->mkCode();

	//_cg->setFileName("generatedCode.cpp");
	//_cg->setBodyBlock(_parser->getBlock());
	//_cg->mkCode(true);

	//string st;
	//cout << "�ڵ尡 �����Ǿ����ϴ�. �����Ű���� y�� �Է��Ͻʽÿ�." << endl;
	//cin >> st;


	//if (st == "y")
	//{
	//	generatedCode* _gc = new generatedCode();
	//	_gc->main();
	//}


	system("pause");
	return 0;
}
