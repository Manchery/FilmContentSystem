#pragma once

#include "FilmInfo.h"
#include "WordSegmentor.h"
#include "HtmlParser.h"
#include "Vector.hpp"
#include "InvertedIndex.hpp"
#include "BalancedBST.hpp"

/*-------------------------默认的config-----------------------------*/

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

const int MAX_FLAG_LEN = 500;	// config中flag的最大长度

/*-------------------------END: 默认的config-------------------------*/

/*
FilmContentSystemApplication
说明：	该项目的应用程序类，实现从载入字典、解析html、中文分词等功能
接口：	loadConfig, loadDatabase, buildIndex
		initDictionary, extractInfo, divideWords
		retrieve, recommend
		doRetrieve, doRecommend
		run
*/

class FilmContentSystemApplication
{
private:
	WordSegmentor segmentor;	// 中文分词
	HtmlParser parser;			// html解析
	
	// 输入输出文件存放的路径
	char inputDir[MAX_FLAG_LEN], outputDir[MAX_FLAG_LEN];
	// 批量检索输入输出文件
	char retrieInput[MAX_FLAG_LEN], retrieOutput[MAX_FLAG_LEN];
	// 批量推荐输入输出文件
	char recommInput[MAX_FLAG_LEN], recommOutput[MAX_FLAG_LEN];
	// 词典、HMM参数、停用词典的路径
	char dictFile[MAX_FLAG_LEN], hmmFile[MAX_FLAG_LEN], stopwordsFile[MAX_FLAG_LEN];
	// 分词是否启用 HMM 和停用词
	bool useHMM, useStopwords;

	// 电影总数
	int docCnt;

	// 电影信息解析结果
	Vector<FilmInfo> filmInfos;
	// 剧情简介分词结果
	Vector<CharStringLink> filmWords;
	// 分词、类型对应的倒排文档，分别用于检索和推荐
	// 模板参数 index_t 可选为 SplayTree 或 HashMap
	InvertedIndex<SplayTree> wordIndex, genreIndex;
	// 将电影名映射到电影id
	HashMap<CharString, int> filmIdMap;
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	bool init(const char *configFile = nullptr);

	void run(const char *configFile = nullptr);

	// 读取 config，要求config文件采用 UTF-8 编码
	void loadConfig(const char *configFile);
	// 载入电影数据库
	void loadDatabase();
	// 建立倒排文档
	void buildIndex();
	// 批量检索
	void doRetrieve();
	// 批量推荐
	void doRecommend();

	// 读取字典、HMM参数、停用词典
	bool initDictionary(const char *dictFile, const char *hmmFile = nullptr, const char *stopwordsFile = nullptr);
	// 解析Html
	FilmInfo extractInfo(const char *htmlFile);
	// 中文分词
	CharStringLink divideWords(const CharString &passage, bool useHMM, bool useStopwords);
	// 检索，返回电影id和关键词个数和关键字出现总次数
	Vector<std::pair<int, std::pair<int, int>>> retrieve(const CharStringLink &keywords) const;
	// 推荐，返回 topK 的电影id和电影名，推荐主要依据是电影评分和类型，并参考导演、主演、标签、地区
	// 具体地，score = 评分/2 + 类型IoU*5 + 导演交集size + top5主演交集size + 标签交集size + 地区交集size
	Vector<std::pair<int, CharString>> recommend(int docId, int topK) const;

	// 给 GUI 提供的查询接口
	bool getUseHMM() const { return useHMM; }
	bool getUseStopwords() const { return useStopwords; }
	bool hasName(const CharString &name) const { return filmIdMap.find(name); }
	int getIdFromName(const CharString& name) const { return filmIdMap.at(name); }
	const FilmInfo& getInfo(int idx) const { return filmInfos[idx]; }
	std::wstring getInputDir() const;
};

// 将文件路径转化为 ANSI 编码
extern void filePathCvtCode(char *filepath);
// 读取电影解析结果文件 *.info
extern void readFilmInfo(const char *file, FilmInfo &info);
// 读取分词结果文件 *.txt
extern void readFilmWord(const char *file, CharStringLink &cuts);