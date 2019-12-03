#include "FilmContentSystemApplication.h"
#include "FileReader.h"
#include "common.h"
#include <ctime>
#include <iostream>
#include <direct.h>
#include <io.h>

FilmContentSystemApplication::FilmContentSystemApplication()
{
	useHMM = useStopwords = false;
	dicLoaded = false;
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
		// ��������Ŀ¼������html�ļ�
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

			if (_access(infoFile, 0) == 0 && _access(txtFile, 0) == 0) { // �����ͷִʽ���Ѿ�����
				readFilmInfo(infoFile, filmInfos[docId]);
				readFilmWord(txtFile, filmWords[docId]);
			}
			else {
				std::cerr << "Parsing & processing file " << file.name << "..." << std::endl;
				// ���� html
				auto info = extractInfo(filePath);
				std::wofstream wfout(infoFile);
				wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
				wfout << info;
				wfout.close();
				filmInfos[docId] = info;

				// ���ķִ�
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
	std::cerr << "Total " << docCnt << " files. " << std::endl;
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

Vector<std::pair<int, int>> FilmContentSystemApplication::retrieve(const CharStringLink & keywords)
{
	using std::pair;
	struct data_t {
		int cnt, tot; // ��ͬ�������ܴ���
		int clk; // ʱ���
	};
	int clk = 0;
	SplayTree<int, data_t> Map;	// first����Ӱ id
	for (auto it = keywords.begin(); it != keywords.end(); ++it) {
		++clk;
		CharString word = *it;
		TermInfo term = wordIndex.search(word);
		for (auto p = term.list.begin(); p != term.list.end(); ++p) {
			data_t &data = Map[p.id()];
			if (data.clk != clk) data.clk = clk, data.cnt++;
			data.tot++;
		}
	}
	// TODO: speed optimize
	Vector<pair<int, data_t>> nodes = Map.list();
	typedef bool(*cmpFunc)(const pair<int, data_t>&, const pair<int, data_t>&);
	// TODO: debug
	nodes.sort(cmpFunc{ [](const pair<int, data_t>& a, const pair<int, data_t> &b) {
		data_t aD = a.second ,bD = b.second;
		return aD.cnt == bD.cnt ? aD.tot > bD.tot: aD.cnt > bD.cnt;
	} });
	Vector<pair<int, int>> res;
	for (int i = 0; i < nodes.size(); i++)
		res.push_back(std::make_pair(nodes[i].first, nodes[i].second.tot));
	return res;
}

Vector<std::pair<int, CharString>> FilmContentSystemApplication::recommend(const CharString & fileName)
{
	return Vector<std::pair<int, CharString>>();
}

void FilmContentSystemApplication::doRetrieve()
{
}

void FilmContentSystemApplication::doRecommend()
{
}

void FilmContentSystemApplication::run(const char * configFile)
{
	loadConfig(configFile != nullptr ? configFile : DEFAULT_CONFIG_PATH);

	loadDatabase();
	
	buildIndex();
}

bool FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile, const char *stopwordsFile)
{
	if (!segmentor.loadDict(dictFile))
		return false;
	if (hmmFile) segmentor.loadHMM(hmmFile);
	if (stopwordsFile) segmentor.loadStopwords(stopwordsFile);
	dicLoaded = true;
	return true;
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * htmlFile)
{
	return parser.parse(FileReader::read_utf8_file(htmlFile));
}


CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage, bool useHMM, bool useStopwords)
{
	if (!dicLoaded) {
		if (!initDictionary(dictFile, hmmFile, stopwordsFile)) {
			std::cerr << "Load dictionary failed !" << std::endl;
			return CharStringLink();
		}
	}
	return segmentor.cut(passage, useHMM, useStopwords);
}

// ���ļ�·��ת��Ϊ ANSI ����
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
		if (buf == L"����:") { cur = &info._directors, cur->push_back(CharString()); }
		else if (buf == L"���:") { 
			cur = &info._screenwriters, cur->push_back(CharString()); }
		else if (buf == L"����:") {
			cur = &info._stars, cur->push_back(CharString()); }
		else if (buf == L"����:") {
			cur = &info._genres, cur->push_back(CharString()); }
		else if (buf == L"��Ƭ����/����:") {
			cur = &info._regions, cur->push_back(CharString()); }
		else if (buf == L"����:") {
			cur = &info._languages, cur->push_back(CharString()); }
		else if (buf == L"��ӳ����:") {
			cur = &info._dates, cur->push_back(CharString()); }
		else if (buf == L"Ƭ��:") {
			cur = &info._durations, cur->push_back(CharString()); }
		else if (buf == L"����:") {
			cur = &info._alternates, cur->push_back(CharString()); }
		else if (buf == L"��ǩ:") {
			cur = &info._tags, cur->push_back(CharString()); }
		else if (buf == L"����:") {
			wfin >> info._rating;
		}
		else if (buf == L"������:") {
			wchar_t line[1000];
			wfin.getline(line, 1000);
			while (!wfin.eof()) {
				wfin.getline(line, 1000);
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
	while (true) {
		wfin >> word;
		if (wfin.eof()) break;
		cuts.push_back(word);
	}
	wfin.close();
}