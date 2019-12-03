#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>

/*
common.h
˵����	���ù���������ʵ��
*/

extern bool isLower(wchar_t w);		// �Ƿ���Сд��ĸ a-z
extern bool isUpper(wchar_t w);		// �Ƿ��Ǵ�д��ĸ A-Z
extern bool isDigit(wchar_t w);		// �Ƿ������� 0-9
extern bool isAlpha(wchar_t w);		// �Ƿ�����ĸ isLower(w) || isUpper(w)
extern bool isHan(wchar_t w);		// �Ƿ��Ǻ��� L'һ' <= w && w <= L'��'
extern bool isChinese(wchar_t w);	// isHan(w) || isLower(w) || isUpper(w) || isDigit(w)

class CharString;
extern bool isNumber(const CharString &str);	// �Ƿ�ÿһλ isDigit
extern bool isSpaces(const CharString &str);	// �Ƿ�ÿһλ iswspace

extern bool endsWith(const char * str, const char *end);	// str�Ƿ���end��β

template<typename T1, typename T2>
constexpr auto Max(T1 a, T2 b) { return ((a) > (b) ? (a) : (b)); }