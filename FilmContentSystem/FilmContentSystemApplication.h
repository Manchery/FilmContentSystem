#pragma once

#include "FilmInfo.h"
#include "WordSegmentor.h"
#include "HtmlParser.h"

/*-------------------------默认的config-----------------------------*/

const char DEFAULT_CONFIG_PATH[] = "FilmContentSystem.config";

const char DEFAULT_INPUT_DIR[] = "input/";
const char DEFAULT_OUTPUT_DIR[] = "output/";

const char DEFAULT_DICT_PATH[] = "dict/dict.txt";
const char DEFAULT_HMM_PATH[] = "dict/HMM.txt";
const char DEFAULT_STOP_PATH[] = "dict/stopwords.txt";

const bool DEFAULT_USE_HMM = true;
const bool DEFAULT_USE_STOP = false;

const int MAX_FLAG_LEN = 500;	// config中flag的最大长度

/*-------------------------END: 默认的config-------------------------*/

/*
FilmContentSystemApplication
说明：	该项目的应用程序类，实现从载入字典、解析html、中文分词等功能
接口：	loadConfig
		initDictionary
		extractInfo
		divideWords
		run
*/

class FilmContentSystemApplication
{
private:
	WordSegmentor segmentor;	// 中文分词
	HtmlParser parser;			// html解析
	
	// 输入输出文件存放的路径
	char inputDir[MAX_FLAG_LEN], outputDir[MAX_FLAG_LEN];
	// 词典、HMM参数、停用词典的路径
	char dictFile[MAX_FLAG_LEN], hmmFile[MAX_FLAG_LEN], stopwordsFile[MAX_FLAG_LEN];
	// 分词是否启用 HMM 和停用词
	bool useHMM, useStopwords;
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	void run(const char *configFile = nullptr);

	// 读取 config，要求config文件采用 UTF-8 编码
	void loadConfig(const char *configFile);

	// 读取字典、HMM参数、停用词典
	bool initDictionary(const char *dictFile, const char *hmmFile = nullptr, const char *stopwordsFile = nullptr);
	// 解析Html
	FilmInfo extractInfo(const char *htmlFile);
	// 中文分词
	CharStringLink divideWords(const CharString &passage, bool useHMM, bool useStopwords);
};

