#include "FilmContentSystemApplication.h"
#include <ctime>


FilmContentSystemApplication::FilmContentSystemApplication()
{
}


FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

void FilmContentSystemApplication::initDictionary(const char * filename)
{
	auto start = clock();
	seg.loadDict(filename);
	auto end = clock();
	std::cout << "Loading Dictionary times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;

	CharStringLink cuts = seg.cut(L"��С¥���ŷ��㣩��̵��£��Ź��٣���һ�Դ�Сһ�𳤴��ʦ�ֵܣ�����һ��������һ���ε���һ����������޷죬����һ���������𼧡��������������ǣ�Ϊ�ˣ�����Լ������һ���ӡ������𼧡��������˶�Ϸ����������ϵ...");
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		std::wcout << *it << std::endl;
	}
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * filename)
{
	return FilmInfo();
}

CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage)
{
	return CharStringLink();
}
