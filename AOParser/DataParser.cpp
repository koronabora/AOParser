#include "DataParser.h"

DataParser::DataParser()
{

}

DataParser::~DataParser()
{

}

void DataParser::parseBossData(const wstring& path, vector<Item>& items)
{
	// load names
	_lootDirectory = path;
	const vector<wstring> bossFiles = _scanDirectory(path, LOOT_TABLE_FILES);
	// additional filter
	vector<wstring> filteredBossFiles;
	for (const wstring& s : bossFiles)
		if (s.find(LOOT_TABLE_FILES_ADD_FILTER) == string::npos)
			filteredBossFiles.push_back(s);
	// work
	_workWithFiles(filteredBossFiles, items);
}

void DataParser::parseItems(const wstring& path, vector<Item>& items)
{
	// load names, every item should be uniq
	const vector<wstring> nameFiles = _scanDirectory(path, ITEM_NAME_FILES);
	for (const wstring& fileName : nameFiles)
	{

		wstring itemId = _getItemIdFromFileName0(fileName);
		wstring fileData;
		if (!_readSimpleText(fileName, fileData))
		{
			Logger::i().log(L"Could not read file data from [" + fileName + L"]");
		}
		else
		{
			bool is_uniq = true;
			for (const Item& i : items)
				if (i.id == itemId)
				{
					is_uniq = false;
					break;
				}
			if (!is_uniq)
				Logger::i().log(L"(!) Item, parsed from file [" + fileName + L"] is not uniq! Skipped! [Loading_names]");
			else
			{
				Item i;
				i.id = itemId;
				i.name = fileData;
				items.push_back(i);
			}
		}
	}

	// load descriptions, no uniq items accepted
	const vector<wstring> descrFiles = _scanDirectory(path, ITEM_DESCR_FILES);
	for (const wstring& fileName : nameFiles)
	{
		fs::path p(fileName);
		const wstring foo = p.filename().wstring();
		wstring itemId = _getItemIdFromFileName0(foo);
		wstring fileData;
		if (!_readSimpleText(fileName, fileData))
		{
			Logger::i().log(L"Could not read file data from [" + fileName + L"]");
		}
		else
		{
			bool found = false;
			for (Item& i : items)
				if (i.id == itemId)
				{
					i.descr = fileData;
					found = true;
					break;
				}
			if (!found) // !
				Logger::i().log(L"(!) Item, parsed from file [" + fileName + L"] is uniq! Skipped! [Loading_descriptions]");
		}
	}
}

bool DataParser::_readSimpleText(const wstring& fileName, wstring& data)
{
	bool res = false;
	wifstream f;
	f.open(fileName, wifstream::in);
	f.imbue(locale(f.getloc(), new codecvt_utf16<wchar_t, 0x10ffff, little_endian>));
	if (!f.is_open())
		Logger::i().log(L"Could not open file: " + fileName);
	else
	{
		getline(f, data);
		if (!data.empty())
			res = true;
	}

	

	return res;
}

vector<wstring> DataParser::_scanDirectory(const wstring& path, const wstring& filter)
{
#ifdef _DEBUG
	Logger::i().log(L"Scanning directory: [" + path + L"] using file mask: <" + filter + L">");
#endif
	vector<wstring> res;
	try
	{
		if (fs::exists(path) && fs::is_directory(path))
		{
			fs::recursive_directory_iterator iterator(path);
			fs::recursive_directory_iterator end;
			wregex regex(filter);
			wsmatch base_match;
			//for (const auto& p : fs::directory_iterator(path))
			while (iterator != end)
			{
				if (fs::is_directory(iterator->path()))
					iterator.disable_recursion_pending();
				else
				{
					const wstring fullPath = iterator->path().wstring();
					const wstring fileName = iterator->path().filename().wstring();
					if (regex_match(fileName, base_match, regex))
						res.push_back(fullPath);
				}

				error_code ec;
				iterator.increment(ec);
				if (ec)
					Logger::i().log(L"(!) Directory scan error: " + iterator->path().wstring() + L" -> ");
			}
		}
	}
	catch (system_error&)
	{
		Logger::i().log(L"(!) Exception -> ");// +string(e.what()));
	}
#ifdef _DEBUG
	Logger::i().log(L"Found " + to_wstring(res.size()) + L" files");
#endif
	return res;
}

