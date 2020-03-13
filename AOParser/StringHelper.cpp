#include "StringHelper.h"

StringHelper* StringHelper::_instance = nullptr;

vector<string> StringHelper::split(const string& s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

vector<wstring> StringHelper::split(const wstring& s, wchar_t delimiter)
{
    vector<wstring> tokens;
    wstring token;
    wistringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/*wstring StringHelper::sToW(const string& input)
{
    wstring res;
    res = dbj::range_to_wstring(input);
    return wstring();
}*/

StringHelper& StringHelper::i()
{
    if (!_instance)
        _instance = new StringHelper();
    return *_instance;
}

StringHelper::StringHelper()
{}

StringHelper::~StringHelper()
{}