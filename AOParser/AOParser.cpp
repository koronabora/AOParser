// AOParser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <iostream>
#include <filesystem>
#include <Windows.h>
#include "Worker.h"

using namespace std;

int main(int argc, char** argv)
{

	setlocale(LC_ALL, "");
	setlocale(LC_NUMERIC, "en_US.UTF-8");

	const fs::path path = argc >= 2 ? argv[1] : fs::current_path();

	cout << "Welcome to AOParser utility!" << endl;
	Worker w;
	w.run(path.wstring());

	//getchar();
	return 0;
};