bool DataParser::_loadXmlFile(const wstring& file, xml_document& doc)
{
	xml_parse_result result = doc.load_file(file.c_str());

	if (result)
	{
#ifdef _DEBUG
		Logger::i().log(L"XML [" + file + L"] parsed without errors, attr value: [" + doc.child(L"node").attribute(L"attr").value() + L"]");
#endif
	}
	else
	{
		Logger::i().log(L"XML [" + file + L"] parsed with errors, attr value: [" + doc.child(L"node").attribute(L"attr").value() + L"]");
		//Logger::i().log(L"Error description: " + result.description() + L"\n");
		Logger::i().log(L"Error offset: " + to_wstring(result.offset) + L" (error at [..." + to_wstring(result.offset) + L"]");
	}
	return result;
}

bool DataParser::_detectWeakFile(const xml_document& doc)
{
	bool res = false;
	const xml_node root = doc.child(LOOT_FILE_ROOT.c_str()).child(LOOT_FILE_MAIN_TABLE_VALUE.c_str());
	return root.empty();
}

vector<double> DataParser::_loadChancesFromNode(const xml_node& node)
{
	vector<double> res;
	for (xml_node_iterator it = node.begin(); it != node.end(); ++it)
	{
		if (!it->empty())
		{
			for (xml_node_iterator it2 = it->begin(); it2 != it->end(); ++it2)
			{
				//xml_node foo = it->child("Item");
				const wstring v = it2->value();// foo.value();
				if (!v.empty())
				{
					double chance = stod(v);
					res.push_back(chance);
				}
			}
		}
	}
	/*#ifdef _DEBUG
		cout << "[";
		for (const double& s : res)
			cout << s << ", ";
		cout << "]" << endl;
	#endif*/
	return res;
}

wstring DataParser::_parseRawItemName(const wstring& v)
{
	// /Items/HeroicInstancesReward/Tier1/HeroicReward_MetaPlateBelt45MagHealer_1.xdb#xpointer(/gameMechanics.constructor.schemes.item.ItemResource)
	wstring res;
	// split using '#'
	const vector<wstring> foo = StringHelper::i().split(v, SPLITTER0);
	if (!foo.empty())
	{
		// split using '/'
		const vector<wstring> bar = StringHelper::i().split(foo.at(0), SPLITTER1);
		if (!bar.empty())
			res = _getItemIdFromFileName(bar.at(bar.size() - 1));
	}
	return res;
}

bool DataParser::_loadItem(const xml_node& node, vector<Item>& items, const double& baseChance, const wstring& bossId)
{
	/*<Item type="gameMechanics.constructor.schemes.item.LootTableSingleItem">
		<itemProvider type="gameMechanics.constructor.schemes.item.meta.MetaItemResourceProvider">
		<item href="/Items/HeroicInstancesReward/Tier1/HeroicReward_MetaPlateBelt45MagHealer_1.xdb#xpointer(/gameMechanics.constructor.schemes.item.ItemResource)" />
		<stateChances>
			<Item>0</Item>
			<Item>76</Item>
			<Item>21</Item>
			<Item>3</Item>
		</stateChances>
		</itemProvider>
		<minNumber>1</minNumber>
		<maxNumber>1</maxNumber>
		<isForAll>true</isForAll>
	</Item>*/
	bool res = true;
	if (node.empty())
		res = false;
	else
	{
		const xml_node item = node.child(L"item");
		const wstring rawName = item.attribute(L"href").value();
		const wstring id = _parseRawItemName(rawName);

		const xml_node stateChances = node.child(L"stateChances");
		const vector<double> stateChanceValues = _loadChancesFromNode(stateChances);
		if (stateChanceValues.empty())
			Logger::i().log(L"(!) Could not parse <stateChances> for item: " + rawName);

		if (id.empty())
		{
			Logger::i().log(L"(!) Could not parse item id from stirng: " + rawName);
			res = false;
		}
		else
			for (Item& i : items)
				if (i.id == id)
				{
					BossData bd;
					bd.id = bossId;
					bd.baseChance = baseChance;
					bd.chances = stateChanceValues;
					i.bossData.push_back(bd);

					break;
				}
	}
	return res;
}

