#include "Compiler.h"


void Compiler::init(char* import_filename)
{
	strcpy_s(_import_filename, import_filename);
}

void Compiler::run()
{
	//렉서
	lexer* _lexer = new lexer(_import_filename);
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
	_tc->typeCheck((Statement*)_parser->getBlock());
	//_tc->showTypeMap();

	cout << endl << endl;
	cout << "**********************************************************************" << endl;
	cout << "** 정상적으로 코드가 생성되었습니다." << endl;
	cout << "** 내보낼 파일 이름을 입력하십시오." << endl;
	cout << "**********************************************************************" << endl;
	cin.getline(_export_filename, 1024);
	system("cls");
	//코드생성
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
