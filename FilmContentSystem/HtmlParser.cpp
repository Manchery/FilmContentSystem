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
	html = _html; hp = 0;
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

CharString HtmlReader::readScript()
{
	// 保证script标签一定闭合，且闭合标签为 </script>
	// TODO: maybe </ script>
	CharString res;
	while (!(html.substring(hp, hp + 9) == L"</script>"))
		res += html[hp++];
	return res;
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

// 此时 reader 指向 '<'
HtmlTag HtmlParser::readTag(TagState & closeState)
{
	closeState = OPEN;
	reader.getChar(); // read '<'

	if (reader.nextChar() == '!') {
		closeState = COMMENT;
		reader.skipBlock('>');
		reader.getChar(); // read '>'
		return HtmlTag();
	}

	if (reader.nextChar() == '/') closeState = CLOSED;
	reader.skipBlock(isAlpha);
	CharString tagName = reader.getBlock(endOfTagName);
	if (isSelfClosed(tagName)) closeState = SELFCLOSED;

	HtmlTag tag(tagName);

	while (true){ // reading attribute
		CharString key, value;

		wchar_t w;
		do w = reader.getChar(); while (w != '>' && !isAlpha(w));
		if (isAlpha(w)) 
			reader.backSpace();
		else if (w == '>') {
			if (reader.prevChar() == '/') closeState = SELFCLOSED;
			return tag;
		}

		key = reader.getBlock(notName);
		do w = reader.getChar(); while (w != '=' && w != '>' && !isAlpha(w));
		if (w == '=') {
			do w = reader.getChar(); while (w != '\'' && w != '\"');
			value = reader.getBlock(w);
			tag.addAttribute(key, value);
		}
		else if (isAlpha(w)) {
			reader.backSpace(); 
			tag.addAttribute(key);
		}
		else if (w == '>') {
			if (reader.prevChar() == '/') closeState = SELFCLOSED;
			return tag;
		}
	}
}

CharString HtmlParser::postProcessTitle(const CharString & name)
{
	int l = 0, r = name.length()-1;
	while (l <= r && iswspace(name[l])) l++;
	while (l <= r && iswspace(name[r])) r--;
	return name.substring(l, r + 1 - 5); // 5 for "_(豆瓣）"
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

void HtmlParser::postProcessInfoLine(const CharString & line, CharStringLink * item)
{
	int start = 0, len = line.length();
	while (line[start] != ':') start++; start++;
	int last = start - 1;
	for (int i = start; i <= len; i++) {
		if (i == len || (i - 1 >= 0 && i + 1 < len && line[i - 1] == ' ' && line[i] == '/' && line[i + 1] == ' ')) {
			int l = last + 1, r = i - 1;
			while (l <= r && iswspace(line[l])) l++;
			while (l <= r && iswspace(line[r])) r--;
			item->push_back(line.substring(l, r + 1));
			last = i + 1;
		}
	}
}

void HtmlParser::postProcessInfo(const CharString & content, FilmInfo & info)
{
	const int itemNum = 9;
	const CharString lineStart[] = {
		L"导演:",L"编剧:",L"主演:",L"类型:",L"制片国家/地区:",L"语言:",L"上映日期:",L"片长:",L"又名:"
	};
	CharStringLink *items[] = {
		&info._directors, &info._screenwriters, &info._stars, &info._genres, &info._regions, &info._languages, &info._dates, &info._durations, &info._alternates
	};
	for (int i = 0; i < itemNum; i++) {
		int start = content.indexOf(lineStart[i]);
		if (start == -1) continue;
		CharString tmp = content.substring(start, content.length());
		int br = tmp.indexOf(L"<br>");
		postProcessInfoLine(tmp.substring(0, br), items[i]);
	}
}

void HtmlParser::postProcessTag(const HtmlTag & tag, FilmInfo & info)
{
	if (tag.type() == L"title") {
		info.setName(postProcessTitle(tag.content()));
	}
	else if (tag.type() == L"div" && tag.hasAttribute(L"id", L"info")) {
		postProcessInfo(tag.content(), info);
	}
	else if (tag.hasAttribute(L"property", L"v:summary") || tag.hasAttribute(L"class", L"all hidden")) {
		info.setIntroduction(postProcessSummary(tag.content()));
	}
}

FilmInfo HtmlParser::parse(const CharString & html)
{
	while (!tags.empty()) tags.pop();
	reader.setHtml(html);
	FilmInfo info;

	while (!reader.eof()) {
		CharString block;
		if (!tags.empty() && tags.top().type() == L"script")
			block = reader.readScript();
		else
			block = reader.getBlock('<');
		if (!isSpaces(block)) {
			tags.top().pushContent(block);
		}

		if (reader.eof()) break;

		TagState state;
		HtmlTag currentTag = readTag(state), lastTop;

		switch (state)
		{
		case OPEN:
			tags.push(currentTag);
			break;
		case CLOSED:
			do {
				lastTop = tags.top(); tags.pop();
				if (!tags.empty()) tags.top().pushContent(lastTop.content());
			} while (!(lastTop.type()==currentTag.type()));
			postProcessTag(lastTop, info);
			break;
		case SELFCLOSED:
			if (currentTag.type() == L"br") {
				tags.top().pushContent(L"<br>");
			}
			break;
		case COMMENT:
			break;
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
