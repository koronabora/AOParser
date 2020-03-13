
#pragma once

#include <iostream>
#include <string>
#include <Windows.h>
#include "DataParser.h"
#include "XlsxWriter.h"
#include "Logger.h"

#include <filesystem>
namespace fs = std::filesystem;
using namespace std;

class Worker
{
public:
	Worker();
	~Worker();

	void run(const wstring& sourceFolder);

private:
	DataParser _parser;
	XlsxWriter _writer;

	vector<Item> _items;

	wstring _combineFolders(const wstring& path, const wstring& folder);
};