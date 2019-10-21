#pragma once

#include "FilmInfo.h"
#include "WordSegmentor.h"
#include "HtmlParser.h"

const char CONFIG_PATH[] = "FilmContentSystem.config";

const char DEFAULT_INPUT_DIR[] = "input/";
const char DEFAULT_OUTPUT_DIR[] = "output/";

const char DEFAULT_DICT_PATH[] = "dict/dict.txt";
const char DEFAULT_HMM_PATH[] = "dict/HMM.txt";
const char DEFAULT_STOP_PATH[] = "dict/stopwords.txt";

const bool DEFAULT_USE_HMM = true;
const bool DEFAULT_USE_STOP = false;


const int MAX_FLAG_LEN = 100;

class FilmContentSystemApplication
{
private:
	WordSegmentor segmentor;
	HtmlParser parser;
	
	char inputDir[MAX_FLAG_LEN], outputDir[MAX_FLAG_LEN];
	char dictFile[MAX_FLAG_LEN], hmmFile[MAX_FLAG_LEN], stopwordsFile[MAX_FLAG_LEN];
	bool useHMM, useStopwords;
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	void loadConfig(const char *configFile);
	void run();

	void initDictionary(const char *dictFile, const char *hmmFile = nullptr, const char *stopwordsFile = nullptr);
	FilmInfo extractInfo(const char *htmlFile);
	CharStringLink divideWords(const CharString &passage);
};

