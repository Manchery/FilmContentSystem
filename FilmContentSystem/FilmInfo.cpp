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
	os << L"导演: " << info._directors << std::endl;
	os << L"编剧: " << info._screenwriters << std::endl;
	os << L"主演: " << info._stars << std::endl;
	os << L"类型: " << info._genres << std::endl;
	os << L"制片国家/地区: " << info._regions << std::endl;
	os << L"语言: " << info._languages << std::endl;
	os << L"上映日期: " << info._dates << std::endl;
	os << L"片长: " << info._durations << std::endl;
	os << L"又名: " << info._alternates << std::endl;
	os << L"剧情简介: \n" << info._introduction << std::endl;
	return os;
}
