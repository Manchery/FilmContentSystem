#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>

/*
common.h
说明：	常用公共函数的实现
*/

extern bool isLower(wchar_t w);		// 是否是小写字母 a-z
extern bool isUpper(wchar_t w);		// 是否是大写字母 A-Z
extern bool isDigit(wchar_t w);		// 是否是数码 0-9
extern bool isAlpha(wchar_t w);		// 是否是字母 isLower(w) || isUpper(w)
extern bool isHan(wchar_t w);		// 是否是汉字 L'一' <= w && w <= L'龥'
extern bool isChinese(wchar_t w);	// isHan(w) || isLower(w) || isUpper(w) || isDigit(w)

class CharString;
extern bool isNumber(const CharString &str);	// 是否每一位 isDigit
extern bool isSpaces(const CharString &str);	// 是否每一位 iswspace

extern bool endsWith(const char * str, const char *end);	// str是否以end结尾

template<typename T1, typename T2>
constexpr auto Max(T1 a, T2 b) { return ((a) > (b) ? (a) : (b)); }