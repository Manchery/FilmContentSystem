#pragma once

#include <string>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>
#include <windows.h>

const int BUF_SIZE = 100000;

/*
FileReader
说明：	提供从文件的快速读入操作
		实现方法为 通过 fread 从文件读取一片缓存
接口：	各接口详见成员函数声明
*/

class FileReader
{
private:
	char *buf, *p1, *p2;
	FILE* file; errno_t err;
public:
	FileReader();
	FileReader(const char *filename);
	~FileReader();

	// 文件是否打开成功
	bool bad() const { return err; }
	void set_file(const char *filename);

	// 通过 fread 从file读取一片缓存，实现比标准的getchar更快的读入
	char nextchar();
	// 清除缓存
	void clear_buf();
	// 读入一个int，通过fast_getchar实现
	void read(int &x);
	// 读入一个char，通过fast_getchar实现
	int read(char *s);
	// 读入一个double，通过fast_getchar实现
	void read(double &x);

	// 以下函数用于读取config文件
	// 读取由 A-Z 以及 _ 组成的flag名，调用fast_getchar
	int read_flag_name(char *s, char &nextChar);
	// 读取由引号包裹的字符串类型的flag的值，调用fast_getchar
	int read_string_flag(char *s);
	// 读取bool类型的flag的值，合法的值应为true或false的大写或小写，调用fast_getchar
	bool read_bool_flag();

	// 读入 utf-8 编码的文件，将所有内容作为wstring返回
	// Reference: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
	static std::wstring read_utf8_file(const char* filename);
};

