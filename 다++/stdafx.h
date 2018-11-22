#pragma once
#include "TokenType.h"
//#include "Token.h"
#include <string>
#include <iostream>
#include <assert.h>
#include <vector>
#include <fstream>

using namespace std;

typedef struct TOKEN {
	TokenType TokenType;
	string TokenName;
	string TokenValue;
}Token;

#define ARRAY_LEN(X) (sizeof(X)/sizeof(X[0]))