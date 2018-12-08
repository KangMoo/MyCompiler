#include "Compiler.h"


void Compiler::init(char* import_filename)
{
	strcpy_s(_import_filename, import_filename);
}

void Compiler::run()
{
	//����
	lexer* _lexer = new lexer(_import_filename);
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
	_tc->typeCheck((Statement*)_parser->getBlock());
	//_tc->showTypeMap();

	cout << endl << endl;
	cout << "**********************************************************************" << endl;
	cout << "** ���������� �ڵ尡 �����Ǿ����ϴ�." << endl;
	cout << "** ������ ���� �̸��� �Է��Ͻʽÿ�." << endl;
	cout << "**********************************************************************" << endl;
	cin.getline(_export_filename, 1024);
	system("cls");
	//�ڵ����
	CodeGenerator* _cg = new CodeGenerator();
	_cg->setFileName(_export_filename);
	_cg->setBodyBlock(_parser->getBlock());
	_cg->mkCode();
}

Compiler::Compiler()
{
}


Compiler::~Compiler()
{
}
