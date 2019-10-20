#include "FilmContentSystemApplication.h"
#include <fstream>
#include <sstream>
#include <locale>
#include <codecvt>
using namespace std;

void _main() {
	FilmContentSystemApplication app; 

	app.initDictionary("C:\\Users\\Manchery\\Desktop\\jieba\\dict2.txt", "C:\\Users\\Manchery\\Desktop\\jieba\\HMM3.txt");
	auto info = app.extractInfo("C:\\Users\\Manchery\\Desktop\\ÊµÑé1²ÄÁÏ\\input\\11.html");

	std::wofstream wfout("C:\\Users\\Manchery\\Desktop\\output.txt");
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	wfout << info;
	wfout.close();

	wfout.open("C:\\Users\\Manchery\\Desktop\\output2.txt");
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	CharStringLink cuts = app.divideWords(info.introduction());
	for (auto it = cuts.begin(); it != cuts.end(); ++it) {
		wfout << *it << std::endl;
	}
	wfout.close();
}

int main() {
	locale::global(locale("chs"));
	_main();
	_CrtDumpMemoryLeaks();
	return 0;
}