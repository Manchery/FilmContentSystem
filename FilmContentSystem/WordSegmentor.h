#pragma once

#include "CharStringLink.h"
#include "HashMap.hpp"
#include <string>

const int MAX_HAN_CODE = 50000;

class WordSegmentor
{
	HashMap < CharString, int, charStringHash > dict, stopwords;
	int totalFreq, numWords;
	int maxWordLen;

	bool hasStopwords;
	
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

	CharStringLink removeStopwords(const CharStringLink& words);
public:
	WordSegmentor();
	~WordSegmentor();
	void loadDict(const char *dictFile);
	void loadHMM(const char *hmmFile);
	void loadStopwords(const char *stopwordsFile);
	CharStringLink cut(const CharString &passage, bool useHMM = true, bool useStopwords = false);
};
