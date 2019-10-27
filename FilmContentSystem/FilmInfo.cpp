#include "FilmInfo.h"



FilmInfo::FilmInfo()
{
}


FilmInfo::~FilmInfo()
{
}

std::wostream & operator<<(std::wostream & os, const FilmInfo & info)
{
	os << info._name << std::endl;
	if (!info._directors.empty()) os << L"导演: " << info._directors << std::endl;
	if (!info._screenwriters.empty()) os << L"编剧: " << info._screenwriters << std::endl;
	if (!info._stars.empty()) os << L"主演: " << info._stars << std::endl;
	if (!info._genres.empty()) os << L"类型: " << info._genres << std::endl;
	if (!info._regions.empty()) os << L"制片国家/地区: " << info._regions << std::endl;
	if (!info._languages.empty()) os << L"语言: " << info._languages << std::endl;
	if (!info._dates.empty()) os << L"上映日期: " << info._dates << std::endl;
	if (!info._durations.empty()) os << L"片长: " << info._durations << std::endl;
	if (!info._alternates.empty()) os << L"又名: " << info._alternates << std::endl;
	if (!info._introduction.empty()) os << L"剧情简介: \n" << info._introduction << std::endl;
	return os;
}
