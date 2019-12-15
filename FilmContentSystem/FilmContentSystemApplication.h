#pragma once

#include "FilmInfo.h"
#include "WordSegmentor.h"
#include "HtmlParser.h"
#include "Vector.hpp"
#include "InvertedIndex.hpp"
#include "BalancedBST.hpp"

/*-------------------------Ĭ�ϵ�config-----------------------------*/

const char DEFAULT_CONFIG_PATH[] = "FilmContentSystem.config";

const char DEFAULT_INPUT_DIR[] = "input/";
const char DEFAULT_OUTPUT_DIR[] = "output/";

const char DEFAULT_DICT_PATH[] = "dict/dict.txt";
const char DEFAULT_HMM_PATH[] = "dict/HMM.txt";
const char DEFAULT_STOP_PATH[] = "dict/stopwords.txt";

const bool DEFAULT_USE_HMM = true;
const bool DEFAULT_USE_STOP = false;

const char RETRIEVAL_INPUT[] = "query1.txt";
const char RETRIEVAL_OUTPUT[] = "result1.txt";
const char RECOMMEND_INPUT[] = "query2.txt";
const char RECOMMEND_OUTPUT[] = "result2.txt";

const int MAX_FLAG_LEN = 500;	// config��flag����󳤶�

/*-------------------------END: Ĭ�ϵ�config-------------------------*/

/*
FilmContentSystemApplication
˵����	����Ŀ��Ӧ�ó����࣬ʵ�ִ������ֵ䡢����html�����ķִʵȹ���
�ӿڣ�	loadConfig, loadDatabase, buildIndex
		initDictionary, extractInfo, divideWords
		retrieve, recommend
		doRetrieve, doRecommend
		run
*/

class FilmContentSystemApplication
{
private:
	WordSegmentor segmentor;	// ���ķִ�
	HtmlParser parser;			// html����
	
	// ��������ļ���ŵ�·��
	char inputDir[MAX_FLAG_LEN], outputDir[MAX_FLAG_LEN];
	// ����������������ļ�
	char retrieInput[MAX_FLAG_LEN], retrieOutput[MAX_FLAG_LEN];
	// �����Ƽ���������ļ�
	char recommInput[MAX_FLAG_LEN], recommOutput[MAX_FLAG_LEN];
	// �ʵ䡢HMM������ͣ�ôʵ��·��
	char dictFile[MAX_FLAG_LEN], hmmFile[MAX_FLAG_LEN], stopwordsFile[MAX_FLAG_LEN];
	// �ִ��Ƿ����� HMM ��ͣ�ô�
	bool useHMM, useStopwords;

	// ��Ӱ����
	int docCnt;

	// ��Ӱ��Ϣ�������
	Vector<FilmInfo> filmInfos;
	// ������ִʽ��
	Vector<CharStringLink> filmWords;
	// �ִʡ����Ͷ�Ӧ�ĵ����ĵ����ֱ����ڼ������Ƽ�
	// ģ����� index_t ��ѡΪ SplayTree �� HashMap
	InvertedIndex<SplayTree> wordIndex, genreIndex;
	// ����Ӱ��ӳ�䵽��Ӱid
	HashMap<CharString, int> filmIdMap;
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	bool init(const char *configFile = nullptr);

	void run(const char *configFile = nullptr);

	// ��ȡ config��Ҫ��config�ļ����� UTF-8 ����
	void loadConfig(const char *configFile);
	// �����Ӱ���ݿ�
	void loadDatabase();
	// ���������ĵ�
	void buildIndex();
	// ��������
	void doRetrieve();
	// �����Ƽ�
	void doRecommend();

	// ��ȡ�ֵ䡢HMM������ͣ�ôʵ�
	bool initDictionary(const char *dictFile, const char *hmmFile = nullptr, const char *stopwordsFile = nullptr);
	// ����Html
	FilmInfo extractInfo(const char *htmlFile);
	// ���ķִ�
	CharStringLink divideWords(const CharString &passage, bool useHMM, bool useStopwords);
	// ���������ص�Ӱid�͹ؼ��ʸ����͹ؼ��ֳ����ܴ���
	Vector<std::pair<int, std::pair<int, int>>> retrieve(const CharStringLink &keywords) const;
	// �Ƽ������� topK �ĵ�Ӱid�͵�Ӱ�����Ƽ���Ҫ�����ǵ�Ӱ���ֺ����ͣ����ο����ݡ����ݡ���ǩ������
	// ����أ�score = ����/2 + ����IoU*5 + ���ݽ���size + top5���ݽ���size + ��ǩ����size + ��������size
	Vector<std::pair<int, CharString>> recommend(int docId, int topK) const;

	// �� GUI �ṩ�Ĳ�ѯ�ӿ�
	bool getUseHMM() const { return useHMM; }
	bool getUseStopwords() const { return useStopwords; }
	bool hasName(const CharString &name) const { return filmIdMap.find(name); }
	int getIdFromName(const CharString& name) const { return filmIdMap.at(name); }
	const FilmInfo& getInfo(int idx) const { return filmInfos[idx]; }
	std::wstring getInputDir() const;
};

// ���ļ�·��ת��Ϊ ANSI ����
extern void filePathCvtCode(char *filepath);
// ��ȡ��Ӱ��������ļ� *.info
extern void readFilmInfo(const char *file, FilmInfo &info);
// ��ȡ�ִʽ���ļ� *.txt
extern void readFilmWord(const char *file, CharStringLink &cuts);