#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <locale>
#include <codecvt>
#include <regex>
#include <filesystem>
#include <Windows.h>
namespace fs = std::filesystem;

#include "Item.h"
#include "Logger.h"
#include "StringHelper.h"
#include <pugixml.hpp>


using namespace std;
using namespace pugi;

class DataParser
{
public:
	DataParser();
	~DataParser();

	void parseBossData(const wstring& path, vector<Item>& items);
	void parseItems(const wstring& path, vector<Item>& items);

private:
	vector<wstring> _scanDirectory(const wstring& path, const wstring& filter);
	void _workWithFiles(const vector<wstring>& files, vector<Item>& items);
	bool _loadXmlFile(const wstring& file, xml_document& doc);
	bool _parseXmlFile(const xml_document& doc, vector<Item>& items, const wstring& bossId);
	bool _readSimpleText(const wstring& fileName, wstring& data);
	wstring _getItemIdFromFileName(const wstring& fileName);
	wstring _getItemIdFromFileName0(const wstring& fileName);
	bool _detectWeakFile(const xml_document& doc);
	vector<double> _loadChancesFromNode(const xml_node& node);
	bool _loadItem(const xml_node& node, vector<Item>& items, const double& baseChance, const wstring& bossId);
	wstring _parseRawItemName(const wstring& v);
	wstring _parseRawBossName(const wstring& v);
	bool _parsePrototypeFile(const wstring& fileName, vector<Item>& items, const vector<double> baseChances, const wstring& bossId);
	wstring _lootDirectory;
};
