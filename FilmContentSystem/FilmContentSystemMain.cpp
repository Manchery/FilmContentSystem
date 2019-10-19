#include "FilmContentSystemApplication.h"
using namespace std;

void _main() {
	FilmContentSystemApplication app;
	app.initDictionary("C:\\Users\\Manchery\\Desktop\\jieba\\dict2.txt", "C:\\Users\\Manchery\\Desktop\\jieba\\HMM3.txt");
}

int main() {
	locale::global(locale("chs"));

	_main();
	_CrtDumpMemoryLeaks();
	putchar('*');
	while (true);
	return 0;
}