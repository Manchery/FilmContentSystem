#include "FilmContentSystemApplication.h"
#include "FileReader.h"
#include "common.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <direct.h>
#include <io.h>

FilmContentSystemApplication::FilmContentSystemApplication()
{
	useHMM = useStopwords = false;
	docCnt = 0;
}

FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

void FilmContentSystemApplication::loadConfig(const char * configFile)
{
	FileReader f(configFile);
	if (!f.bad()) {
		char flag[MAX_FLAG_LEN], nextChar;
		while (f.read_flag_name(flag, nextChar) != -1) {
			if (nextChar != '=') while (f.nextchar() != '=');

			if (strcmp(flag, "INPUT_DIR") == 0) {
				f.read_string_flag(inputDir);
				filePathCvtCode(inputDir);
				if (!endsWith(inputDir, "/")) strcat_s(inputDir, "/");
			}
			else if (strcmp(flag, "OUTPUT_DIR") == 0) {
				f.read_string_flag(outputDir);
				filePathCvtCode(outputDir);
				if (!endsWith(outputDir, "/")) strcat_s(outputDir, "/");
			}
			else if (strcmp(flag, "DICT_PATH") == 0) {
				f.read_string_flag(dictFile);
				filePathCvtCode(dictFile);
			}
			else if (strcmp(flag, "HMM_PATH") == 0) {
				f.read_string_flag(hmmFile);
				filePathCvtCode(hmmFile);
			}
			else if (strcmp(flag, "STOP_PATH") == 0) {
				f.read_string_flag(stopwordsFile);
				filePathCvtCode(stopwordsFile);
			}
			else if (strcmp(flag, "USE_HMM") == 0) {
				useHMM = f.read_bool_flag();
			}
			else if (strcmp(flag, "USE_STOP") == 0) {
				useStopwords = f.read_bool_flag();
			}
			else if (strcmp(flag, "RETRIEVAL_INPUT") == 0) {
				f.read_string_flag(retrieInput);
				filePathCvtCode(retrieInput);
			}
			else if (strcmp(flag, "RETRIEVAL_OUTPUT") == 0) {
				f.read_string_flag(retrieOutput);
				filePathCvtCode(retrieOutput);
			}
			else if (strcmp(flag, "RECOMMEND_INPUT") == 0) {
				f.read_string_flag(recommInput);
				filePathCvtCode(recommInput);
			}
			else if (strcmp(flag, "RECOMMEND_OUTPUT") == 0) {
				f.read_string_flag(recommOutput);
				filePathCvtCode(recommOutput);
			}
		}
	}
	else { // by Default
		strcpy_s(inputDir, DEFAULT_INPUT_DIR);
		strcpy_s(outputDir, DEFAULT_OUTPUT_DIR);
		strcpy_s(dictFile, DEFAULT_DICT_PATH);
		strcpy_s(hmmFile, DEFAULT_HMM_PATH);
		strcpy_s(stopwordsFile, DEFAULT_STOP_PATH);
		useHMM = DEFAULT_USE_HMM; 
		useStopwords = DEFAULT_USE_STOP;
	}
}

void FilmContentSystemApplication::loadDatabase()
{
	_mkdir(outputDir);

	_finddata_t file;
	long lf; const int MAX_FILE_NAME_LEN = 1000;
	char inputReg[MAX_FILE_NAME_LEN] = { 0 };
	strcat_s(inputReg, inputDir); strcat_s(inputReg, "*");
	if ((lf = _findfirst(inputReg, &file)) == -1) {
		std::cerr << inputDir << " not found!" << std::endl;
	}
	else {
		filmInfos.resize(2000);
		filmWords.resize(2000);
		// 遍历输入目录下所有html文件
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if (!endsWith(file.name, ".html"))
				continue;

			// std::cerr << "Found file " << file.name << "..." << std::endl;

			char baseName[MAX_FILE_NAME_LEN] = { 0 };
			strncpy_s(baseName, file.name, strlen(file.name) - 5);
			char filePath[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(filePath, inputDir); strcat_s(filePath, file.name);
			char infoFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(infoFile, outputDir); strcat_s(infoFile, baseName); strcat_s(infoFile, ".info");
			char txtFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(txtFile, outputDir); strcat_s(txtFile, baseName); strcat_s(txtFile, ".txt");

			int docId = atoi(baseName); 
			docCnt = Max(docCnt, docId + 1);
			if (docId >= filmInfos.size()) filmInfos.resize(docId), filmWords.resize(docId);

			if (_access(infoFile, 0) == 0 && _access(txtFile, 0) == 0) { // 解析和分词结果已经存在
				readFilmInfo(infoFile, filmInfos[docId]);
				readFilmWord(txtFile, filmWords[docId]);
			}
			else {
				std::cerr << "Parsing & processing file " << file.name << "..." << std::endl;
				// 解析 html
				auto info = extractInfo(filePath);
				std::wofstream wfout(infoFile);
				wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
				wfout << info;
				wfout.close();
				filmInfos[docId] = info;

				// 中文分词
				CharStringLink cuts = divideWords(info.introduction(), useHMM, useStopwords);
				wfout.open(txtFile);
				wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
				for (auto it = cuts.begin(); it != cuts.end(); ++it)
					wfout << *it << std::endl;
				wfout.close();
				filmWords[docId] = cuts;
			}
			filmIdMap[filmInfos[docId].name()] = docId;
		}
	}
	_findclose(lf);
	std::cerr << "Loaded total " << docCnt << " files. " << std::endl;
}

