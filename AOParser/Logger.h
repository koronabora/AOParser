#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>

#include "Defines.h"
#include "StringHelper.h"

using namespace std;

class Logger
{
public:
	//static void log(const string& s);
	static void log(const wstring& s);

	static Logger& i();
private:
	Logger();
	~Logger();

	static Logger* _instance;
	static wofstream _logFile;
};
