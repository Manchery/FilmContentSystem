#include "FilmContentSystemApplication.h"
#include "FastIO.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <sstream>


FilmContentSystemApplication::FilmContentSystemApplication()
{
}


FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

void FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile)
{
	auto start = clock();
	seg.loadDict(dictFile);
	auto end = clock();
	std::cout << "Loading Dictionary times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;

	if (hmmFile) {
		start = clock();
		seg.loadHMM(hmmFile);
		end = clock();
		std::cout << "Loading HMM times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;
	}

	/*CharStringLink cuts = seg.cut(L"八公（Forest 饰）是一条谜一样的犬，因为没有人知道它从哪里来。教授帕克（理查·基尔 Richard Gere 饰）在小镇的火车站拣到一只走失的小狗，冥冥中似乎注定小狗和帕克教授有着某种缘分，帕克...");
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		std::wcout << *it << std::endl;
	}*/
}

// Ref: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
static std::wstring readFile(const char* filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * htmlFile)
{
	// TODO: optimize input
	/*freopen(htmlFile, "r", stdin);
	int len = 0; const int max_len = 500000;
	char *file = new char[max_len], c;
	while ((c = fast_getchar()) != EOF) file[len++] = c; file[len] = 0;
	freopen("CON", "r", stdin); */
	
	return parser.parse(readFile(htmlFile));
}

CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage)
{
	return seg.cut(passage);
}
