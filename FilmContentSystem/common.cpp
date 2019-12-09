#include "common.h"
#include "CharString.h"
#include <cstring> 

bool isLower(wchar_t w) { return 'a' <= w && w <= 'z'; }
bool isUpper(wchar_t w) { return 'A' <= w && w <= 'Z'; }
bool isDigit(wchar_t w) { return '0' <= w && w <= '9'; }
bool isHan(wchar_t w) { return L'Ò»' <= w && w <= L'ý›'; }
bool isChinese(wchar_t w) { 
	return isHan(w) || isLower(w) || isUpper(w) || isDigit(w); 
}
bool isAlpha(wchar_t w) {
	return isLower(w) || isUpper(w);
}

bool isNumber(const CharString & str)
{
	for (int i = 0; i < str.length(); i++)
		if (!isDigit(str[i]))
			return false;
	return true;
}

bool isSpaces(const CharString & str)
{
	for (int i = 0; i < str.length(); i++)
		if (!iswspace(str[i]))
			return false;
	return true;
}

bool endsWith(const char * str, const char * end) {
	int len1 = strlen(str), len2 = strlen(end);
	return len1 >= len2 && strcmp(str + len1 - len2, end) == 0;
}