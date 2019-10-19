#pragma once

#include "FilmInfo.h"
#include "WordSegmentor.h"
#include "HtmlParser.h"

class FilmContentSystemApplication
{
private:
	WordSegmentor seg;
	HtmlParser parser;
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	void initDictionary(const char *dictFile, const char *hmmFile = nullptr);
	FilmInfo extractInfo(const char *htmlFile);
	CharStringLink divideWords(const CharString &passage);
};

