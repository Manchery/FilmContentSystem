#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>

extern char fast_getchar();
extern void fast_read(int &x);
extern int fast_read(char *s);
extern void fast_read(double &x);

// Ref: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
extern std::wstring read_utf8_file(const char* filename);