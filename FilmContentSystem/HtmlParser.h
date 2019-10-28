#pragma once

#include "HtmlTag.h"
#include "Stack.hpp"
#include "CharString.h"
#include "FilmInfo.h"

/*
HtmlReader
˵����	Ϊ HtmlParser ��װ��һЩ��ȡhtml�ı��Ĳ�����ʹ�� HtmlParser ��ֱ�Ӳ������洢html�Ķ�ȡ״̬
�ӿڣ�	���ֶ����������������ע��
*/

class HtmlReader {
private:
	CharString html;
	int hp;		// html pointer��ָ����һ��Ӧ�ö�ȡ���ַ�
public:
	HtmlReader();
	HtmlReader(const CharString &html);
	void setHtml(const CharString &_html);

	// �Ƿ���꣬�ж�����Ϊ hp >= html.length()
	bool eof() const;
	// ��ȡһ���ַ���hpָ�����һλ
	wchar_t getChar();
	// ����hpָ��ָ����ַ���hpָ�벻�ƶ�
	wchar_t curChar() const;
	// ����hpָ���λ�õĺ�һ���ַ�
	wchar_t nextChar() const;
	// ����hpָ���λ�õ�ǰһ���ַ�
	wchar_t prevChar() const;
	// �˸�hpָ��ǰ��һλ
	void backSpace();

	// ��ȡһ���ַ�����ֱ������stopChar������ʱhpָ��stopChar��λ��
	CharString getBlock(bool stopChar(wchar_t));
	CharString getBlock(wchar_t stopChar);
	// ����һ���ַ�����ֱ������stopChar������ʱhpָ��stopChar��λ��
	void skipBlock(bool stopChar(wchar_t));
	void skipBlock(wchar_t stopChar);

	// ��ȡJavaScript���룬����ʱhpָ�� </script> �ĵ�һ��λ�� '<'
	// ����script��ǩһ���պϣ��ұպϱ�ǩ��ȷƥ�� "</script>"
	// TODO: maybe </ script>, etc
	CharString readScript();
};

// ��ǩ ('<'��'>'��Χ��һ���ַ�) ��״̬���ֱ�Ϊ�������رա��Թرա�ע��
// �磺<p>, </p>, <br/>, <!--->
enum TagState {
	OPEN, CLOSED,SELFCLOSED, COMMENT
};

/*
HtmlParser
˵����	��html�ı��н�������ĵ�Ӱ����Ϣ
		ջ��ǩȫ������ά��һ��ջ��HtmlTag���ͣ�������ǩ���͡���ǩ���ԡ���ǩ���ݣ���
			��ջÿʱÿ��ά��Ϊ��ǰ��ǩ�ļ�������ǩ�Ĳ�ι�ϵ
		�������õĿ���չ��
�ӿڣ�	������parse
*/

class HtmlParser
{
private:
	HtmlReader reader;
	// ��ջÿʱÿ��ά��Ϊ��ǰ��ǩ�ļ�������ǩ�Ĳ�ι�ϵ
	Stack<HtmlTag> tags;
	
	// ��ȡһ���� '<' �� '>' ��Χ�ı�ǩ������ʱ reader ��hpӦָ�� '<'
	HtmlTag readTag(TagState & closeState);

	// ����title��ǩ�е��ı�������أ�ȥ����β�հ��ַ��Լ��� �����꣩��
	CharString postProcessTitle(const CharString &name);
	// ��������飬����أ�ȥ����β�հ��ַ�������<br>ת��Ϊ '\n'
	CharString postProcessSummary(const CharString &summary);
	// ����info�е�һ�У�����أ�ȥ����ͷ��ð�ţ�������������ݸ��� " / " �Ͽ�
	void postProcessInfoLine(const CharString &line, CharStringLink *item);
	// �������info��div���ı����ݣ�����أ����е���postProcessInfoLine
	void postProcessInfo(const CharString &content, FilmInfo &info);
	// �����ջ�е����ı�ǩ������أ��ж��Ƿ�Ϊ�����Ӱ��Ϣ��������Ӧ����
	void postProcessTag(const HtmlTag& tag, FilmInfo &info);

public:
	HtmlParser();
	~HtmlParser();
	
	// ����html����ȡ��Ӱ��Ϣ��ͬʱά����ǩջ
	FilmInfo parse(const CharString &html);
};

// SELF_CLOSED_TAG ����Ĵ�С
const int NUM_SELF_CLOSED_TAG = 16;
// html�涨���Թرյı�ǩ
const wchar_t SELF_CLOSED_TAG[][10] = { L"area", L"base", L"br", L"col", L"command", L"embed", L"hr", L"img", L"input", L"keygen", L"link", L"meta", L"param", L"source", L"track", L"wbr" };
// �ж��Ƿ�Ϊ�Թرյı�ǩ �� isSelfClosed(L"br")==true
extern bool isSelfClosed(const CharString &tag);

