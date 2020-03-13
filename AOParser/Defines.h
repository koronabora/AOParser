#pragma once

#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

// common
static const wchar_t FILE_NAME_DELIMITER = L'_';
static const wchar_t DIR_SEPARATOR = L'\\';

// Log file
static const wstring LINE_SEPARATOR = L"==========================================";
static const wstring LINE_SEPARATOR_THIN = L"------------------------------------------";
static const wstring LOG_FILE_NAME = L"AOParser.log";

// Items
static const wstring ITEM_NAMES_FOLDER = L"Item";
//static const wstring ITEM_NAME_FILES = "*name.txt";
static const wstring ITEM_NAME_FILES = L"(.*)name.txt$";

//static const string ITEM_DESCR_FILES = "*description.txt";
static const wstring ITEM_DESCR_FILES = L"(.*)description.txt$";
static const size_t ITEM_NAME_FILE_SKIP_FIRST_POSITIONS = 1; // count of position to skip while parsing item name from file name
static const size_t ITEM_NAME_FILE_SKIP_LAST_POSITIONS = 0; // count of position to skip while parsing item name from file name
static const size_t ITEM_NAME_FILE_SKIP_LAST_POSITIONS_V0 = 1; // count of position to skip while parsing item name from file name

// Loot tables
static const wstring LOOT_TABLE_FOLDER = L"Tier1";
//static const string LOOT_TABLE_FILES = "*_LootTable_Boss_*.xdb";
static const wstring LOOT_TABLE_FILES = L"(.*)_LootTable_Boss(.*).xdb$";
static const wstring LOOT_TABLE_FILES_ADD_FILTER = L"Prototype"; // additional filter to SKIP file
static const wstring LOOT_FILE_ROOT = L"gameMechanics.constructor.schemes.item.LootTableResource";
static const wstring LOOT_PROTOTYPE_FILE_ROOT = L"gameMechanics.constructor.schemes.item.LootTableResource";
static const wstring LOOT_FILE_MAIN_TABLE_VALUE = L"table";
static const wstring LOOT_FILE_HEADER_VALUE = L"Header";
static const wchar_t SPLITTER0 = L'#';
static const wchar_t SPLITTER1 = L'/';

// out file
static const string OUT_FILE = "loot.xlsx";
static const wstring OUT_TEXT_FILE = L"loot.txt";


