#include "FilmContentSystemApplication.h"
#include "FastIO.h"
#include <ctime>
#include <iostream>


FilmContentSystemApplication::FilmContentSystemApplication()
{
}


FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

void FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile)
{
	auto start = clock();
	segmentor.loadDict(dictFile);
	auto end = clock();
	std::cout << "Loading Dictionary times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;

	if (hmmFile) {
		start = clock();
		segmentor.loadHMM(hmmFile);
		end = clock();
		std::cout << "Loading HMM times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;
	}

	/*CharStringLink cuts = seg.cut(L"八公（Forest 饰）是一条谜一样的犬，因为没有人知道它从哪里来。教授帕克（理查·基尔 Richard Gere 饰）在小镇的火车站拣到一只走失的小狗，冥冥中似乎注定小狗和帕克教授有着某种缘分，帕克...");
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		std::wcout << *it << std::endl;
	}*/
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
	return segmentor.cut(passage);
}
