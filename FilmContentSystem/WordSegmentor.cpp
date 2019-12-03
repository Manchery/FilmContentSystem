#include "WordSegmentor.h"
#include "common.h"
#include "FileReader.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <windows.h>

WordSegmentor::WordSegmentor()
{
	hasHMM = hasStopwords = false;
	probEmit = new double[MAX_HAN_CODE][4];
	logProb = nullptr; jump = nullptr;
	vit = nullptr; optState = nullptr;

	toConcatNumerals = true;
	if (toConcatNumerals) {
		numeralFreq = new int[MAX_HAN_CODE];
		memset(numeralFreq, 0, sizeof(int)*MAX_HAN_CODE);
	}
	else
		numeralFreq = nullptr;
}

WordSegmentor::~WordSegmentor()
{
	delete[] logProb; delete[] jump;
	delete[] vit; delete[] optState;
	delete[] probEmit;
	delete[] numeralFreq;
}

int WordSegmentor::state2Idx(char s)
{
	switch (s){
		case 'B': return 0;
		case 'E': return 1;
		case 'M': return 2;
		case 'S': return 3;
		default: return -1;
	}
}

bool WordSegmentor::loadDict(const char * dictFile)
{
	FileReader f(dictFile);
	if (!f.bad()) {
		dict.reserve(350000); 
		numWords = 0; totalFreq = 0; maxWordLen = 0;

		const int MAXLEN = 100;
		char cword[MAXLEN], POS[MAXLEN]; int freq; wchar_t word[MAXLEN];
		int t;
		while ((t = f.read(cword)) != -1) {
			f.read(freq); f.read(POS);
			//mbstowcs(word, cword, MAXLEN);
			MultiByteToWideChar(CP_UTF8, 0, cword, -1, word, MAXLEN);

			++numWords; totalFreq += freq; maxWordLen = Max(maxWordLen, (int)wcslen(word));
			dict[word] = freq;

			// numeralFreq: 记录每个字作为量词结尾的出现次数，用于判断某一词是否为量词
			if (toConcatNumerals && strcmp(POS, "m") == 0)
				numeralFreq[word[wcslen(word) - 1]]++;
		}
		return true;
	}
	else {
		std::cerr << "Dictionary file not found!" << std::endl;
		return false;
	}
}

void WordSegmentor::loadHMM(const char * hmmFile)
{
	FileReader f(hmmFile);
	if (!f.bad()) {
		for (int i = 0; i < 4; i++) probStart[i] = -3.14e+100;
		for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) probTrans[i][j] = -3.14e+100;
		for (int i = 0; i < MAX_HAN_CODE; i++) for (int j = 0; j < 4; j++) probEmit[i][j] = -3.14e+100;

		char s[105];
		bool readingStart = false, readingTrans = false, readingEmit = false;
		int emitState;

		while (f.read(s) != -1) {
			if (strcmp(s, "#") == 0) {
				f.read(s);
				if (strcmp(s, "prob_start") == 0) readingStart = true;
				else if (strcmp(s, "prob_trans") == 0) readingTrans = true;
				else if (strcmp(s, "prob_emit_B") == 0) readingEmit = true, emitState = 0;
				else if (strcmp(s, "prob_emit_E") == 0) readingEmit = true, emitState = 1;
				else if (strcmp(s, "prob_emit_M") == 0) readingEmit = true, emitState = 2;
				else if (strcmp(s, "prob_emit_S") == 0) readingEmit = true, emitState = 3;
				else if (strcmp(s, "end") == 0) readingEmit = readingStart = readingTrans = false;
			}
			else {
				double p;
				if (readingEmit) {
					wchar_t word[5]; 
					// mbstowcs(word, s, 100);
					MultiByteToWideChar(CP_UTF8, 0, s, -1, word, 5);
					f.read(p);
					probEmit[(int)word[0]][emitState] = p;
				}
				else if (readingStart) {
					f.read(p);
					probStart[state2Idx(s[0])] = p;
				}
				else if (readingTrans) {
					char s2[5]; f.read(s2); f.read(p);
					probTrans[state2Idx(s[0])][state2Idx(s2[0])] = p;
				}
			}
		}

		hasHMM = true;
	}
	else {
		std::cerr << "HMM file not found!" << std::endl;
		hasHMM = false;
	}
}

