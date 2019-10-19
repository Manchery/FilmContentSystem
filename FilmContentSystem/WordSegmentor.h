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

public:
	WordSegmentor();
	~WordSegmentor();
	void loadDict(const char *dictFile);
};

