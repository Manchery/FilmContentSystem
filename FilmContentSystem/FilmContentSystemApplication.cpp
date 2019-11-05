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
}

FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

// 将文件路径转化为 ANSI 编码
static void filePathCvtCode(char *filepath) {
	wchar_t tmp[MAX_FLAG_LEN];
	MultiByteToWideChar(CP_UTF8, 0, filepath, -1, tmp, MAX_FLAG_LEN);
	WideCharToMultiByte(CP_ACP, 0, tmp, -1, filepath, MAX_FLAG_LEN, NULL, 0);
}

void FilmContentSystemApplication::loadConfig(const char * configFile)
{
	FileReader f(configFile);
	if (!f.bad()) {
		char flag[MAX_FLAG_LEN], nextChar;
		while (f.read_flag_name(flag, nextChar) != -1) {
			if (strcmp(flag, "INPUT_DIR") == 0) {
				if (nextChar != '=') while (f.nextchar() != '=');
				f.read_string_flag(inputDir);
				filePathCvtCode(inputDir);
				if (!endsWith(inputDir, "/")) 
					strcat_s(inputDir, "/");
			}
			else if (strcmp(flag, "OUTPUT_DIR") == 0) {
				if (nextChar != '=') while (f.nextchar() != '=');
				f.read_string_flag(outputDir);
				filePathCvtCode(outputDir);
				if (!endsWith(outputDir, "/")) 
					strcat_s(outputDir, "/");
			}
			else if (strcmp(flag, "DICT_PATH") == 0) {
				if (nextChar != '=') while (f.nextchar() != '=');
				f.read_string_flag(dictFile);
				filePathCvtCode(dictFile);
			}
			else if (strcmp(flag, "HMM_PATH") == 0) {
				if (nextChar != '=') while (f.nextchar() != '=');
				f.read_string_flag(hmmFile);
				filePathCvtCode(hmmFile);
			}
			else if (strcmp(flag, "STOP_PATH") == 0) {
				if (nextChar != '=') while (f.nextchar() != '=');
				f.read_string_flag(stopwordsFile);
				filePathCvtCode(stopwordsFile);
			}
			else if (strcmp(flag, "USE_HMM") == 0) {
				if (nextChar != '=') while (f.nextchar() != '=');
				useHMM = f.read_bool_flag();
			}
			else if (strcmp(flag, "USE_STOP") == 0) {
				if (nextChar != '=') while (f.nextchar() != '=');
				useStopwords = f.read_bool_flag();
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

void FilmContentSystemApplication::run(const char * configFile)
{
	if (configFile != nullptr)
		loadConfig(configFile);
	else
		loadConfig(DEFAULT_CONFIG_PATH);

	_mkdir(outputDir);

	if (!initDictionary(dictFile, hmmFile, stopwordsFile)) return;

	_finddata_t file;
	long lf; const int MAX_FILE_NAME_LEN = 1000;
	char inputReg[MAX_FILE_NAME_LEN] = { 0 };
	strcat_s(inputReg, inputDir); strcat_s(inputReg, "*");
	if ((lf = _findfirst(inputReg, &file)) == -1) {
		std::cerr << inputDir << " not found!" << std::endl;
	}
	else {
		// 遍历输入目录下所有html文件
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if (!endsWith(file.name, ".html"))
				continue;

			std::cerr << "Found " << file.name << "..." << std::endl;

			char baseName[MAX_FILE_NAME_LEN] = { 0 };
			strncpy_s(baseName, file.name, strlen(file.name) - 5);
			char filePath[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(filePath, inputDir); strcat_s(filePath, file.name);
			char infoFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(infoFile, outputDir); strcat_s(infoFile, baseName); strcat_s(infoFile, ".info");
			char txtFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat_s(txtFile, outputDir); strcat_s(txtFile, baseName); strcat_s(txtFile, ".txt");

			/*------------------------------------------------------------------------------------*/

			// 解析 html
			auto info = extractInfo(filePath);

			std::wofstream wfout(infoFile);
			wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			wfout << info;
			wfout.close();

			wfout.open(txtFile);
			wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			// 中文分词
			CharStringLink cuts = divideWords(info.introduction(), useHMM, useStopwords);
			for (auto it = cuts.begin(); it != cuts.end(); ++it) {
				wfout << *it << std::endl;
			}
			wfout.close();

			/*------------------------------------------------------------------------------------*/
		}
	}
	_findclose(lf);
}

bool FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile, const char *stopwordsFile)
{
	auto start = clock();
	if (!segmentor.loadDict(dictFile))
		return false;
	std::cerr << "Loading Dictionary times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

	if (hmmFile) {
		start = clock();
		segmentor.loadHMM(hmmFile);
		std::cerr << "Loading HMM times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;
	}

	if (stopwordsFile) {
		start = clock();
		segmentor.loadStopwords(stopwordsFile);
		std::cerr << "Loading Stopwords times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;
	}
	return true;
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * htmlFile)
{
	auto start = clock();
	std::wstring wfile = FileReader::read_utf8_file(htmlFile); 
	std::cerr << "Reading Html times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;
	
	start = clock();
	FilmInfo info = parser.parse(wfile);
	std::cerr << "Parsing Html times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

	return info;

	//return parser.parse(FileReader::read_utf8_file(htmlFile));
}


CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage, bool useHMM, bool useStopwords)
{
	auto start = clock();
	CharStringLink res = segmentor.cut(passage, useHMM, useStopwords);
	std::cerr << "Segmenting summary times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

	return res;

	//return segmentor.cut(passage, useHMM, useStopwords);
}