bool DataParser::_parsePrototypeFile(const wstring& fileName, vector<Item>& items, const vector<double> baseChances, const wstring& bossId)
{
	bool res = true;
	xml_document doc;
	const wstring p = _lootDirectory + DIR_SEPARATOR + fileName;
	if (!_loadXmlFile(p, doc))
	{
		Logger::i().log(L"XML loading error occured at file: " + fileName);
	}
	else
	{
		const xml_node root = doc.child(LOOT_PROTOTYPE_FILE_ROOT.c_str());
		if (root.empty())
			Logger::i().log(L"XML parsing error occured at file: " + fileName);
		else
		{
			// ingoring <chanceX> insyde header
			const xml_node table = root.child(L"table");
			const xml_node topEntries = table.child(L"entries");
			const xml_node topChances = table.child(L"chances");
			vector<double> topChanceValues = _loadChancesFromNode(topChances);

			size_t topChanceId = 0;
			for (xml_node_iterator it0 = topEntries.begin(); it0 != topEntries.end(); ++it0)
			{
				if (topChanceId >= topChanceValues.size())
				{
					Logger::i().log(L"Top chances and items count mismatch! Aborting...");
					break;
				}
				else
				{
					// load items at current group
					const xml_node chances = it0->child(L"chances");
					vector<double> chanceValues = _loadChancesFromNode(chances);

					const xml_node entries = it0->child(L"entries");
					size_t chanceId = 0;
					for (xml_node_iterator it1 = entries.begin(); it1 != entries.end(); ++it1)
					{
						if (chanceId >= chanceValues.size())
						{
							Logger::i().log(L"Chances and items count mismatch! Aborting...");
							break;
						}
						else
						{
							const xml_node finalChances = it1->child(L"chances");
							//vector<double> finalChanceValues = _loadChancesFromNode(finalChances);
							// read vector
							vector<wstring> finalChanceNameValues;
							for (xml_node_iterator it3 = finalChances.begin(); it3 != finalChances.end(); ++it3)
							{
								if (!it3->empty())
								{
									/*for (xml_node_iterator it2 = it->begin(); it2 != it->end(); ++it2)
									{
										//xml_node foo = it->child("Item");
										const string v = it2->value();// foo.value();
										if (!v.empty())
										{
											double chance = stod(v);
											res.push_back(chance);
										}
									}*/
									xml_attribute att = it3->attribute(L"var");
									finalChanceNameValues.push_back(att.value());
								}
							}
							vector<double> finalChanceValues;
							for (const wstring& s : finalChanceNameValues)
							{
								if (!s.empty())
								{
									const size_t bar = s.at(s.size() - 1) - '1';
									if (bar <= baseChances.size())
										finalChanceValues.push_back(baseChances.at(bar));
								}
							}

							const xml_node finalEntries = it1->child(L"entries");
							size_t finalChanceId = 0;
							for (xml_node_iterator it2 = finalEntries.begin(); it2 != finalEntries.end(); ++it2)
							{
								if (finalChanceId >= finalChanceValues.size())
								{
									Logger::i().log(L"Final chances and items count mismatch! Aborting...");
									break;
								}
								else
								{
									const xml_node curItem = it2->child(L"itemProvider");
									const double curChance = finalChanceValues.at(finalChanceId) * chanceValues.at(chanceId) * topChanceValues.at(topChanceId);
									if (!_loadItem(curItem, items, curChance, bossId))
										Logger::i().log(L" (!) Item skipped!");
								}
								finalChanceId++;
							}
						}
						chanceId++;
					}
				}
				topChanceId++;
			}
		}
	}
	return res;
}

bool DataParser::_parseXmlFile(const xml_document& doc, vector<Item>& items, const wstring& bossId)
{
	bool res = true;
	const xml_node root = doc.child(LOOT_FILE_ROOT.c_str());

	if (_detectWeakFile(doc))
	{
		// load href
		const xml_node header = root.child(L"Header");
		const xml_node href = header.child(L"Prototype");
		const xml_attribute href2 = href.attribute(L"href");

		const xml_node variables = header.child(L"Variables");
		vector<double> vars = _loadChancesFromNode(variables);

		const vector<wstring> foo = StringHelper::i().split(href2.value(), L'#');
		if (foo.size() == 0 || !_parsePrototypeFile(foo.at(0), items, vars, bossId))
			Logger::i().log(L" (!) An error occured while parsing external file");
	}
	else
	{
		// load all file
		const xml_node table = root.child(L"table");
		const xml_node topEntries = table.child(L"entries");
		const xml_node topChances = table.child(L"chances");
		vector<double> topChanceValues = _loadChancesFromNode(topChances);

		size_t topChanceId = 0;
		for (xml_node_iterator it0 = topEntries.begin(); it0 != topEntries.end(); ++it0)
		{
			if (topChanceId >= topChanceValues.size())
			{
				Logger::i().log(L"Top chances and items count mismatch! Aborting...");
				break;
			}
			else
			{
				// load items at current group
				const xml_node chances = it0->child(L"chances");
				vector<double> chanceValues = _loadChancesFromNode(chances);

				const xml_node entries = it0->child(L"entries");
				size_t chanceId = 0;
				for (xml_node_iterator it1 = entries.begin(); it1 != entries.end(); ++it1)
				{
					if (chanceId >= chanceValues.size())
					{
						Logger::i().log(L"Chances and items count mismatch! Aborting...");
						break;
					}
					else
					{
						const xml_node finalChances = it1->child(L"chances");
						vector<double> finalChanceValues = _loadChancesFromNode(finalChances);

						const xml_node finalEntries = it1->child(L"entries");
						size_t finalChanceId = 0;
						for (xml_node_iterator it2 = finalEntries.begin(); it2 != finalEntries.end(); ++it2)
						{
							if (finalChanceId >= finalChanceValues.size())
							{
								Logger::i().log(L"Final chances and items count mismatch! Aborting...");
								break;
							}
							else
							{
								const xml_node curItem = it2->child(L"itemProvider");
								const double curChance = finalChanceValues.at(finalChanceId) * chanceValues.at(chanceId) * topChanceValues.at(topChanceId);
								if (!_loadItem(curItem, items, curChance, bossId))
									Logger::i().log(L" (!) Item skipped!");
							}
							finalChanceId++;
						}
					}
					chanceId++;
				}
			}
			topChanceId++;
		}
	}
	return res;
}

