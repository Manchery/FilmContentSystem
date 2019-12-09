#pragma once

#include "Vector.hpp"
#include "CharString.h"
#include <cstdio>
#include <cstdlib>

/*
TagAttribute
˵����	���ڱ�ʾhtml��ǩ�����Ե����ݽṹ���洢��һ�Լ�ֵ
�ӿڣ�	��ȡ��key/value
*/

class TagAttribute {
private:
	CharString _key, _value;
public:
	TagAttribute() { }
	TagAttribute(const CharString &key, const CharString &value) :_key(key), _value(value) { }
	TagAttribute(const CharString &key):_key(key),_value() { }
	const CharString &key() const { return _key; }
	const CharString &value() const { return _value; }
};

/*
HtmlTag
˵����	���ڱ�ʾhtml��ǩ�����ݽṹ���洢�˱�ǩ���͡���ǩ���ԡ���ǩ�е��ı�����
�ӿڣ�	��ȡ������type/attributes/content
		�޸Ĳ�����addAttribute/pushContent
		��ѯ������hasAttribute
*/

class HtmlTag
{
	CharString _type;				// ��ǩ����
	Vector<TagAttribute> _attr;		// ��ǩ����
	CharString _content;			// �ı�����
public:
	HtmlTag();
	HtmlTag(const CharString &type);
	~HtmlTag();
	const CharString &type() const { return _type; }
	const Vector<TagAttribute>& attributes() const { return _attr; }
	const CharString &content() const { return _content; }
	
	// ������ԣ�ֻ�м�û��ֵ
	void addAttribute(const CharString &key);
	// ������ԣ���ֵ��
	void addAttribute(const CharString &key, const CharString &value);
	// concat���ı�����β��
	void pushContent(const CharString &content);

	// �����Ƿ��и�������
	bool hasAttribute(const CharString &key) const;
	bool hasAttribute(const CharString &key, const CharString &value) const;
	CharString attributeValue(const CharString &key) const;
};

