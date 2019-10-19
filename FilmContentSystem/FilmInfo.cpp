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
	os << L"����: " << info._directors << std::endl;
	os << L"���: " << info._screenwriters << std::endl;
	os << L"����: " << info._stars << std::endl;
	os << L"����: " << info._genres << std::endl;
	os << L"��Ƭ����/����: " << info._regions << std::endl;
	os << L"����: " << info._languages << std::endl;
	os << L"��ӳ����: " << info._dates << std::endl;
	os << L"Ƭ��: " << info._durations << std::endl;
	os << L"����: " << info._alternates << std::endl;
	os << L"������: \n" << info._introduction << std::endl;
	return os;
}
