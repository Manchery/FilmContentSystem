#pragma once

#include "FilmInfo.h"
#include "WordSegmentor.h"
#include "HtmlParser.h"

/*-------------------------Ĭ�ϵ�config-----------------------------*/

const char DEFAULT_CONFIG_PATH[] = "FilmContentSystem.config";

const char DEFAULT_INPUT_DIR[] = "input/";
const char DEFAULT_OUTPUT_DIR[] = "output/";

const char DEFAULT_DICT_PATH[] = "dict/dict.txt";
const char DEFAULT_HMM_PATH[] = "dict/HMM.txt";
const char DEFAULT_STOP_PATH[] = "dict/stopwords.txt";

const bool DEFAULT_USE_HMM = true;
const bool DEFAULT_USE_STOP = false;

const int MAX_FLAG_LEN = 500;	// config��flag����󳤶�

/*-------------------------END: Ĭ�ϵ�config-------------------------*/

/*
FilmContentSystemApplication
˵����	����Ŀ��Ӧ�ó����࣬ʵ�ִ������ֵ䡢����html�����ķִʵȹ���
�ӿڣ�	loadConfig
		initDictionary
		extractInfo
		divideWords
		run
*/

class FilmContentSystemApplication
{
private:
	WordSegmentor segmentor;	// ���ķִ�
	HtmlParser parser;			// html����
	
	// ��������ļ���ŵ�·��
	char inputDir[MAX_FLAG_LEN], outputDir[MAX_FLAG_LEN];
	// �ʵ䡢HMM������ͣ�ôʵ��·��
	char dictFile[MAX_FLAG_LEN], hmmFile[MAX_FLAG_LEN], stopwordsFile[MAX_FLAG_LEN];
	// �ִ��Ƿ����� HMM ��ͣ�ô�
	bool useHMM, useStopwords;
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	void run(const char *configFile = nullptr);

	// ��ȡ config��Ҫ��config�ļ����� UTF-8 ����
	void loadConfig(const char *configFile);

	// ��ȡ�ֵ䡢HMM������ͣ�ôʵ�
	bool initDictionary(const char *dictFile, const char *hmmFile = nullptr, const char *stopwordsFile = nullptr);
	// ����Html
	FilmInfo extractInfo(const char *htmlFile);
	// ���ķִ�
	CharStringLink divideWords(const CharString &passage, bool useHMM, bool useStopwords);
};

