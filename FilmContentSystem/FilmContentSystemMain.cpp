#include "FilmContentSystemApplication.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <locale>
#include <codecvt>
#include <ctime>

using namespace std;

void _main() {
	auto start = clock();

#ifdef _DEBUG
	FilmContentSystemApplication().run("../Debug/FilmContentSystem.config");
#else
	FilmContentSystemApplication().run();
#endif

	std::cerr << "Totally times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;
}

void _debug(const char *dictFile, const char *hmmFile, const char* stopwordsFile, const char *inputFile, const char *infoFile, const char *txtFile) {
	FilmContentSystemApplication app;
	app.initDictionary(dictFile, hmmFile, stopwordsFile);
	auto info = app.extractInfo(inputFile);

	std::wofstream wfout(infoFile);
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	wfout << info;
	wfout.close();

	wfout.open(txtFile);
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	CharStringLink cuts = app.divideWords(info.introduction(), true, true);
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		wfout << *it << std::endl;
	}
	wfout.close();
}

int main() {
	locale::global(locale("chs"));
	/*_debug("C:\\Users\\Manchery\\Desktop\\实验1材料\\dict_utf-8\\dict.txt",
		"C:\\Users\\Manchery\\Desktop\\实验1材料\\dict_utf-8\\HMM.txt",
		"C:\\Users\\Manchery\\Desktop\\实验1材料\\dict_utf-8\\stopwords.txt",
		"C:\\Users\\Manchery\\Desktop\\实验1材料\\input\\19.html",
		"C:\\Users\\Manchery\\Desktop\\output.info",
		"C:\\Users\\Manchery\\Desktop\\output.txt");*/
	_main();
	_CrtDumpMemoryLeaks();
	system("pause");
	return 0;
}