void WordSegmentor::loadStopwords(const char * stopwordsFile)
{
	FileReader f(stopwordsFile);
	if (!f.bad()) {
		stopwords.reserve(1000);

		const int MAXLEN = 100;
		char cword[MAXLEN]; wchar_t word[MAXLEN];
		int t;
		while ((t = f.read(cword)) != -1) {
			//mbstowcs(word, cword, MAXLEN);
			MultiByteToWideChar(CP_UTF8, 0, cword, -1, word, MAXLEN);
			stopwords[CharString(word)] = 1;
		}

		hasStopwords = true;
	}
	else {
		std::cerr << "Stopwords file not found!" << std::endl;
		hasStopwords = false;
	}
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
				vit[i][j] = Max(vit[i][j], vit[i - 1][k] + probTrans[k][j]);
			}
			vit[i][j] += probEmit[(int)sentense[i]][j];
		}

	int k = 1;
	for (int j = 0; j < 4; j++)
		if (j == 1 || j == 3)	// 最后一字的隐状态只能为 'E' or 'S'
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

CharStringLink WordSegmentor::cut_HMM(const CharString & sentense)
{
	CharStringLink res;
	CharString buf; int len = sentense.length();
	buf += sentense[0];
	for (int i = 1; i <= len; i++) {
		if (i == len || (isHan(sentense[i]) ^ isHan(sentense[i - 1]))) {
			if (isHan(sentense[i - 1])) {
				viterbi(buf); int startPos;
				for (int j = 0; j < buf.length(); j++) {
					if (optState[j] == 3) {			// 'S'
						res.push_back(buf[j]);
					}
					else if (optState[j] == 0) {	// 'B'
						startPos = j;
					}
					else if (optState[j] == 1) {	// 'E'
						res.push_back(buf.substring(startPos, j + 1));
					}
				}
			}
			else {
				res.push_back(buf);		// 连续的西文字符（数字和字母）直接作为一词
			}
			buf.clear();
		}
		if (i < len) buf += sentense[i];
	}
	return res;
}

void WordSegmentor::calcDAG(const CharString & sentense)
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
			CharString word = sentense.substring(i, i + j);
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
		if (!hasWord) {		// 若不存在任何以该字开头的词，则只能单字成词
			logProb[i] = logProb[i + 1] /* + log(1) */ - logTotalFreq;
			jump[i] = i + 1;
		}
	}
}

CharStringLink WordSegmentor::cut(const CharString &passage, bool useHMM, bool useStopwords)
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
			// res.add_back(passage[i]);	// 标点符号等
		}
	}
	if (!sentence.empty()) {
		res.concat(hasHMM && useHMM ? cut_DAG_HMM(sentence) : cut_DAG(sentence));
		sentence.clear();
	}
	if (toConcatNumerals) res = concatNumerals(res);
	return hasStopwords && useStopwords ? removeStopwords(res) : res;
}

CharStringLink WordSegmentor::concatNumerals(const CharStringLink & words)
{
	CharStringLink res;
	const int nFreqThreshold = 10;	// 通过某词的结尾的字的 numeralFreq 来判断是否为量词的阈值
	for (auto it = words.begin(); it != words.end(); ++it)
		if (isNumber(*it) && it.next()!=words.end() 
			&& numeralFreq!=nullptr && numeralFreq[(*(it.next())).back()]>nFreqThreshold) {
			res.push_back(concat(*it, *(it.next())));
			++it;
		}else
			res.push_back(*it);
	return res;
}

CharStringLink WordSegmentor::removeStopwords(const CharStringLink & words)
{
	CharStringLink res;
	for (auto it = words.begin(); it != words.end(); ++it)
		if (!stopwords.find(*it))
			res.push_back(*it);
	return res;
}

CharStringLink WordSegmentor::cut_DAG(const CharString &sentense)
{
	calcDAG(sentense);

	CharStringLink res;
	int p = 0, len = sentense.length();
	while (p != len) {
		if (isHan(sentense[p])) {
			res.add(sentense.substring(p, jump[p]));
			p = jump[p];
		}
		else {		// 合并西文字符（数字和字母）
			int t = p; 
			while (p != len && !isHan(sentense[p])) p = jump[p];
			res.add(sentense.substring(t, p));
		}
	} 
	return res;
}

CharStringLink WordSegmentor::cut_DAG_HMM(const CharString & sentense)
{
	calcDAG(sentense);

	CharStringLink res;
	int p = 0, len = sentense.length();
	CharString buf;		// 存储连续的单字序列，用于输入HMM识别
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
