#include "FilmContentSystemApplication.h"
#include <ctime>


FilmContentSystemApplication::FilmContentSystemApplication()
{
}


FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

void FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile)
{
	auto start = clock();
	seg.loadDict(dictFile);
	auto end = clock();
	std::cout << "Loading Dictionary times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;

	if (hmmFile) {
		start = clock();
		seg.loadHMM(hmmFile);
		end = clock();
		std::cout << "Loading HMM times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;
	}

	CharStringLink cuts = seg.cut(L"�˹���Forest �Σ���һ����һ����Ȯ����Ϊû����֪���������������������ˣ���顤���� Richard Gere �Σ���С��Ļ�վ��һֻ��ʧ��С����ڤڤ���ƺ�ע��С�������˽�������ĳ��Ե�֣�����...");
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
