#include "FilmInfo.h"
#include <iomanip>


FilmInfo::FilmInfo()
{
	_rating = -1;
}


FilmInfo::~FilmInfo()
{
}

std::wostream & operator<<(std::wostream & os, const FilmInfo & info)
{
	os.setf(std::ios::fixed);

	os << info._name << std::endl;
	if (!info._directors.empty()) os << L"����: " << info._directors << std::endl;
	if (!info._screenwriters.empty()) os << L"���: " << info._screenwriters << std::endl;
	if (!info._stars.empty()) os << L"����: " << info._stars << std::endl;
	if (!info._genres.empty()) os << L"����: " << info._genres << std::endl;
	if (!info._regions.empty()) os << L"��Ƭ����/����: " << info._regions << std::endl;
	if (!info._languages.empty()) os << L"����: " << info._languages << std::endl;
	if (!info._dates.empty()) os << L"��ӳ����: " << info._dates << std::endl;
	if (!info._durations.empty()) os << L"Ƭ��: " << info._durations << std::endl;
	if (!info._alternates.empty()) os << L"����: " << info._alternates << std::endl;
	if (!info._tags.empty()) os << L"��ǩ: " << info._tags << std::endl;
	if (info._rating != -1) os << L"����: " << std::fixed << std::setprecision(1) << info._rating << std::endl;
	if (!info._introduction.empty()) os << L"������: \n" << info._introduction << std::endl;

	os.unsetf(std::ios::fixed);
	return os;
}