void FilmContentSystemApplication::buildIndex()
{
	for (int i = 0; i < docCnt; i++) {
		CharStringLink cut = filmWords[i];
		if (cut.empty()) continue;

		for (auto it = cut.begin(); it != cut.end(); ++it) {
			wordIndex.inc(*it, i);
		}
		
		FilmInfo info = filmInfos[i];
		for (auto it = info.genres().begin(); it != info.genres().end(); ++it) {
			genreIndex.add(*it, i, info.rating());
		}
	}
}

Vector<std::pair<int, std::pair<int, int>>> FilmContentSystemApplication::retrieve(const CharStringLink & keywords)
{
	using std::pair;
	struct data_t {
		int cnt, tot; // 不同词数、总次数
		int clk; // 时间戳
	};
	int clk = 0;
	SplayTree<int, data_t> Map;	// first：电影 id
	for (auto it = keywords.begin(); it != keywords.end(); ++it) {
		++clk;
		CharString word = *it;
		TermInfo term = wordIndex.search(word);
		for (auto p = term.list.begin(); p != term.list.end(); ++p) {
			data_t &data = Map[p.id()];
			if (data.clk != clk) data.clk = clk, data.cnt++;
			data.tot += (int)p.rating();
		}
	}
	// TODO: maybe need speed optimize
	Vector<pair<int, data_t>> nodes = Map.list();
	typedef bool(*cmpFunc)(const pair<int, data_t>&, const pair<int, data_t>&);
	nodes.sort(cmpFunc{ [](const pair<int, data_t>& a, const pair<int, data_t> &b) {
		data_t aD = a.second ,bD = b.second;
		return aD.cnt == bD.cnt ? aD.tot > bD.tot: aD.cnt > bD.cnt;
	} });
	Vector<pair<int, pair<int, int>>> res;
	for (int i = 0; i < nodes.size(); i++)
		res.push_back(std::make_pair(nodes[i].first, std::make_pair(nodes[i].second.cnt, nodes[i].second.tot)));
	return res;
}

Vector<std::pair<int, CharString>> FilmContentSystemApplication::recommend(int docId, int topK)
{
	FilmInfo info = filmInfos[docId];
	struct data_t {
		double score; int id;
		bool operator < (const data_t &B) const {
			return score == B.score ? id<B.id : score>B.score;
		}
		bool operator == (const data_t &B) const {
			return id == B.id;
		}
	};
	Vector<data_t> nodes;
	int cap = topK * 1.5;
	for (auto it = info.genres().begin(); it != info.genres().end(); ++it) {
		CharString genre = *it;
		TermInfo term = genreIndex.search(genre);
		int cnt = 0;
		for (auto p = term.list.begin(); cnt < cap && p != term.list.end(); ++p, ++cnt) {
			if (p.id() == docId) continue;
			FilmInfo target = filmInfos[p.id()];

			double score = target.rating()/2 + 5 * IoU(target.genres(), info.genres())
				+ intersectionSize(target.directors(), info.directors())
				+ intersectionSize(target.stars(), info.stars(), 5)
				+ intersectionSize(target.tags(), info.tags())
				+ intersectionSize(target.regions(), info.regions());
			
			nodes.push_back(data_t{ score, p.id()});
		}
	}
	nodes.sort();
	nodes.unique();
	Vector<std::pair<int, CharString>> res;
	for (int i = 0; i < min(nodes.size(), topK); i++)
		res.push_back(std::make_pair(nodes[i].id, filmInfos[nodes[i].id].name()));
	
	// 如果不足topK，随意补全
	int ptr = 0;
	while (res.size() < topK && ptr<docCnt) {
		bool flag = 0;
		for (int i = 0; i < res.size(); i++)
			if (res[i].first == ptr) {
				flag = 1; break;
			}
		if (!flag) res.push_back(std::make_pair(ptr, filmInfos[ptr].name()));
		ptr++;
	}

	return res;
}

void FilmContentSystemApplication::doRetrieve()
{
	std::wifstream wfin(retrieInput);
	wfin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wofstream wfout(retrieOutput);
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	
	const int MAXLEN = 1000;
	wchar_t line[MAXLEN];
	while(!wfin.eof()) {
		wfin.getline(line, MAXLEN);
		if (wcslen(line) == 0) continue;

		std::wistringstream wsin(line); 
		CharStringLink keywords; wchar_t tmp[100];
		while (!wsin.eof()) {
			wsin >> tmp;
			if (wcslen(tmp) == 0) continue;
			keywords.concat(divideWords(tmp, useHMM, useStopwords));
		}

		Vector<std::pair<int, std::pair<int, int>>> res = retrieve(keywords);
		
		for (int i = 0; i < res.size(); i++) {
			if (i) wfout << ' ';
			wfout << '(' << res[i].first << ',' << res[i].second.second << ")";
		}
		wfout << std::endl;
	}
	wfin.close(); wfout.close();
}

