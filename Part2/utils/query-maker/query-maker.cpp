#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

int cnt;
wstring word[1000005];
int cont[1000005];

inline void make_query1(){
	wifstream wfin("dict/dict.txt"); 
	wfin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	cnt=0;
	while (!wfin.eof()){
		wstring x,y;
		wfin>>word[cnt]>> cont[cnt]>>y; cnt++;
		if (wfin.eof()) {
			cnt--;
			break;
		}
	}
	wfin.close();
	wofstream wfout("query1.txt");
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	int n=100000;
	while (n--){
		int m = rand()%5+1;
		for (int i=1;i<=m;i++){
			int x=rand()%cnt;
			while (cont[x]<1000) x=rand()%cnt;
			wfout<<word[x]<<' ';
		}
		wfout<<endl;
	}
	wfout.close();
}

wstring title[2005];

inline void make_query2(){
	int N=1083;
	for (int i=0;i<N;i++){
		char buf[105];
		sprintf_s(buf, "input/info/%d.info", i);
		wifstream wfin(buf);
		wfin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
		wfin>>title[i];
		wfin.close();
	}
	wofstream wfout("query2.txt");
	wfout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	int n=100000;
	while (n--){
		wfout<<title[rand()%N]<<endl;
	}
	wfout.close();
}

int main(){
	srand(time(0));
	locale::global(locale("chs"));
	make_query1();
	make_query2();
	return 0;
}
