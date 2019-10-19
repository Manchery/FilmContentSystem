#pragma once

#include <cstdio>
#include <cstdlib>
#include <cctype>

inline char fast_getchar() {
	static char buf[100000], *p1 = buf, *p2 = buf;
	return p1 == p2 && (p2 = (p1 = buf) + fread(buf, 1, 100000, stdin), p1 == p2) ? EOF : *p1++;
}
inline void fast_read(int &x) {
	char c = fast_getchar(); int b = 1;
	for (; !(c >= '0' && c <= '9'); c = fast_getchar()) if (c == '-') b = -1;
	for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = fast_getchar()); 
	x = x * b;
}
inline int fast_read(char *s) {
	char c = fast_getchar(); int len = -1;
	if (c == EOF) return -1;
	for (; c == ' ' || c == '\n' || c == '\r'; c = fast_getchar());
	for (; c != ' ' && c != '\n' && c != '\r'; s[++len] = c, c = fast_getchar()); s[++len] = 0; return len;
}
inline void fast_read(double &x) {
	char c = fast_getchar(); int b = 1;
	for (; !(c >= '0' && c <= '9'); c = fast_getchar()) if (c == '-') b = -1;
	for (x = 0; c >= '0' && c <= '9'; x = x * 10 + c - '0', c = fast_getchar());
	if (c == '.') {
		c = fast_getchar();
		for (double base = 0.1; c >= '0' && c <= '9'; x = x + base * (c - '0'), base /= 10, c = fast_getchar());
	}
	x = x * b;
}