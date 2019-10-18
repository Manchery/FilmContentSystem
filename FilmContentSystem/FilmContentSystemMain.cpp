#include "FilmContentSystemApplication.h"
using namespace std;

void _main() {
	FilmContentSystemApplication app;
	app.initDictionary("C:\\Users\\Manchery\\Desktop\\jieba\\dict2.txt");
}

int main() {
	locale::global(locale("chs"));
	//std::ios::sync_with_stdio(false);
	//std::cin.tie(0);

	_main();
	_CrtDumpMemoryLeaks();
	putchar('*');
	while (true);
	return 0;
}