wstring DataParser::_getItemIdFromFileName(const wstring& fileName)
{
	wstring res;
	vector<wstring> parts0 = StringHelper::split(fileName, L'.');
	if (!parts0.empty())
	{
		vector<wstring> parts1 = StringHelper::split(parts0.at(0), FILE_NAME_DELIMITER);
		const size_t lastPos = parts1.size() - ITEM_NAME_FILE_SKIP_LAST_POSITIONS;
		for (size_t i = ITEM_NAME_FILE_SKIP_FIRST_POSITIONS; i < lastPos; i++)
		{
			res += parts1.at(i);
			if (i < lastPos - 1)
				res += FILE_NAME_DELIMITER;
		}
	}
	return res;
}


wstring DataParser::_getItemIdFromFileName0(const wstring& fileName)
{
	wstring res;
	vector<wstring> parts0 = StringHelper::split(fileName, L'.');
	if (!parts0.empty())
	{
		vector<wstring> parts1 = StringHelper::split(parts0.at(0), FILE_NAME_DELIMITER);
		const size_t lastPos = parts1.size() - ITEM_NAME_FILE_SKIP_LAST_POSITIONS_V0;
		for (size_t i = ITEM_NAME_FILE_SKIP_FIRST_POSITIONS; i < lastPos; i++)
		{
			res += parts1.at(i);
			if (i < lastPos - 1)
				res += FILE_NAME_DELIMITER;
		}
	}
	return res;
}

wstring DataParser::_parseRawBossName(const wstring& v)
{
	wstring res;

	if (!v.empty())
	{
		// split using '/'
		const vector<wstring> bar = StringHelper::i().split(v, DIR_SEPARATOR);
		if (!bar.empty())
		{

			/*// split using '_'
			const vector<wstring> foo = StringHelper::i().split(bar.at(bar.size() - 1), FILE_NAME_DELIMITER);
			if (!foo.empty())
			{
				wstring buf = foo.at(foo.size() - 1);
				if (v.find(L"UncmChested") != string::npos && foo.size() > 1)
					buf = foo.at(foo.size() - 2) + buf;
					*/
					//split using '.'
					//const vector<wstring> zoo = StringHelper::i().split(buf, L'.');
			const vector<wstring> zoo = StringHelper::i().split(bar.at(bar.size() - 1), L'.');
			if (!zoo.empty())
			{
				res = zoo.at(0);
#ifdef _DEBUG
				wcout << L"Parsed boss name: " << v << L" -> " << res << endl;
#endif
			}
			//	}
		}
	}
	return res;
}

void DataParser::_workWithFiles(const vector<wstring>& files, vector<Item>& items)
{
	for (const wstring& f : files)
	{
#ifdef _DEBUG
		Logger::i().log(L"Loading an xml object from file: " + f);
#endif
		xml_document doc;
		if (!_loadXmlFile(f, doc))
		{
			Logger::i().log(L"XML loading error occured at file: " + f);
		}
		else
		{
			const wstring bossId = _parseRawBossName(f);
			if (!_parseXmlFile(doc, items, bossId))
			{
				Logger::i().log(L"XML parsing error occured at file: " + f);
			}
			else
			{
#ifdef _DEBUG
				Logger::i().log(L"ok!");
#endif
			}
		}
	}
}
