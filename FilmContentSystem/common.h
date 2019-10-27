#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <locale>
#include <codecvt>
#include <fstream>
#include <sstream>

/*
common.h
˵����	���ù���������ʵ��
*/

// ͨ�� fread ��stdin��ȡһƬ���棬ʵ�ֱ�getchar����Ķ���
// !!! �����������������
extern char fast_getchar();
// �������
extern void clear_buf();
// ����һ��int��ͨ��fast_getcharʵ��
extern void fast_read(int &x);
// ����һ��char��ͨ��fast_getcharʵ��
extern int fast_read(char *s);
// ����һ��double��ͨ��fast_getcharʵ��
extern void fast_read(double &x);

// ���º������ڶ�ȡconfig�ļ�
// ��ȡ�� A-Z �Լ� _ ��ɵ�flag��������fast_getchar
extern int read_flag_name(char *s, char &nextChar);
// ��ȡ�����Ű������ַ������͵�flag��ֵ������fast_getchar
extern int read_string_flag(char *s);
// ��ȡbool���͵�flag��ֵ���Ϸ���ֵӦΪtrue��false�Ĵ�д��Сд������fast_getchar
extern bool read_bool_flag();

// ���� utf-8 ������ļ�
// Reference: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
extern std::wstring read_utf8_file(const char* filename);

extern bool isLower(wchar_t w);		// �Ƿ���Сд��ĸ a-z
extern bool isUpper(wchar_t w);		// �Ƿ��Ǵ�д��ĸ A-Z
extern bool isDigit(wchar_t w);		// �Ƿ������� 0-9
extern bool isAlpha(wchar_t w);		// �Ƿ�����ĸ isLower(w) || isUpper(w)
extern bool isHan(wchar_t w);		// �Ƿ��Ǻ��� L'һ' <= w && w <= L'��'
extern bool isChinese(wchar_t w);	// isHan(w) || isLower(w) || isUpper(w) || isDigit(w)

class CharString;
extern bool isNumber(const CharString &str);	// �Ƿ�ÿһλ isDigit
extern bool isSpaces(const CharString &str);	// �Ƿ�ÿһλ iswspace

extern bool endsWith(const char * str, const char *end);	// str�Ƿ���end��β