#include "stdafx.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"
#include "TypeCheckr.h"
#include "CodeGenerator.h"
using namespace std;

int main()
{
	//test~
	ofstream test;
	test.open("tttt.txt");
	string strtest;
	strtest = "int main() { \n int a;\nchar b = \'A\';\n return 0;\n}\n";
	string strtest2 = strtest;
	strtest += strtest2;
	test << strtest << endl;
	test.close();
	cout << "WHATTTTTTTTTTTTTTTTTTTTTT" << endl;
	//~test
	char filename[] = "test.txt";

	//����
	lexer* _lexer = new lexer(filename);
	_lexer->makeToken();
	//_lexer->showAllToken();

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
	system("pause");
	return 0;
}
