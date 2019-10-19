#pragma once

#include "CharStringLink.h"
#include "HashMap.hpp"
#include <string>

class WordSegmentor
{
	HashMap < CharString, int, charStringHash > dict;
	int totalFreq, numWords;
	int maxWordLen;

	void loadDict_v1(const char *dictFile);
	void loadDict_v2(const char *dictFile);
	void loadDict_v3(const char *dictFile);

	double *logProb; int *jump;

	CharStringLink cut_DAG(const CharString &sentense);
public:
	WordSegmentor();
	~WordSegmentor();
	void loadDict(const char *dictFile);
	CharStringLink cut(const CharString &passage);
};

extern bool isLower(wchar_t w);
extern bool isUpper(wchar_t w);
extern bool isDigit(wchar_t w);
extern bool isHan(wchar_t w);
extern bool isChinese(wchar_t w);

