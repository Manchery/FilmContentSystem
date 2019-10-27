#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>

/*
common.h
说明：	常用公共函数的实现
*/

// 通过 fread 从stdin读取一片缓存，实现比getchar更快的读入
// !!! 不可与其他读入混用
extern char fast_getchar();
// 清除缓存
extern void clear_buf();
// 读入一个int，通过fast_getchar实现
extern void fast_read(int &x);
// 读入一个char，通过fast_getchar实现
extern int fast_read(char *s);
// 读入一个double，通过fast_getchar实现
extern void fast_read(double &x);

// 以下函数用于读取config文件
// 读取由 A-Z 以及 _ 组成的flag名，调用fast_getchar
extern int read_flag_name(char *s, char &nextChar);
// 读取由引号包裹的字符串类型的flag的值，调用fast_getchar
extern int read_string_flag(char *s);
// 读取bool类型的flag的值，合法的值应为true或false的大写或小写，调用fast_getchar
extern bool read_bool_flag();

// 读入 utf-8 编码的文件
// Reference: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
extern std::wstring read_utf8_file(const char* filename);

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