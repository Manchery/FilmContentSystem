#pragma once

#include "CharString.h"
#include "CharStringLink.h"

class FilmInfo
{
private:
	CharString _name, _introduction;
	CharStringLink _directors, _screenwriters, _stars, _genres, _regions, _languages, _dates, _durations, _alternates;

public:
	FilmInfo();
	~FilmInfo();
	const CharString& name() const { return _name; }
	const CharString& introduction() const { return _introduction; }
	const CharStringLink& directors() const { return _directors; }
	const CharStringLink& screenwriters() const { return _screenwriters; }
	const CharStringLink& stars() const { return _stars; }
	const CharStringLink& genres() const { return _genres; }
	const CharStringLink& regions() const { return _regions; }
	const CharStringLink& languages() const { return _languages; }
	const CharStringLink& dates() const { return _dates; }
	const CharStringLink& durations() const { return _durations; }
	const CharStringLink& alternates() const { return _alternates; }

	void setName(const CharString& name) { _name = name; }
	void setIntroduction(const CharString& introduction) { _introduction = introduction; }
	void addDirector(const CharString& x) { _directors.add(x); }
	void addScreenwriter(const CharString& x) { _screenwriters.add(x); }
	void addStar(const CharString& x) { _stars.add(x); }
	void addGenre(const CharString& x) { _genres.add(x); }
	void addRegion(const CharString& x) { _regions.add(x); }
	void addLanguage(const CharString& x) { _languages.add(x); }
	void addDate(const CharString& x) { _dates.add(x); }
	void addDuration(const CharString& x) { _durations.add(x); }
	void addAlternate(const CharString& x) { _alternates.add(x); }
};

