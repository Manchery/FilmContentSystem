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
˵����	�ṩ���ļ��Ŀ��ٶ������
		ʵ�ַ���Ϊ ͨ�� fread ���ļ���ȡһƬ����
�ӿڣ�	���ӿ������Ա��������
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

	// �ļ��Ƿ�򿪳ɹ�
	bool bad() const { return err; }
	void set_file(const char *filename);

	// ͨ�� fread ��file��ȡһƬ���棬ʵ�ֱȱ�׼��getchar����Ķ���
	char nextchar();
	// �������
	void clear_buf();
	// ����һ��int��ͨ��fast_getcharʵ��
	void read(int &x);
	// ����һ��char��ͨ��fast_getcharʵ��
	int read(char *s);
	// ����һ��double��ͨ��fast_getcharʵ��
	void read(double &x);

	// ���º������ڶ�ȡconfig�ļ�
	// ��ȡ�� A-Z �Լ� _ ��ɵ�flag��������fast_getchar
	int read_flag_name(char *s, char &nextChar);
	// ��ȡ�����Ű������ַ������͵�flag��ֵ������fast_getchar
	int read_string_flag(char *s);
	// ��ȡbool���͵�flag��ֵ���Ϸ���ֵӦΪtrue��false�Ĵ�д��Сд������fast_getchar
	bool read_bool_flag();

	// ���� utf-8 ������ļ���������������Ϊwstring����
	// Reference: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
	static std::wstring read_utf8_file(const char* filename);
};

