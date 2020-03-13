#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <locale>
#include <codecvt>
#include <Windows.h>

using namespace std;

static const wchar_t WENDL = L'\n';

class StringHelper
{
public:
	static vector<string> split(const string& s, char delimiter);
	static vector<wstring> split(const wstring& s, wchar_t delimiter);
	static StringHelper& i();
	//wstring sToW(const string& input);
private:
	StringHelper();
	~StringHelper();

	static StringHelper* _instance;
};
