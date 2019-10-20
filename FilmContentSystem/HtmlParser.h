#pragma once

#include "Stack.hpp"
#include "CharString.h"
#include "FilmInfo.h"

enum TagState {
	OPEN, CLOSED,SELFCLOSED 
};

class HtmlParser
{
private:
	Stack<CharString> tags;
	int important; // TODO: rename
	
	CharString _html;
	int hp; // html pointer
	
	wchar_t nextChar();
	wchar_t prevChar();
	wchar_t getChar();
	CharString getBlock(bool stopChar(wchar_t));
	CharString getBlock(wchar_t stopWord);
	void skipBlock(bool stopChar(wchar_t));
	void skipBlock(wchar_t stopWord);
	void backSpace();

	void readTag(CharString &tagName, TagState &closeState, bool & isName, bool &isInfo, bool &isSummary);

	CharString postProcessName(const CharString &name);
	CharString postProcessSummary(const CharString &summary);
	void postProcessInfo(const CharString &info, CharStringLink *item);
public:
	HtmlParser();
	~HtmlParser();
	
	FilmInfo parse(const CharString &__html);
};

const int NUM_SELF_CLOSED_TAG = 16;
const wchar_t SELF_CLOSED_TAG[][10] = { L"area", L"base", L"br", L"col", L"command", L"embed", L"hr", L"img", L"input", L"keygen", L"link", L"meta", L"param", L"source", L"track", L"wbr" };

extern bool isSelfClosed(const CharString &tag);

