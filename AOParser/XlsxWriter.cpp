#include "XlsxWriter.h"

XlsxWriter::XlsxWriter()
{

}

XlsxWriter::~XlsxWriter()
{
}

void XlsxWriter::saveData(const vector<Item>& items, const wstring& folder)
{
	const wstring file0 = folder + DIR_SEPARATOR + OUT_TEXT_FILE;
	const string file1 = OUT_FILE;

	vector<wstring> bossNames;

	wofstream oFile0;
	oFile0.open(file0, ios::out);
	if (oFile0.is_open())
	{
		for (const Item& i : items)
		{

			oFile0 << LINE_SEPARATOR << WENDL;
			oFile0 << L"Id: " << i.id << WENDL;
			oFile0 << L"Name: " << i.name << WENDL;
			oFile0 << L"Description: " << i.descr << WENDL;
			for (const BossData& b : i.bossData)
			{
				oFile0 << LINE_SEPARATOR_THIN << WENDL;
				oFile0 << L"Boss: " << b.id << WENDL;
				oFile0 << L"Chance: " << b.baseChance * 100 << "%" << WENDL;
				oFile0 << L"[";
				for (const double& d : b.chances)
					oFile0 << d << L", ";
				oFile0 << L"]" << endl;

				bool isBossNaeUniq = true;
				for (const wstring& s : bossNames)
					if (b.id == s)
					{
						isBossNaeUniq = false;
						break;
					}
				if (isBossNaeUniq)
					bossNames.push_back(b.id);
			}
		}
	}
	oFile0.close();

	// =================

	CWorkbook book("Results");
	//vector<ColumnWidth> ColWidth;
	//ColWidth.push_back(ColumnWidth(0, 3, 25));
	CWorksheet& Sheet = book.AddSheet("Items");

	/*Style style;
	style.horizAlign = ALIGN_H_CENTER;
	style.font.attributes = FONT_BOLD;
	const size_t CenterStyleIndex = book.AddStyle(style);*/

	Sheet.BeginRow();
	Sheet.AddCell("Items\\Boos_Names");
	for (const wstring& s : bossNames)
		Sheet.AddCell(s);
	Sheet.EndRow();

	for (const Item& i : items)
	{
		Sheet.BeginRow();
		if (i.name.size() > 1)
			Sheet.AddCell(i.name);
		else
			Sheet.AddCell(i.id);

		for (const wstring& s : bossNames)
		{
			bool found = false;
			size_t pos = 0;
			for (size_t m = 0; m < i.bossData.size(); m++)
				if (i.bossData.at(m).id == s)
				{
					found = true;
					pos = m;
					break;
				}
			if (found)
				Sheet.AddCell(to_wstring(i.bossData.at(pos).baseChance * 100) + L"%");
			else
				Sheet.AddCell(L"");
		}
		Sheet.EndRow();
	}

	if (!book.Save(file1))
		Logger::i().log(L"The book saving has been failed");
}