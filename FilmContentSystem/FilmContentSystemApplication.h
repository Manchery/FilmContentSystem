#pragma once

#include "FilmInfo.h"
#include "WordSegmentor.h"

class FilmContentSystemApplication
{
private:
	WordSegmentor seg;
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	void initDictionary(const char *dictFile, const char *hmmFile = nullptr);
	FilmInfo extractInfo(const char *filename);
	CharStringLink divideWords(const CharString &passage);
};

