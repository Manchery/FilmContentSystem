#pragma once

#include "CharString.h"
#include "CharStringLink.h"
#include <iostream>

/*
FilmInfo
˵����	�洢��Ӱ��Ϣ�����������
		��Ӱ��Ϣ�ֱ��У���Ӱ�������ݼ�顢���ݡ���硢���ݡ����͡����������ԡ���ӳ���ڡ�ʱ��������
�ӿڣ�	get/set ����Ϣ
		���
*/

class HtmlParser;
class FilmInfo
{
	friend class HtmlParser;
private:
	// ��Ӱ�������ݼ��
	CharString _name, _introduction;
	// ���ݡ���硢���ݡ����͡����������ԡ���ӳ���ڡ�ʱ��������
	CharStringLink _directors, _screenwriters, _stars, _genres, _regions, _languages, _dates, _durations, _alternates;
	CharStringLink _tags;	// ��ǩ
	double _rating;			// ����

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
	const CharStringLink& tags() const { return _tags; }
	double rating() const { return _rating; }

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
	void addTag(const CharString& x) { _tags.add(x); }
	void setRating(double rating) { _rating = rating; }

	friend std::wostream & operator<<(std::wostream & os, const FilmInfo & info);
	friend void readFilmInfo(const char *file, FilmInfo &info);
};

extern std::wostream & operator<<(std::wostream & os, const FilmInfo & info);

