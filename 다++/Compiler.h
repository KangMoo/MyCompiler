#pragma once
#include "stdafx.h"
#include "lexer.h"
#include "parser.h"
#include "AST.h"
#include "TypeCheckr.h"
#include "CodeGenerator.h"
#include <Windows.h> 
#include <direct.h>
class Compiler
{
private:
	char _import_filename[1024];
	char _export_filename[1024];
	char _inputCommand[1024];
public:
	void init(char* import_filename);
	void run();
	char* getExport_filename() { return _export_filename; };
	Compiler();
	~Compiler();
};

