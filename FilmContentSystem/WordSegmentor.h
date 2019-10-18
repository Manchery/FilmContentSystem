#pragma once

#include "CharStringLink.h"
#include "HashMap.hpp"
#include <string>

class WordSegmentor
{
	HashMap < CharString, int, charStringHash > dict;
	int totalFreq, numWords;
public:
	WordSegmentor();
	~WordSegmentor();
	void loadDict(const char *dictFile);
};

