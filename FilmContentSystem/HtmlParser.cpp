#include "HtmlParser.h"
#include "common.h"
#include <cctype>


HtmlReader::HtmlReader()
{
}

HtmlReader::HtmlReader(const CharString & html) : html(html) {
	hp = 0;
}

void HtmlReader::setHtml(const CharString & _html)
{
	html = _html;
}

bool HtmlReader::eof() const
{
	return hp >= html.length();
}

wchar_t HtmlReader::prevChar() const
{
	if (hp >= 2) return html[hp - 2];
	throw std::out_of_range("The previous out of range!");
}

wchar_t HtmlReader::nextChar() const
{
	if (hp < html.length()) return html[hp];
	throw std::out_of_range("The next out of range");
}

wchar_t HtmlReader::curChar() const
{
	if (hp >= html.length()) return EOF;
	return html[hp];
}

wchar_t HtmlReader::getChar() {
	if (hp >= html.length()) return EOF;
	return html[hp++];
}

void HtmlReader::backSpace()
{
	if (hp > 0) hp--;
}

CharString HtmlReader::getBlock(bool stopChar(wchar_t)) {
	if (hp >= html.length()) return CharString();
	CharString res;
	while (hp < html.length() && !stopChar(html[hp]))
		res += html[hp++];
	return res;
}

CharString HtmlReader::getBlock(wchar_t stopChar)
{
	if (hp >= html.length()) return CharString();
	CharString res;
	while (hp < html.length() && html[hp]!= stopChar)
		res += html[hp++];
	return res;
}

void HtmlReader::skipBlock(bool stopChar(wchar_t))
{
	if (hp >= html.length()) return;
	while (hp < html.length() && !stopChar(html[hp]))
		hp++;
}

void HtmlReader::skipBlock(wchar_t stopChar)
{
	if (hp >= html.length()) return;
	while (hp < html.length() && html[hp]!= stopChar)
		hp++;
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
	if (reader.nextChar() == '/') closeState = CLOSED;
	reader.skipBlock(isAlpha);
	tagName = reader.getBlock(endOfTagName); isInfo = false; isSummary = false; isName = false;
	if (tagName == L"title") isName = true;
	if (isSelfClosed(tagName)) closeState = SELFCLOSED;

	while (true){ // reading attribute
		CharString key, value;
		wchar_t w;
		do w = reader.getChar(); while (w != '>' && !isAlpha(w));
		if (isAlpha(w)) reader.backSpace();
		else if (w == '>') {
			if (reader.prevChar() == '/') closeState = SELFCLOSED;
			return;
		}

		key = reader.getBlock(notName);
		do w = reader.getChar(); while (w != '=' && w != '>' && !isAlpha(w));
		if (w == '=') {
			do w = reader.getChar(); while (w != '\'' && w != '\"');
			value = reader.getBlock(w);
			if (key == L"id" && value == L"info")
				isInfo = true;
			if (key == L"property" && value == L"v:summary")
				isSummary = true;
			if (key == L"class" && value == L"all hidden")
				isSummary = true;
			/*if (key == L"property" &&value == L"v:itemreviewed")
				isName = true;*/
		}
		else if (isAlpha(w)) {
			reader.backSpace(); continue;
		}
		else if (w == '>') {
			if (reader.prevChar() == '/') closeState = SELFCLOSED;
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
		if (i == len || (i - 1 >= 0 && i + 1 < len && info[i - 1] == ' ' && info[i] == '/' && info[i + 1] == ' ')) {
			int l = last + 1, r = i - 1;
			while (l <= r && iswspace(info[l])) l++;
			while (l <= r && iswspace(info[r])) r--;
			item->push_back(info.substring(l, r + 1));
			last = i + 1;
		}
	}
}

FilmInfo HtmlParser::parse(const CharString & html)
{
	reader.setHtml(html);
	FilmInfo info;
	bool readingInfo = false, readingSummary = false, readingName = false;
	
	CharString readed; bool readingInfoItem=false;
	CharStringLink *item=nullptr;

	while (!reader.eof()) {
		if (!readingInfo && !readingSummary && !readingName)
			reader.skipBlock('<');
		else {
			CharString block;
			block = reader.getBlock('<');
			if (readingInfoItem || readingName || readingSummary)
				readed += block;
			else {
				if (block == L"导演") item = &info._directors, readingInfoItem = true;
				else if (block == L"编剧") item = &info._screenwriters, readingInfoItem = true;
				else if (block == L"主演") item = &info._stars, readingInfoItem = true;
				else if (block == L"类型:") item = &info._genres, readingInfoItem = true;
				else if (block == L"制片国家/地区:") item = &info._regions, readingInfoItem = true;
				else if (block == L"语言:") item = &info._languages, readingInfoItem = true;
				else if (block == L"上映日期:") item = &info._dates, readingInfoItem = true;
				else if (block == L"片长:") item = &info._durations, readingInfoItem = true;
				else if (block == L"又名:") item = &info._alternates, readingInfoItem = true;
			}
		}

		reader.getChar(); // pass '<'
		if (reader.eof()) break;

		if (reader.nextChar() == '!') // 注释
			reader.skipBlock('>'), reader.getChar();
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
