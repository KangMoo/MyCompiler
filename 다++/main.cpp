#include "stdafx.h"
#include "Compiler.h"
#include <Windows.h> 
#include <direct.h>
//#include "generatedCode.h"
using namespace std;

int main()
{
	Compiler* _compiler = new Compiler();
	char import_filename[1024];
	char _export_filename[1024];
	int input1 = 0, input2 = 0;
	char command[1024] = "";
	char curDir[1024];
	while (true)
	{
		cout << endl << endl;
		cout << "**********************************************************************" << endl;
		cout << "** 실행할 명령을 선택해주세요." << endl;
		cout << "** 1. 파일 컴파일" << endl;
		cout << "** 2. 종료" << endl;
		cout << "**********************************************************************" << endl;
		cin >> input1;
		cin.ignore();
		system("cls");
		if (input1 == 1)
		{
			cout << endl << endl;
			cout << "**********************************************************************" << endl;
			cout << "** 불러올 파일 이름을 입력하십시오." << endl;
			cout << "**********************************************************************" << endl;
			cin.getline(import_filename, 1024);
			system("cls");
			_compiler->init(import_filename);
			_compiler->run();

			cout << endl << endl;
			cout << "**********************************************************************" << endl;
			cout << "** 실행할 명령을 선택해주세요." << endl;
			cout << "**********************************************************************" << endl;
			cout << "** 1. 실행 파일 생성" << endl;
			cout << "** 2. 다른 파일 컴파일" << endl;
			cout << "** 3. 종료" << endl;
			cout << "**********************************************************************" << endl;
			cin >> input2;
			cin.ignore();
			system("cls");
			if (input2 == 1)
			{
				cout << endl << endl << endl;
				//컴파일
				strcpy_s(_export_filename, _compiler->getExport_filename());
				_getcwd(curDir, 1024);
				strcpy_s(command, "");
				strcat_s(command, "\"C:\\Program Files (x86)\\Microsoft Visual Studio\\2017\\Community\\Common7\\Tools\\VsDevCmd.bat\"");
				strcat_s(command, "&& cd ");
				strcat_s(command, curDir);
				strcat_s(command, "&& cl /EHsc ");
				strcat_s(command, _export_filename);
				system(command);

				cout << endl << endl;
				cout << "**********************************************************************" << endl;
				cout << "** 실행 파일이 생성되었습니다. 실행하시겠습니까? ( Input 'Y' To Run)" << endl;
				cout << "**********************************************************************" << endl;
				string temp;
				getline(cin, temp);
				system("cls");
				if (temp == "Y" || temp == "y")
				{
					//exe파일 실행
					strcpy_s(_export_filename, _compiler->getExport_filename());
					for (int i = 0; i < strlen(_export_filename); i++)
					{
						if (_export_filename[i] == '.')
						{
							_export_filename[++i] = 'e';
							_export_filename[++i] = 'x';
							_export_filename[++i] = 'e';
							_export_filename[++i] = '\n';
							break;
						}
					}
					strcpy_s(command, "");
					strcat_s(command, curDir);
					strcat_s(command, "\\");
					strcat_s(command, _export_filename);

					cout << endl << endl << endl;
					cout << "**********************************************************************" << endl;
					cout << "** 프로그램 시작" << endl;
					cout << "**********************************************************************" << endl;
					system(command);
					cout << endl << endl << endl;
					cout << "**********************************************************************" << endl;
					cout << "** 프로그램 종료" << endl;
					cout << "** 계속 진행하려면 아무 키나 누르세요 (Input anykey to progress)" << endl;
					cout << "**********************************************************************" << endl;
					getline(cin, temp);
					system("cls");
				}
				else
				{

				}
			}
			else if (input2 == 2)
			{
				continue;
			}
			else if (input2 == 3)
			{
				break;
			}
		}
		else if (input1 == 2)
		{
			break;
		}

	}
	return 0;
}
