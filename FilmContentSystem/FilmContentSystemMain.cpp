#include "FilmContentSystemApplication.h"
#include <fstream>
#include <sstream>
#include <cstring>

#include <locale>
#include <codecvt>

#include <direct.h>
#include <io.h>

using namespace std;

bool endsWith(const char * str, const char *end) {
	int len1 = strlen(str), len2 = strlen(end);
	return len1 >= len2 && 0 == strcmp(str + len1 - len2, str) == 0;
}

void _main() {
	FilmContentSystemApplication app; 

#ifdef _DEBUG
	const char dictFile[] = "C:\\Users\\Manchery\\Desktop\\实验1材料\\dict\\dict.txt";
	const char hmmFile[] = "C:\\Users\\Manchery\\Desktop\\实验1材料\\dict\\HMM.txt";
	const char inputDir[] = "C:\\Users\\Manchery\\Desktop\\实验1材料\\input\\";
	const char outputDir[] = "C:\\Users\\Manchery\\Desktop\\实验1材料\\output\\";
#else
	const char dictFile[] = "dict\\dict.txt";
	const char hmmFile[] = "dict\\HMM.txt";
	const char inputDir[] = "input\\";
	const char outputDir[] = "output\\";
#endif

	_mkdir(outputDir);

	app.initDictionary(dictFile, hmmFile);

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

			auto info = app.extractInfo(filePath);

			std::wofstream wfout(infoFile);
			wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			wfout << info;
			wfout.close();

			wfout.open(txtFile);
			wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
			CharStringLink cuts = app.divideWords(info.introduction());
			for (auto it = cuts.begin(); it != cuts.end(); ++it) {
				wfout << *it << std::endl;
			}
			wfout.close();

			/*------------------------------------------------------------------------------------*/
		}
	}
	_findclose(lf);
}

void _debug(const char *dictFile, const char *hmmFile, const char *inputFile, const char *infoFile, const char *txtFile) {
	FilmContentSystemApplication app;
	app.initDictionary(dictFile, hmmFile);
	auto info = app.extractInfo(inputFile);

	std::wofstream wfout(infoFile);
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	wfout << info;
	wfout.close();

	wfout.open(txtFile);
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	CharStringLink cuts = app.divideWords(info.introduction());
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		wfout << *it << std::endl;
	}
	wfout.close();
}

int main() {
	locale::global(locale("chs"));
	_debug("C:\\Users\\Manchery\\Desktop\\实验1材料\\dict\\dict.txt",
		"C:\\Users\\Manchery\\Desktop\\实验1材料\\dict\\HMM.txt",
		"C:\\Users\\Manchery\\Desktop\\实验1材料\\input\\19.html",
		"C:\\Users\\Manchery\\Desktop\\output.info",
		"C:\\Users\\Manchery\\Desktop\\output.txt");
	_CrtDumpMemoryLeaks();
	return 0;
}