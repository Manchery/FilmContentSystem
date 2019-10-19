#pragma once

#include "Stack.hpp"
#include "CharString.h"
#include "FilmInfo.h"

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

	void readTag(CharString &tagName, int &closeState, bool & isName, bool &isInfo, bool &isSummary);
 
	CharString postProcessSummary(const CharString &summary);
	void postProcessInfo(const CharString &info, CharStringLink *item);
public:
	HtmlParser();
	~HtmlParser();
	
	FilmInfo parse(const CharString &__html);
};

