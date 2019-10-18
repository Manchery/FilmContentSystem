#pragma once

#include "FilmInfo.h"

class FilmContentSystemApplication
{
public:
	FilmContentSystemApplication();
	~FilmContentSystemApplication();

	void initDictionary(const char *filename);
	FilmInfo extractInfo(const char *filename);
	CharStringLink divideWords(const CharString &passage);
};

