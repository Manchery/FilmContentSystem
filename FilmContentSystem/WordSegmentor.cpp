#include "WordSegmentor.h"
#include "FastIO.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
using std::max;



WordSegmentor::WordSegmentor()
{
}


WordSegmentor::~WordSegmentor()
{
	delete[] logProb; delete[] jump;
}

void WordSegmentor::loadDict(const char * dictFile)
{
	loadDict_v1(dictFile);
}

CharStringLink WordSegmentor::cut(const CharString &passage)
{
	CharStringLink res;

	CharString sentence;
	for (int i = 0; i < passage.length(); i++) {
		if (isChinese(passage[i])) {
			sentence += passage[i];
		}
		else {
			res.concat(cut_DAG(sentence));
			sentence.clear();
			res.add_back(passage[i]);
		}
	}
	if (!sentence.empty()) {
		res.concat(cut_DAG(sentence));
		sentence.clear();
	}
	return res;
}

CharStringLink WordSegmentor::cut_DAG(const CharString &sentense)
{
	int len = sentense.length();
	delete[] logProb; logProb = new double[len+1]; 
	delete[] jump; jump = new int[len+1];
	logProb[len] = 0;
	double logTotalFreq = log(totalFreq);
	for (int i = len - 1; ~i; i--) {
		logProb[i] = -1e10; jump[i] = -1;
		bool hasWord = false;
		for (int j = 1; j <= maxWordLen && i + j - 1 < len; j++) {
			CharString word = sentense.substring(i, i + j); // TODO: optimize speed
			if (dict.find(word)) {
				hasWord = true;
				int freq = dict[word];
				double tmp = logProb[i + j] + log(freq) - logTotalFreq;
				if (tmp >= logProb[i]) {
					logProb[i] = tmp;
					jump[i] = i + j;
				}
			}
		}
		if (!hasWord) {
			logProb[i] = logProb[i + 1] /* + log(1) */ - logTotalFreq;
			jump[i] = i + 1;
		}
	}
	CharStringLink res;
	if (jump[0] == -1) return res;
	int p = 0; 
	while (p != len) {
		res.add(sentense.substring(p, jump[p]));
		p = jump[p];
	}
	return res;
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

bool isLower(wchar_t w) {
	return 'a' <= w && w <= 'z';
}

bool isUpper(wchar_t w) {
	return 'A' <= w && w <= 'Z';
}

bool isDigit(wchar_t w) {
	return '0' <= w && w <= '9';
}

bool isHan(wchar_t w) {
	return L'Ò»' <= w && w <= L'ý›';
}

bool isChinese(wchar_t w) {
	return isHan(w) || isLower(w) || isUpper(w) || isDigit(w);
}
