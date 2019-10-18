#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

int main() {
	locale::global(locale("chs"));
	wstring wstr;
	freopen("test.txt","r",stdin);
	wcin >> wstr;
	cout << wstr.length() << endl;
	wcout << wstr << endl;
	//system("pause");
	return 0;
}