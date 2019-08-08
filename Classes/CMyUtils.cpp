#include "cocos2d.h"
#include "CMyUtils.h"

CMyUtils::CMyUtils()
{
}


CMyUtils::~CMyUtils()
{
}

string CMyUtils::UTF8FromUTF16(const wstring & utf16)
{
	// 
// Special case of empty input string 
// 
	if (utf16.empty())
		return string();


	// 
	// Get length (in chars) of resulting UTF-8 string 
	// 
	const int utf8Length = ::WideCharToMultiByte(
		CP_UTF8,            // convert to UTF-8 
		0,                  // default flags 
		utf16.data(),       // source UTF-16 string 
		utf16.length(),     // source string length, in wchar_t's, 
		NULL,               // unused - no conversion required in this step 
		0,                  // request buffer size 
		NULL, NULL          // unused 
	);
	if (utf8Length == 0)
	{
		// Error 
		DWORD error = ::GetLastError();
		/*throw utf8_conversion_error(
			"Can't get length of UTF-8 string (WideCharToMultiByte failed).",
			utf8_conversion_error::conversion_utf8_from_utf16,
			error);*/
	}


	// 
	// Allocate destination buffer for UTF-8 string 
	// 
	string utf8;
	utf8.resize(utf8Length);


	// 
	// Do the conversion from UTF-16 to UTF-8 
	// 
	if (!::WideCharToMultiByte(
		CP_UTF8,                // convert to UTF-8 
		0,                      // default flags 
		utf16.data(),           // source UTF-16 string 
		utf16.length(),         // source string length, in wchar_t's, 
		&utf8[0],               // destination buffer 
		utf8.length(),          // destination buffer size, in chars 
		NULL, NULL              // unused 
	))
	{
		// Error 
		DWORD error = ::GetLastError();
		/*throw utf8_conversion_error(
			"Can't convert string from UTF-16 to UTF-8 (WideCharToMultiByte failed).",
			utf8_conversion_error::conversion_utf8_from_utf16,
			error);*/
	}


	// 
	// Return resulting UTF-8 string 
	// 
	return utf8;
}

vector<string> CMyUtils::getTextData(const string &fullPath)
{
	vector<string> v;
	wfstream inputStream;
	wstring thisLine;


	// Open file
	inputStream.imbue(locale(inputStream.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));
	inputStream.open(fullPath.c_str(), ios::in);

	// Check if it is open
	bool bOpen = inputStream.is_open();

	if (bOpen) {
		while (getline(inputStream, thisLine)) {
			// Put all lines in vector
			v.push_back(UTF8FromUTF16(thisLine));
		}
		inputStream.close();
	}

	return v;
}

int CMyUtils::is_zh_ch(char p)
{
	if (~(p >> 8) == 0){
		return 1;
	}
	return -1;
}

string CMyUtils::substr(const string &str, int start, int end)
{
	if(typeid(str)==typeid(string) && str.length()>0){
		int len=str.length();
		string tmp="";
		vector <string> dump;
		int i=0;
		while(i<len)
		{
			if (is_zh_ch(str.at(i))==1)
			{
				dump.push_back(str.substr(i,3));
				i=i+3;
			}else{
				dump.push_back(str.substr(i,1));
				i=i+1;
			}
		}
		end = end>0 ? end : dump.size();
		if(start<0||start>end){
			return "";
		}
		for(int i=start; i<=end; i++)
		{
			tmp+=dump[i-1];
		}
		return tmp;
	}
	else
	{
		return "";
	}
}
