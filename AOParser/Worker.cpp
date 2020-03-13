#include "Worker.h"

Worker::Worker()
{}

Worker::~Worker()
{}

void Worker::run(const wstring& sourceFolder)
{
	Logger::i().log(L"Working directory: " + sourceFolder);

	// scanning for an items
	const wstring itemFolder = _combineFolders(sourceFolder, ITEM_NAMES_FOLDER);
	//Logger::i().log(LINE_SEPARATOR);
	Logger::i().log(L"Searching for item data at folder: " + itemFolder);
	_parser.parseItems(itemFolder, _items);

	// scanning for loot data
	const wstring lootFolder = _combineFolders(sourceFolder, LOOT_TABLE_FOLDER);
	Logger::i().log(L"Searching for loot data at folder: " + lootFolder);
	_parser.parseBossData(lootFolder, _items);

	// save data
	_writer.saveData(_items, sourceFolder);
}

wstring Worker::_combineFolders(const wstring& path, const wstring& folder)
{
	wstring res = path;
	if (!(res.at(res.length() - 1) == DIR_SEPARATOR))
		res += DIR_SEPARATOR;
	res += folder;
	//if (!(res.at(res.length() - 1) == DIR_SEPARATOR))
		//res += DIR_SEPARATOR;
	return res;
}