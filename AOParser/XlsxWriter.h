#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <Windows.h>
#include "StringHelper.h"
#include <Xlsx/Workbook.h>
#include "Item.h"
#include "Defines.h"
#include "Logger.h"

using namespace std;
using namespace SimpleXlsx;

class XlsxWriter
{
public:
	XlsxWriter();
	~XlsxWriter();

	void saveData(const vector<Item>& items, const wstring& folder);

private:

};
