#pragma once

#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

struct BossData
{
	wstring id;
	double baseChance = 0.0;
	vector<double> chances;
};

struct Item
{
	wstring id; // interntal name
	wstring name;
	wstring descr;
	vector<BossData> bossData;
};