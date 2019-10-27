#pragma once

#include "HtmlTag.h"
#include "Stack.hpp"
#include "CharString.h"
#include "FilmInfo.h"

class HtmlReader {
private:
	CharString html;
	int hp; // html pointer
public:
	HtmlReader();
	HtmlReader(const CharString &html);
	void setHtml(const CharString &_html);

	bool eof() const;
	wchar_t nextChar() const;
	wchar_t prevChar() const;
	wchar_t curChar() const;
	wchar_t getChar();
	void backSpace();

	CharString getBlock(bool stopChar(wchar_t));
	CharString getBlock(wchar_t stopChar);
	void skipBlock(bool stopChar(wchar_t));
	void skipBlock(wchar_t stopChar);
};

enum TagState {
	OPEN, CLOSED,SELFCLOSED 
};

class HtmlParser
{
private:
	HtmlReader reader;
	Stack<CharString> tags;
	int important; // TODO: rename

	void readTag(CharString &tagName, TagState &closeState, bool & isName, bool &isInfo, bool &isSummary);

	CharString postProcessName(const CharString &name);
	CharString postProcessSummary(const CharString &summary);
	void postProcessInfo(const CharString &info, CharStringLink *item);
public:
	HtmlParser();
	~HtmlParser();
	
	FilmInfo parse(const CharString &html);
};

const int NUM_SELF_CLOSED_TAG = 16;
const wchar_t SELF_CLOSED_TAG[][10] = { L"area", L"base", L"br", L"col", L"command", L"embed", L"hr", L"img", L"input", L"keygen", L"link", L"meta", L"param", L"source", L"track", L"wbr" };
extern bool isSelfClosed(const CharString &tag);

