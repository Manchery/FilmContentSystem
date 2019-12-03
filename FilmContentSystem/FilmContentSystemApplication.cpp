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
		filmInfos.reserve(2000);
		filmWords.reserve(2000);
		// 遍历输入目录下所有html文件
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if (!endsWith(file.name, ".html"))
				continue;

			std::cerr << "Found file " << file.name << "..." << std::endl;

			char baseName[MAX_FILE_NAME_LEN] = { 0 };
			strncpy_s(baseName, file.name, strlen(file.name) - 5);
			char filePath[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(filePath, inputDir); strcat_s(filePath, file.name);
			char infoFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(infoFile, outputDir); strcat_s(infoFile, baseName); strcat_s(infoFile, ".info");
			char txtFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(txtFile, outputDir); strcat_s(txtFile, baseName); strcat_s(txtFile, ".txt");

			int docId = atoi(baseName);
			if (docId >= filmInfos.capacity()) filmInfos.reserve(docId), filmWords.reserve(docId);

			if (_access(infoFile, 0) == 0 && _access(txtFile, 0) == 0) { // 解析和分词结果已经存在
				readFilmInfo(infoFile, filmInfos[docId]);
				readFilmWord(txtFile, filmWords[docId]);
			}
			else {
				std::cerr << "Processing file " << file.name << "..." << std::endl;
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
		}
	}
	_findclose(lf);
}

void FilmContentSystemApplication::buildIndex()
{
}

void FilmContentSystemApplication::run(const char * configFile)
{
	loadConfig(configFile != nullptr ? configFile : DEFAULT_CONFIG_PATH);

	loadDatabase();
	
	//buildIndex();
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

// 将文件路径转化为 ANSI 编码
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
	while (!wfin.eof()) {
		wfin >> buf;
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
		cuts.push_back(word);
	}
	wfin.close();
}