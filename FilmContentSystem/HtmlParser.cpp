#include "HtmlParser.h"
#include "WordSegmentor.h"
#include <cctype>


wchar_t HtmlParser::prevChar()
{
	if (hp >= 2) return _html[hp - 2];
	throw std::out_of_range("The previous out of range!");
}

wchar_t HtmlParser::nextChar()
{
	if (hp < _html.length()) return _html[hp];
	throw std::out_of_range("The next out of range");
}

wchar_t HtmlParser::getChar() {
	if (hp >= _html.length()) return EOF;
	return _html[hp++];
}

CharString HtmlParser::getBlock(bool stopChar(wchar_t)) {
	if (hp >= _html.length()) return CharString();
	CharString res;
	while (hp < _html.length() && !stopChar(_html[hp]))
		res += _html[hp++];
	return res;
}

CharString HtmlParser::getBlock(wchar_t stopWord)
{
	if (hp >= _html.length()) return CharString();
	CharString res;
	while (hp < _html.length() && _html[hp]!=stopWord)
		res += _html[hp++];
	return res;
}

void HtmlParser::skipBlock(bool stopChar(wchar_t))
{
	if (hp >= _html.length()) return;
	while (hp < _html.length() && !stopChar(_html[hp]))
		hp++;
}

void HtmlParser::skipBlock(wchar_t stopWord)
{
	if (hp >= _html.length()) return;
	while (hp < _html.length() && _html[hp]!=stopWord)
		hp++;
}

void HtmlParser::backSpace()
{
	if (hp > 0) hp--;
}

static inline bool notName(wchar_t w) {
	return !isDigit(w) && !isAlpha(w) && w != '-';
}
static inline bool endOfTagName(wchar_t w) {
	return iswblank(w) || w == '>'  || w == '/';
}

HtmlParser::HtmlParser()
{
}

HtmlParser::~HtmlParser()
{
}

void HtmlParser::readTag(CharString & tagName, TagState & closeState, bool & isName, bool & isInfo, bool & isSummary)
{
	closeState = OPEN;
	if (nextChar() == '/') closeState = CLOSED;
	skipBlock(isAlpha);
	tagName = getBlock(endOfTagName); isInfo = false; isSummary = false; isName = false;
	if (tagName == L"title") isName = true;
	if (isSelfClosed(tagName)) closeState = SELFCLOSED;

	while (true){ // reading attribute
		CharString key, value;
		wchar_t w;
		do w = getChar(); while (w != '>' && !isAlpha(w));
		if (isAlpha(w)) backSpace();
		else if (w == '>') {
			if (prevChar() == '/') closeState = SELFCLOSED;
			return;
		}

		key = getBlock(notName);
		do w = getChar(); while (w != '=' && w != '>' && !isAlpha(w));
		if (w == '=') {
			do w = getChar(); while (w != '\'' && w != '\"');
			value = getBlock(w);
			if (key == L"id" && value == L"info")
				isInfo = true;
			if (key == L"property" && value == L"v:summary")
				isSummary = true;
			/*if (key == L"property" &&value == L"v:itemreviewed")
				isName = true;*/
		}
		else if (isAlpha(w)) {
			backSpace(); continue;
		}
		else if (w == '>') {
			if (prevChar() == '/') closeState = SELFCLOSED;
			return;
		}
	}
}

CharString HtmlParser::postProcessName(const CharString & name)
{
	int l = 0, r = name.length()-1;
	while (l <= r && iswspace(name[l])) l++;
	while (l <= r && iswspace(name[r])) r--;
	return name.substring(l, r + 1 - 4); // 4 for （豆瓣）
}

CharString HtmlParser::postProcessSummary(const CharString & summary)
{
	CharString res; int len = summary.length();
	for (int i = 0; i < len; i++) {
		if (iswspace(summary[i])) continue;
		if (i + 3 < len && summary.substring(i, i + 4) == L"<br>")
			res += '\n', i += 4;
		else 
			res += summary[i];
	}
	return res;
}

void HtmlParser::postProcessInfo(const CharString & info, CharStringLink * item)
{
	int start = 0, len = info.length();
	if (info[0] == ':') start++;
	int last = start - 1;
	for (int i = start; i <= len; i++) {
		// if (i == len || (i - 1 >= 0 && i + 1 < len && info[i - 1] == ' ' && info[i] == '/' && info[i + 1] == ' ')) {
		if (i == len || info[i] == '/' ) {
			int l = last + 1, r = i - 1;
			while (l <= r && iswspace(info[l])) l++;
			while (l <= r && iswspace(info[r])) r--;
			item->push_back(info.substring(l, r + 1));
			last = i + 1;
		}
	}
}

FilmInfo HtmlParser::parse(const CharString & __html)
{
	FilmInfo info;
	_html = __html; hp = 0; 
	bool readingInfo = false, readingSummary = false, readingName = false;
	
	CharString readed; bool readingInfoItem=false;
	CharStringLink *item=nullptr;

	while (hp < _html.length()) {
		if (!readingInfo && !readingSummary && !readingName)
			skipBlock('<');
		else {
			CharString tmp;
			tmp = getBlock('<');
			if (readingInfoItem || readingName || readingSummary)
				readed += tmp;
			else {
				if (tmp == L"导演") item = &info._directors, readingInfoItem = true;
				else if (tmp == L"编剧") item = &info._screenwriters, readingInfoItem = true;
				else if (tmp == L"主演") item = &info._stars, readingInfoItem = true;
				else if (tmp == L"类型:") item = &info._genres, readingInfoItem = true;
				else if (tmp == L"制片国家/地区:") item = &info._regions, readingInfoItem = true;
				else if (tmp == L"语言:") item = &info._languages, readingInfoItem = true;
				else if (tmp == L"上映日期:") item = &info._dates, readingInfoItem = true;
				else if (tmp == L"片长:") item = &info._durations, readingInfoItem = true;
				else if (tmp == L"又名:") item = &info._alternates, readingInfoItem = true;
			}
		}

		getChar(); // pass '<'
		if (hp == _html.length()) break;

		if (nextChar() == '!') // 注释
			skipBlock('>'), getChar();
		else {
			CharString tagName;
			bool isInfo, isSummary, isName;
			TagState closeState;
			
			readTag(tagName, closeState, isName, isInfo, isSummary);

			if (closeState == OPEN) {
				tags.push(tagName);
				if (isInfo) 
					readingInfo = true, important = tags.size();
				if (isSummary) 
					readingSummary = true, important = tags.size();
				if (isName)
					readingName = true, important = tags.size();
			}
			else if (closeState == CLOSED) {
				while (!(tags.top() == tagName)) tags.pop();  // 未正确关闭的标签
				tags.pop();
				if (tags.size() < important) {
					if (readingName) {
						info.setName(postProcessName(readed));
						readed.clear();
					}
					if (readingSummary) {
						info.setIntroduction(postProcessSummary(readed));
						readed.clear();
					}
					important = -1, readingInfo = readingSummary = readingName = false;
				}
			}
			else if (closeState == SELFCLOSED) {
				if (readingInfoItem && tagName == L"br") {
					postProcessInfo(readed, item);
					readingInfoItem = false;
					readed.clear();
				}
				if (readingSummary && tagName == L"br") {
					readed += L"<br>";
				}
			}
		}
	}

	return info;
}

bool isSelfClosed(const CharString & tag) {
	for (int i = 0; i < NUM_SELF_CLOSED_TAG; i++)
		if (tag == SELF_CLOSED_TAG[i])
			return true;
	return false;
}
