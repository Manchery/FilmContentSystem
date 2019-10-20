#pragma once

#include "CharStringLink.h"
#include "HashMap.hpp"
#include <string>

const int MAX_HAN_CODE = 50000;

class WordSegmentor
{
	HashMap < CharString, int, charStringHash > dict;
	int totalFreq, numWords;
	int maxWordLen;
	
	static int state2Idx(char s);
	double (*probEmit)[4];
	double probStart[4];
	double probTrans[4][4];
	bool hasHMM;

	double *logProb; int *jump;
	double(*vit)[4]; int *optState;

	void viterbi(const CharString& sentense);
	CharStringLink cut_HMM(const CharString &sentense);

	void calcDAG(const CharString &sentense);
	
	CharStringLink cut_DAG(const CharString &sentense); // TODO: unit test
	CharStringLink cut_DAG_HMM(const CharString &sentense);
public:
	WordSegmentor();
	~WordSegmentor();
	void loadDict(const char *dictFile);
	void loadHMM(const char *hmmFile);
	CharStringLink cut(const CharString &passage, bool useHMM = true);
};

extern bool isLower(wchar_t w);
extern bool isUpper(wchar_t w);
extern bool isDigit(wchar_t w);
extern bool isAlpha(wchar_t w);
extern bool isHan(wchar_t w);
extern bool isChinese(wchar_t w);
