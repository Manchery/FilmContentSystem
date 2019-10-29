#include "FileReader.h"


FileReader::FileReader()
{
	buf = new char[BUF_SIZE];
	p1 = p2 = buf;
	file = NULL;
}

FileReader::FileReader(const char * filename)
{
	buf = new char[BUF_SIZE];
	p1 = p2 = buf;
	err = fopen_s(&file, filename, "r");
}

FileReader::~FileReader()
{
	if (file) fclose(file);
	delete buf;
}

void FileReader::set_file(const char * filename)
{
	if (file) fclose(file); 
	err = fopen_s(&file, filename, "r");
	clear_buf();
}

char FileReader::nextchar() {
	return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 100000, file), p1 == p2) ? EOF : *p1++;
}

void FileReader::clear_buf() {
	p1 = p2 = buf;
}

void FileReader::read(int & x) {
	char c = nextchar(); int b = 1;
	for (; !(c >= '0' && c <= '9'); c = nextchar()) if (c == '-') b = -1;
	for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nextchar());
	x = x * b;
}

int FileReader::read(char * s) {
	char c = nextchar(); int len = -1;
	if (c == EOF) return -1;
	for (; c == ' ' || c == '\n' || c == '\r'; c = nextchar());
	for (; c != ' ' && c != '\n' && c != '\r' && c != EOF; s[++len] = c, c = nextchar()); s[++len] = 0; return len;
}

void FileReader::read(double & x) { // not support scientific notation
	char c = nextchar(); int b = 1;
	for (; !(c >= '0' && c <= '9'); c = nextchar()) if (c == '-') b = -1;
	for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = nextchar());
	if (c == '.') {
		c = nextchar();
		for (double base = 0.1; c >= '0' && c <= '9'; x = x + base * (c - '0'), base /= 10, c = nextchar());
	}
	x = x * b;
}

int FileReader::read_flag_name(char * s, char &nextChar)
{
	char c = nextchar(); int len = -1;
	for (; !((c >= 'A' && c <= 'Z') || c == '_' || c == EOF); c = nextchar());
	if (c == EOF) return -1;
	for (; (c >= 'A' && c <= 'Z') || c == '_'; s[++len] = c, c = nextchar()); s[++len] = 0;
	nextChar = c; return len;
}

int FileReader::read_string_flag(char * s)
{
	char c = nextchar(); int len = -1;
	for (; c != '\"' && c != '\''; c = nextchar()); c = nextchar();
	for (; c != '\"' && c != '\''; s[++len] = c, c = nextchar()); s[++len] = 0; return len;
}

bool FileReader::read_bool_flag()
{
	char value[100];
	read(value);
	_strlwr_s(value, strlen(value) + 1);
	if (strcmp(value, "true") == 0) return true;
	return false;
}

// Reference: https://stackoverflow.com/questions/4775437/read-unicode-utf-8-file-into-wstring
std::wstring FileReader::read_utf8_file(const char * filename)
{
	std::wifstream wif(filename);
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t>));
	std::wstringstream wss;
	wss << wif.rdbuf();
	return wss.str();
}
