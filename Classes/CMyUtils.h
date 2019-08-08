#pragma once
#include <string>

#include <fstream>
#include <locale>
#include <codecvt>

using namespace std;

class CMyUtils
{
public:
	CMyUtils();
	~CMyUtils();

	string UTF8FromUTF16(const wstring & utf16);
	vector<string> getTextData(const string &fullPath);
	int is_zh_ch(char p);
	string substr(const string &str, int start, int end);
};

