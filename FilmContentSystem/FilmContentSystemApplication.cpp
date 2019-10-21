#include "FilmContentSystemApplication.h"
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

void FilmContentSystemApplication::loadConfig(const char * configFile)
{
	if (freopen(configFile, "r", stdin)) {
		const int MAX_LEN = 100;
		char flag[MAX_LEN], nextChar;
		while (read_flag_name(flag, nextChar) != -1) {
			if (strcmp(flag, "INPUT_DIR") == 0) {
				if (nextChar != '=') while (fast_getchar() != '=');
				read_string_flag(inputDir);
				if (!endsWith(inputDir, "/")) 
					strcat(inputDir, "/");
			}
			else if (strcmp(flag, "OUTPUT_DIR") == 0) {
				if (nextChar != '=') while (fast_getchar() != '=');
				read_string_flag(outputDir);
				if (!endsWith(outputDir, "/")) 
					strcat(outputDir, "/");
			}
			else if (strcmp(flag, "DICT_PATH") == 0) {
				if (nextChar != '=') while (fast_getchar() != '=');
				read_string_flag(dictFile);
			}
			else if (strcmp(flag, "HMM_PATH") == 0) {
				if (nextChar != '=') while (fast_getchar() != '=');
				read_string_flag(hmmFile);
			}
			else if (strcmp(flag, "STOP_PATH") == 0) {
				if (nextChar != '=') while (fast_getchar() != '=');
				read_string_flag(stopwordsFile);
			}
			else if (strcmp(flag, "USE_HMM") == 0) {
				if (nextChar != '=') while (fast_getchar() != '=');
				useHMM = read_bool_flag();
			}
			else if (strcmp(flag, "USE_STOP") == 0) {
				if (nextChar != '=') while (fast_getchar() != '=');
				useStopwords = read_bool_flag();
			}
		}
	}
	else { // by Default
		strcmp(inputDir, DEFAULT_INPUT_DIR);
		strcmp(outputDir, DEFAULT_OUTPUT_DIR);
		strcmp(dictFile, DEFAULT_DICT_PATH);
		strcmp(hmmFile, DEFAULT_HMM_PATH);
		strcmp(stopwordsFile, DEFAULT_STOP_PATH);
		useHMM = DEFAULT_USE_HMM; 
		useStopwords = DEFAULT_USE_STOP;
	}
}

void FilmContentSystemApplication::run()
{
#ifdef _DEBUG
	loadConfig("../Debug/FilmContentSystem.config");
#else
	loadConfig(CONFIG_PATH);
#endif

	_mkdir(outputDir);

	initDictionary(dictFile, hmmFile, stopwordsFile);

	_finddata_t file;
	long lf; const int MAX_FILE_NAME_LEN = 1000;
	char inputReg[MAX_FILE_NAME_LEN] = { 0 };
	strcat(strcat(inputReg, inputDir), "*");
	if ((lf = _findfirst(inputReg, &file)) == -1) {
		std::cerr << inputDir << " not found!" << std::endl;
	}
	else {
		while (_findnext(lf, &file) == 0) {
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if (!endsWith(file.name, ".html"))
				continue;

			std::cerr << "Found " << file.name << "..." << std::endl;

			char baseName[MAX_FILE_NAME_LEN] = { 0 };
			strncpy(baseName, file.name, strlen(file.name) - 5);
			char filePath[MAX_FILE_NAME_LEN] = { 0 };
			strcat(strcat(filePath, inputDir), file.name);
			char infoFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat(strcat(strcat(infoFile, outputDir), baseName), ".info");
			char txtFile[MAX_FILE_NAME_LEN] = { 0 };
			strcat(strcat(strcat(txtFile, outputDir), baseName), ".txt");

			/*------------------------------------------------------------------------------------*/

			auto info = extractInfo(filePath);

			std::wofstream wfout(infoFile);
			wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			wfout << info;
			wfout.close();

			wfout.open(txtFile);
			wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
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

void FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile, const char *stopwordsFile)
{
	auto start = clock();
	segmentor.loadDict(dictFile);
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
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * htmlFile)
{
	auto start = clock();
	std::wstring wfile = read_utf8_file(htmlFile); 
	std::cerr << "Reading Html times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;
	
	start = clock();
	FilmInfo info = parser.parse(wfile);
	std::cerr << "Parsing Html times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

	return info;

	//return parser.parse(read_utf8_file(htmlFile));
}


CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage, bool useHMM, bool useStopwords)
{
	auto start = clock();
	CharStringLink res = segmentor.cut(passage, useHMM, useStopwords);
	std::cerr << "Segmenting summary times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

	return res;

	//return segmentor.cut(passage);
}
