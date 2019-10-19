#include "FilmContentSystemApplication.h"
#include <fstream>
#include <sstream>
using namespace std;

void _main() {
	FilmContentSystemApplication app; 
	
	//app.initDictionary("C:\\Users\\Manchery\\Desktop\\jieba\\dict2.txt", "C:\\Users\\Manchery\\Desktop\\jieba\\HMM3.txt");

	wostringstream wsout;
	wsout << app.extractInfo("C:\\Users\\Manchery\\Desktop\\ÊµÑé1²ÄÁÏ\\input\\19.html");
	wstring output = wsout.str();

	std::wofstream outFile("C:\\Users\\Manchery\\Desktop\\output.txt");
	outFile << output;

	/*std::ofstream outFile("C:\\Users\\Manchery\\Desktop\\output.txt", std::ios::out);
	outFile.write((char *)output.c_str(), output.length() * sizeof(wchar_t));
	outFile.close();*/

	// wfout <<'/'<< L"\u2002";
}

int main() {
	locale::global(locale("chs"));

	_main();
	_CrtDumpMemoryLeaks();
	putchar('*');
	while (true);
	return 0;
}