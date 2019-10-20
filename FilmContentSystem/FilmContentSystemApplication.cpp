#include "FilmContentSystemApplication.h"
#include "IO.h"
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
	std::cerr << "Loading Dictionary times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

	if (hmmFile) {
		start = clock();
		segmentor.loadHMM(hmmFile);
		std::cerr << "Loading HMM times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;
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

CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage)
{
	auto start = clock();
	CharStringLink res = segmentor.cut(passage);
	std::cerr << "Segmenting summary times " << ((double)clock() - start) / CLOCKS_PER_SEC << std::endl;

	return res;

	//return segmentor.cut(passage);
}
