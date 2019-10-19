#include "WordSegmentor.h"
#include "FastIO.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>



WordSegmentor::WordSegmentor()
{
}


WordSegmentor::~WordSegmentor()
{
}

void WordSegmentor::loadDict(const char * dictFile)
{
	loadDict_v1(dictFile);
}

void WordSegmentor::loadDict_v1(const char * dictFile)
{
	dict.reserve(350000);
	numWords = 0; totalFreq = 0; maxWordLen = 0;

	freopen(dictFile, "r", stdin);
	char cword[100], POS[100]; int freq; wchar_t word[100];
	int t;
	while ((t=fast_read(cword))!=-1) {
		fast_read(freq); fast_read(POS);
		mbstowcs(word, cword, 100);

		++numWords; totalFreq += freq; maxWordLen = std::max(maxWordLen, (int)wcslen(word));
		dict[CharString(word)] = freq;
		/*if (numWords % 100000 == 0)
			std::cout << numWords << std::endl;*/
	}
	freopen("CON", "r", stdin);
}

void WordSegmentor::loadDict_v2(const char * dictFile)
{
	dict.reserve(350000);
	numWords = 0; totalFreq = 0;

	FILE * pFile;
	pFile = fopen(dictFile, "r");
	wchar_t line[100];
	wchar_t word[100] = { 0 }; int freq = 0;
	if (pFile != NULL)
	{
		while (fgetws(line, 100, pFile) != NULL) { //TODO:unsafe
			int p = -1; bool readFreq = false;
			freq = 0;
			for (int i = 0; i < wcslen(line); i++) {
				if (line[i] == ' ') {
					if (readFreq) break;
					readFreq = true; continue;
				}
				if (readFreq)
					freq = freq * 10 + line[i] - '0';
				else
					word[++p] = line[i];
			}
			word[++p] = 0;
			//std::wcout << word << " " << freq << std::endl;

			++numWords; totalFreq += freq; maxWordLen = std::max(maxWordLen, (int)wcslen(word));
			dict[CharString(word)] = freq;
			/*if (numWords % 100000 == 0)
				std::cout << numWords << std::endl;*/
		}
		fclose(pFile);
	}
}

void WordSegmentor::loadDict_v3(const char * dictFile)
{
	dict.reserve(350000);
	numWords = 0; totalFreq = 0;

	std::wifstream wfin = std::wifstream(dictFile);
	wchar_t word[100], POS[100]; int freq;
	while (!wfin.eof()) {
		wfin >> word >> freq >> POS;

		++numWords; totalFreq += freq; maxWordLen = std::max(maxWordLen, (int)wcslen(word));
		dict[CharString(word)] = freq;
		/*if (numWords % 100000 == 0)
			std::cout << numWords << std::endl;*/
	}
	wfin.close();
}
