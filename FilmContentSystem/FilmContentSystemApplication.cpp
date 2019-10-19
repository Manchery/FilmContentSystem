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
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * filename)
{
	return FilmInfo();
}

CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage)
{
	return CharStringLink();
}