void FilmContentSystemApplication::doRecommend()
{
	std::wifstream wfin(recommInput);
	wfin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wofstream wfout(recommOutput);
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));

	const int MAXLEN = 1000;
	wchar_t line[MAXLEN];
	while (!wfin.eof()) {
		wfin.getline(line, MAXLEN);
		if (wcslen(line) == 0) continue;

		if (!filmIdMap.find(line)) {
			wfout << L"该电影不在数据库中，无法推荐" << std::endl;
			continue;
		}
		int docId = filmIdMap[line];
		Vector<std::pair<int, CharString>> res = recommend(docId, 10);

		for (int i = 0; i < res.size(); i++) {
			if (i) wfout << ' ';
			wfout << '(' << res[i].first << ',' << res[i].second << ")";
		}
		wfout << std::endl;
	}

	wfin.close(); wfout.close();
}


bool FilmContentSystemApplication::init(const char * configFile)
{
	loadConfig(configFile != nullptr ? configFile : DEFAULT_CONFIG_PATH);
	if (!initDictionary(dictFile, hmmFile, stopwordsFile)) {
		std::cerr << "Load dictionary failed !" << std::endl;
		return false;
	}
	loadDatabase();
	buildIndex();
	return true;
}

void FilmContentSystemApplication::run(const char * configFile)
{
	if (!init(configFile)) return;
	doRetrieve();
	doRecommend();
}

bool FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile, const char *stopwordsFile)
{
	if (!segmentor.loadDict(dictFile))
		return false;
	if (hmmFile) segmentor.loadHMM(hmmFile);
	if (stopwordsFile) segmentor.loadStopwords(stopwordsFile);
	return true;
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * htmlFile)
{
	return parser.parse(FileReader::read_utf8_file(htmlFile));
}

CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage, bool useHMM, bool useStopwords)
{
	return segmentor.cut(passage, useHMM, useStopwords);
}


void filePathCvtCode(char *filepath) {
	wchar_t tmp[MAX_FLAG_LEN];
	MultiByteToWideChar(CP_UTF8, 0, filepath, -1, tmp, MAX_FLAG_LEN);
	WideCharToMultiByte(CP_ACP, 0, tmp, -1, filepath, MAX_FLAG_LEN, NULL, 0);
}

void readFilmInfo(const char *file, FilmInfo & info)
{
	std::wifstream wfin(file);
	wfin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstring buf;

	bool isFirst = true;
	CharStringLink *cur = nullptr;
	while (true) {
		wfin >> buf;
		if (wfin.eof()) break;

		if (isFirst) {
			info.setName(buf), isFirst = false;
			continue;
		}
		if (buf == L"导演:") { cur = &info._directors, cur->push_back(CharString()); }
		else if (buf == L"编剧:") { 
			cur = &info._screenwriters, cur->push_back(CharString()); }
		else if (buf == L"主演:") {
			cur = &info._stars, cur->push_back(CharString()); }
		else if (buf == L"类型:") {
			cur = &info._genres, cur->push_back(CharString()); }
		else if (buf == L"制片国家/地区:") {
			cur = &info._regions, cur->push_back(CharString()); }
		else if (buf == L"语言:") {
			cur = &info._languages, cur->push_back(CharString()); }
		else if (buf == L"上映日期:") {
			cur = &info._dates, cur->push_back(CharString()); }
		else if (buf == L"片长:") {
			cur = &info._durations, cur->push_back(CharString()); }
		else if (buf == L"又名:") {
			cur = &info._alternates, cur->push_back(CharString()); }
		else if (buf == L"标签:") {
			cur = &info._tags, cur->push_back(CharString()); }
		else if (buf == L"评分:") {
			wfin >> info._rating;
		}
		else if (buf == L"剧情简介:") {
			wchar_t line[1000];
			wfin.getline(line, 1000);
			while (!wfin.eof()) {
				wfin.getline(line, 1000);
				if (wcslen(line) == 0) continue;
				if (!info._introduction.empty()) info._introduction += L"\n";
				info._introduction += line;
			}
		}
		else if (buf == L"/") {
			if (cur) cur->push_back(CharString());
		}
		else {
			if (cur) {
				if (!cur->back().empty()) cur->back() += L" ";
				cur->back() += buf;
			}
		}
	}
	wfin.close();
}

void readFilmWord(const char *file, CharStringLink & cuts)
{
	std::wifstream wfin(file);
	wfin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstring word; 
	while (!wfin.eof()) {
		wfin >> word;
		if (wfin.eof()) break;
		cuts.push_back(word);
	}
	wfin.close();
}