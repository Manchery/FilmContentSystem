#include "WordSegmentor.h"
#include "FastIO.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
using std::max;



WordSegmentor::WordSegmentor()
{
	hasHMM = false;
	probEmit = new double[MAX_HAN_CODE][4];
}

WordSegmentor::~WordSegmentor()
{
	delete[] logProb; delete[] jump;
	delete[] vit; delete[] optState;
	delete[] probEmit;
}

void WordSegmentor::loadDict(const char * dictFile)
{
	loadDict_v1(dictFile);
}

int WordSegmentor::state2Idx(char s)
{
	switch (s)
	{
	case 'B':
		return 0;
	case 'E':
		return 1;
	case 'M':
		return 2;
	case 'S':
		return 3;
	default:
		return -1;
	}
}

void WordSegmentor::loadHMM(const char * hmmFile)
{
	bool readingStart = false, readingTrans = false, readingEmit = false;
	int state;
	char s[105];
	
	for (int i = 0; i < 4; i++) probStart[i] = -3.14e+100;
	for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) probTrans[i][j] = -3.14e+100;
	for (int i = 0; i < MAX_HAN_CODE; i++) for (int j = 0; j < 4; j++) probEmit[i][j] = -3.14e+100;

	freopen(hmmFile, "r", stdin);
	while (fast_read(s) != -1) {
		if (strcmp(s, "#") == 0) {
			fast_read(s);
			if (strcmp(s, "prob_start") == 0) readingStart = true;
			else if (strcmp(s, "prob_trans") == 0) readingTrans = true;
			else if (strcmp(s, "prob_emit_B") == 0) readingEmit = true, state = 0;
			else if (strcmp(s, "prob_emit_E") == 0) readingEmit = true, state = 1;
			else if (strcmp(s, "prob_emit_M") == 0) readingEmit = true, state = 2;
			else if (strcmp(s, "prob_emit_S") == 0) readingEmit = true, state = 3;
			else if (strcmp(s, "end") == 0) readingEmit = readingStart = readingTrans = false;
		}
		else {
			double p;
			if (readingEmit) {
				wchar_t word[5]; mbstowcs(word, s, 100);
				fast_read(p);
				probEmit[(int)word[0]][state] = p;
			}
			else if (readingStart) {
				fast_read(p); 
				probStart[state2Idx(s[0])] = p;
			}
			else if (readingTrans) {
				char s2[5]; fast_read(s2); fast_read(p);
				probTrans[state2Idx(s[0])][state2Idx(s2[0])] = p;
			}
		}
	}

	hasHMM = true;
	freopen("CON", "r", stdin);
}

CharStringLink WordSegmentor::cut(const CharString &passage, bool useHMM)
{
	CharStringLink res;
	CharString sentence;
	for (int i = 0; i < passage.length(); i++) {
		if (isChinese(passage[i])) {
			sentence += passage[i];
		}
		else {
			res.concat(hasHMM && useHMM ? cut_DAG_HMM(sentence) : cut_DAG(sentence));
			sentence.clear();
			// res.add_back(passage[i]);
		}
	}
	if (!sentence.empty()) {
		res.concat(hasHMM && useHMM ? cut_DAG_HMM(sentence) : cut_DAG(sentence));
		sentence.clear();
	}
	return res;
}

void WordSegmentor::viterbi(const CharString & sentense)
{
	int len = sentense.length();
	delete[] vit; vit = new double[len][4];
	delete[] optState; optState = new int[len];

	for (int j = 0; j < 4; j++)
		vit[0][j] = probStart[j] + probEmit[(int)sentense[0]][j];
	for (int i=1;i<len;i++)
		for (int j = 0; j < 4; j++) {
			vit[i][j] = -3.14e+100;
			for (int k = 0; k < 4; k++) {
				vit[i][j] = max(vit[i][j], vit[i - 1][k] + probTrans[k][j]);
			}
			vit[i][j] += probEmit[(int)sentense[i]][j];
		}

	int k = 0;
	for (int j = 0; j < 4; j++)
		if (j == 1 || j == 3) // 'E' or 'S'
			if (vit[len - 1][j] > vit[len - 1][k])
				k = j;
	optState[len - 1] = k;

	for (int i = len - 2; ~i; i--) {
		int k = 0; int next = optState[i + 1];
		for (int j = 0; j < 4; j++)
			if (vit[i][j] + probTrans[j][next] > vit[i][k] + probTrans[k][next])
				k = j;
		optState[i] = k;
	}
}

void WordSegmentor::dag(const CharString & sentense)
{
	int len = sentense.length();
	delete[] logProb; logProb = new double[len + 1];
	delete[] jump; jump = new int[len + 1];
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
}

CharStringLink WordSegmentor::cut_DAG(const CharString &sentense)
{
	dag(sentense);
	CharStringLink res;
	int p = 0, len = sentense.length();
	while (p != len) {
		res.add(sentense.substring(p, jump[p]));
		p = jump[p];
	} // TODO: 手动合并西文字符（没有HMM
	return res;
}

CharStringLink WordSegmentor::cut_HMM(const CharString & sentense)
{
	CharStringLink res;
	CharString buf; int len = sentense.length();
	buf += sentense[0];
	for (int i = 1; i <= len; i++) {
		if (i==len || (isHan(sentense[i])^isHan(sentense[i-1]))) {
			if (isHan(sentense[i-1])) {
				viterbi(buf); int startPos;
				for (int j = 0; j < buf.length(); j++) {
					if (optState[j] == 3) { // 'S'
						res.add_back(buf[j]);
					}
					else if (optState[j] == 0) { // 'B'
						startPos = j;
					}
					else if (optState[j] == 1) { // 'E'
						res.add_back(buf.substring(startPos, j + 1));
					}
				}
			}else{
				res.add_back(buf);
			}
			buf.clear();
		}
		if (i<len) buf += sentense[i];
	}
	return res;
}

CharStringLink WordSegmentor::cut_DAG_HMM(const CharString & sentense)
{
	dag(sentense);
	CharStringLink res;
	int p = 0, len = sentense.length();
	CharString buf;
	while (p != len) {
		if (jump[p] == p + 1)
			buf += sentense[p];
		else {
			if (!buf.empty()) {
				if (buf.length() == 1)
					res.add(buf);
				else
					res.concat(cut_HMM(buf));
				buf.clear();
			}
			res.add(sentense.substring(p, jump[p]));
		}
		p = jump[p];
	}
	if (!buf.empty()) {
		if (buf.length() == 1)
			res.add(buf);
		else
			res.concat(cut_HMM(buf));
		buf.clear();
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
	return L'一' <= w && w <= L'龥';
}

bool isChinese(wchar_t w) {
	return isHan(w) || isLower(w) || isUpper(w) || isDigit(w);
}
