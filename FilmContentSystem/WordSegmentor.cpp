#include "WordSegmentor.h"
#include <iostream>
#include <fstream>
#include <ctime>



WordSegmentor::WordSegmentor()
{
}


WordSegmentor::~WordSegmentor()
{
}

void WordSegmentor::loadDict(const char * dictFile)
{
	auto start = clock();

	/*freopen(dictFile, "r",stdin);
	char c;
	while ((c = getchar()) != EOF);*/

	/*FILE * pFile;
	pFile = fopen(dictFile, "r");
	wchar_t mystring[100];
	int cnt = 0;
	if (pFile != NULL)
	{
		while (fgetws(mystring, 100, pFile) != NULL) {
			if ((++cnt) % 100000 == 0)
				std::cout << cnt << std::endl;
		}
		fclose(pFile);
	}*/

	std::wifstream wfin = std::wifstream(dictFile);
	wfin.tie(0);
	numWords = 0; totalFreq = 0;
	std::wstring word, POS; int freq; 
	while (!wfin.eof()) {
		wfin >> word >> freq >> POS;
		++numWords; totalFreq += freq;
		dict[CharString(word)] = freq;
		if (numWords % 100000 == 0)
			std::cout << numWords << std::endl;
	}
	wfin.close();

	auto end = clock();
	std::cout << ((double)end - start) / CLOCKS_PER_SEC << std::endl;
}
