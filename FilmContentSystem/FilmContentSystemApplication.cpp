#include "FilmContentSystemApplication.h"
#include "FastIO.h"
#include <ctime>
#include <iostream>


FilmContentSystemApplication::FilmContentSystemApplication()
{
}


FilmContentSystemApplication::~FilmContentSystemApplication()
{
}

void FilmContentSystemApplication::initDictionary(const char * dictFile, const char * hmmFile)
{
	auto start = clock();
	segmentor.loadDict(dictFile);
	auto end = clock();
	std::cout << "Loading Dictionary times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;

	if (hmmFile) {
		start = clock();
		segmentor.loadHMM(hmmFile);
		end = clock();
		std::cout << "Loading HMM times " << ((double)end - start) / CLOCKS_PER_SEC << std::endl;
	}

	/*CharStringLink cuts = seg.cut(L"�˹���Forest �Σ���һ����һ����Ȯ����Ϊû����֪���������������������ˣ���顤���� Richard Gere �Σ���С��Ļ�վ��һֻ��ʧ��С����ڤڤ���ƺ�ע��С�������˽�������ĳ��Ե�֣�����...");
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		std::wcout << *it << std::endl;
	}*/
}

FilmInfo FilmContentSystemApplication::extractInfo(const char * htmlFile)
{
	// TODO: optimize input
	/*freopen(htmlFile, "r", stdin);
	int len = 0; const int max_len = 500000;
	char *file = new char[max_len], c;
	while ((c = fast_getchar()) != EOF) file[len++] = c; file[len] = 0;
	freopen("CON", "r", stdin); */
	
	return parser.parse(readFile(htmlFile));
}

CharStringLink FilmContentSystemApplication::divideWords(const CharString & passage)
{
	return segmentor.cut(passage);
}
