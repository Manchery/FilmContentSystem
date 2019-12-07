#include "CharString.h"
#include "common.h"
#include <stdexcept>
#include <cstring>
#include <string>


CharString::CharString()
{
	_len = _capacity = 0; _str = nullptr;
}

CharString::CharString(wchar_t ch)
{
	_len = _capacity = 1;
	_str = new wchar_t[1]{ ch };
}

CharString::CharString(const wchar_t * wstr)
{
	_len = _capacity = wcslen(wstr);
	if (_len == 0)
		_str = nullptr;
	else {
		_str = new wchar_t[_len];
		memcpy(_str, wstr, sizeof(wchar_t)*_len);
	}
}

CharString::CharString(int len)
{
	_len = _capacity = len; 
	_str = len == 0 ? nullptr : new wchar_t[len] {0};
}

CharString::CharString(const CharString & wstr)
{
	_len = _capacity = wstr._len;
	if (_len == 0)
		_str = nullptr;
	else {
		_str = new wchar_t[_len];
		memcpy(_str, wstr._str, sizeof(wchar_t)*_len);
	}
}

CharString::CharString(const std::wstring & wstr)
{
	_len = _capacity = wstr.length();
	if (_len == 0)
		_str = nullptr;
	else {
		_str = new wchar_t[_len];
		for (int i = 0; i < _len; i++)
			_str[i] = wstr[i];
	}
}

CharString::~CharString()
{
	delete[] _str;
}

void CharString::reserve(int cap)
{
	if (cap <= _capacity) return;
	wchar_t *newStr = new wchar_t[cap];
	memcpy(newStr, _str, sizeof(wchar_t)*_len);
	delete[] _str; _str = newStr; _capacity = cap;
}

wchar_t CharString::operator[](int x) const
{
	if (x < 0 || x >= _len) throw std::out_of_range("Index out of range!");
	return _str[x];
}

wchar_t & CharString::operator[](int x)
{
	if (x < 0 || x >= _len) throw std::out_of_range("Index out of range!");
	return _str[x];
}

CharString CharString::substring(int l, int r) const {
	if (l < 0 || r > _len)
		throw std::out_of_range("Index out of range!");
	if (l >= r)
		return CharString(L"");
	CharString result(r - l);
	memcpy(result._str, _str + l, sizeof(wchar_t)*(r - l));
	return result;
}

int CharString::indexOf(const CharString & b) const
{
	int blen = b.length();
	int *next = new int[blen];		// border 数组
	next[0] = -1;
	int k = -1;
	for (int i = 1; i < blen; i++) {
		while (~k && b[k + 1] != b[i]) k = next[k];
		if (b[k + 1] == b[i]) k++;
		next[i] = k;
	}

	k = -1; int ans = -1;
	for (int i = 0; i < _len; i++) {
		while (~k && b[k + 1] != _str[i]) k = next[k];
		if (b[k + 1] == _str[i]) k++;
		if (k == blen - 1) {
			ans = i - k; break;
		}
	}
	delete[] next;
	return ans;
}

double CharString::toDouble() const
{
	double result = 0; int i;
	for (i = 0; i < _len && _str[i] != '.'; i++)
		result = result * 10 + _str[i] - '0';
	double base = 1;
	for (i++; i < _len; i++)
		base *= 0.1, result += base * (_str[i] - '0');
	return result;
}

std::wstring CharString::toWString() const
{
	std::wstring res;
	for (int i = 0; i < _len; i++) res += _str[i];
	return res;
}

CharString& CharString::assign(const CharString & b)
{
	if (b._len > _capacity) {
		_len = _capacity = b.length();
		delete[] _str; 
		_str = new wchar_t[_len];
		memcpy(_str, b._str, sizeof(wchar_t)*_len);
	} else {
		_len = b.length();
		memcpy(_str, b._str, sizeof(wchar_t)*_len);
	}		
	return *this;
}

CharString & CharString::operator=(const CharString & b)
{
	return assign(b);
}

CharString & CharString::operator+=(const CharString & b)
{
	if (_len + b._len > _capacity) {
		// 容量不足时，容量翻倍重新分配，该操作均摊复杂度 O(len(b))
		int newCap = _capacity == 0 ? 1 : _capacity;
		while (newCap < _len + b._len) newCap <<= 1;
		reserve(newCap);
	}
	memcpy(_str + _len, b._str, sizeof(wchar_t)*b._len);
	_len += b._len;
	return *this;
}

CharString & CharString::operator+=(wchar_t b)
{
	if (_len + 1 > _capacity) {
		// 容量不足时，容量翻倍重新分配，该操作均摊复杂度 O(len(b))
		int newCap = _capacity == 0 ? 1 : _capacity;
		while (newCap < _len + 1) newCap <<= 1;
		reserve(newCap);
	}
	_str[_len] = b;
	_len++;
	return *this;
}

std::wistream & operator>>(std::wistream & is, CharString & str)
{
	std::wstring wstr;
	is >> wstr;
	if (str._capacity < wstr.length()) {
		str._len = str._capacity = wstr.length();
		delete[] str._str; str._str = new wchar_t[str._len];
		for (int i = 0; i < str._len; i++) str._str[i] = wstr[i];
	}
	else {
		str._len = wstr.length();
		for (int i = 0; i < str._len; i++) str._str[i] = wstr[i];
	}
	return is;
}

std::wostream & operator<<(std::wostream & os, const CharString & str)
{
	for (int i = 0; i < str._len; i++) os << str._str[i];
	return os;
}

CharString concat(const CharString & a, const CharString & b)
{
	CharString result(a.length()+b.length());
	if (a.length())
		memcpy(result._str, a._str, sizeof(wchar_t)*a.length());
	if (b.length())
		memcpy(result._str + a.length(), b._str, sizeof(wchar_t)*b.length());
	return result;
}

CharString operator+(const CharString & a, const CharString & b)
{
	return concat(a, b);
}

bool operator==(const CharString & a, const CharString & b)
{
	if (a.length() != b.length()) return false;
	for (int i = 0; i < a.length(); i++)
		if (a[i] != b[i]) 
			return false;
	return true;
}

bool operator<(const CharString & a, const CharString & b)
{
	for (int i = 0; i < Max(a.length(), b.length()); i++)
		if (i >= b.length())
			return false;
		else if (i >= a.length())
			return true;
		else if (a[i] < b[i])
			return true;
		else if (a[i] > b[i])
			return false;
	return false;
}

hash_t charStringHash(const CharString & str) {
	static hash_t seed = 50021;
	hash_t h = 0;
	for (int i = 0; i < str.length(); i++) {
		h = h * seed + (hash_t)str[i];
	}
	return h;
}
