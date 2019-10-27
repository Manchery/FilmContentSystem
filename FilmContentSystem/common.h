#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>

extern char fast_getchar();
extern void clear_buf();
extern void fast_read(int &x);
extern int fast_read(char *s);
extern void fast_read(double &x);

extern int read_flag_name(char *s, char &nextChar);
extern int read_string_flag(char *s);
extern bool read_bool_flag();

// Ref: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
extern std::wstring read_utf8_file(const char* filename);

extern bool isLower(wchar_t w);
extern bool isUpper(wchar_t w);
extern bool isDigit(wchar_t w);
extern bool isAlpha(wchar_t w);
extern bool isHan(wchar_t w);
extern bool isChinese(wchar_t w);
class CharString;
extern bool isNumber(const CharString &str);
extern bool isSpaces(const CharString &str);

extern bool endsWith(const char * str, const char *end);