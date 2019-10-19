#include "FilmContentSystemApplication.h"
#include <ctime>


FilmContentSystemApplication::FilmContentSystemApplication()
{
}


FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

void FilmContentSystemApplication::initDictionary(const char * filename)
{
	auto start = clock();
	seg.loadDict(filename);
	auto end = clock();
	std::cout << "Loading Dictionary times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;

	CharStringLink cuts = seg.cut(L"段小楼（张丰毅）与程蝶衣（张国荣）是一对打小一起长大的师兄弟，两人一个演生，一个饰旦，一向配合天衣无缝，尤其一出《霸王别姬》，更是誉满京城，为此，两人约定合演一辈子《霸王别姬》。但两人对戏剧与人生关系...");
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		std::wcout << *it << std::endl;
	}
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * filename)
{
	return FilmInfo();
}

CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage)
{
	return CharStringLink();
}
