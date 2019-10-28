#pragma once

#include "HtmlTag.h"
#include "Stack.hpp"
#include "CharString.h"
#include "FilmInfo.h"

/*
HtmlReader
说明：	为 HtmlParser 封装了一些读取html文本的操作，使得 HtmlParser 不直接操作、存储html的读取状态
接口：	各种读操作详见函数声明注释
*/

class HtmlReader {
private:
	CharString html;
	int hp;		// html pointer，指向下一个应该读取的字符
public:
	HtmlReader();
	HtmlReader(const CharString &html);
	void setHtml(const CharString &_html);

	// 是否读完，判断条件为 hp >= html.length()
	bool eof() const;
	// 读取一个字符，hp指针后移一位
	wchar_t getChar();
	// 返回hp指针指向的字符，hp指针不移动
	wchar_t curChar() const;
	// 返回hp指向的位置的后一个字符
	wchar_t nextChar() const;
	// 返回hp指向的位置的前一个字符
	wchar_t prevChar() const;
	// 退格：hp指针前移一位
	void backSpace();

	// 读取一个字符串，直到遇到stopChar，结束时hp指向stopChar的位置
	CharString getBlock(bool stopChar(wchar_t));
	CharString getBlock(wchar_t stopChar);
	// 跳过一个字符串，直到遇到stopChar，结束时hp指向stopChar的位置
	void skipBlock(bool stopChar(wchar_t));
	void skipBlock(wchar_t stopChar);

	// 读取JavaScript代码，结束时hp指向 </script> 的第一个位置 '<'
	// 假设script标签一定闭合，且闭合标签精确匹配 "</script>"
	// TODO: maybe </ script>, etc
	CharString readScript();
};

// 标签 ('<'和'>'包围的一段字符) 的状态：分别为开启、关闭、自关闭、注释
// 如：<p>, </p>, <br/>, <!--->
enum TagState {
	OPEN, CLOSED,SELFCLOSED, COMMENT
};

/*
HtmlParser
说明：	从html文本中解析所需的电影的信息
		栈标签全遍历：维护一个栈（HtmlTag类型，包含标签类型、标签属性、标签内容），
			该栈每时每刻维护为当前标签的及其外层标签的层次关系
		具有良好的可扩展性
接口：	解析：parse
*/

class HtmlParser
{
private:
	HtmlReader reader;
	// 该栈每时每刻维护为当前标签的及其外层标签的层次关系
	Stack<HtmlTag> tags;
	
	// 读取一段由 '<' 和 '>' 包围的标签，调用时 reader 的hp应指向 '<'
	HtmlTag readTag(TagState & closeState);

	// 处理title标签中的文本，具体地，去除首尾空白字符以及“ （豆瓣）”
	CharString postProcessTitle(const CharString &name);
	// 处理剧情简介，具体地，去除首尾空白字符，并将<br>转化为 '\n'
	CharString postProcessSummary(const CharString &summary);
	// 处理info中的一行，具体地，去除开头至冒号，并将后面的内容根据 " / " 断开
	void postProcessInfoLine(const CharString &line, CharStringLink *item);
	// 处理包含info的div的文本内容，具体地，分行调用postProcessInfoLine
	void postProcessInfo(const CharString &content, FilmInfo &info);
	// 处理从栈中弹出的标签，具体地，判断是否为所需电影信息并调用相应函数
	void postProcessTag(const HtmlTag& tag, FilmInfo &info);

public:
	HtmlParser();
	~HtmlParser();
	
	// 解析html，提取电影信息并同时维护标签栈
	FilmInfo parse(const CharString &html);
};

// SELF_CLOSED_TAG 数组的大小
const int NUM_SELF_CLOSED_TAG = 16;
// html规定的自关闭的标签
const wchar_t SELF_CLOSED_TAG[][10] = { L"area", L"base", L"br", L"col", L"command", L"embed", L"hr", L"img", L"input", L"keygen", L"link", L"meta", L"param", L"source", L"track", L"wbr" };
// 判断是否为自关闭的标签 如 isSelfClosed(L"br")==true
extern bool isSelfClosed(const CharString &tag);

