#pragma once

#include "Vector.hpp"
#include "CharString.h"
#include <cstdio>
#include <cstdlib>

/*
TagAttribute
说明：	用于表示html标签的属性的数据结构，存储了一对键值
接口：	读取：key/value
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
说明：	用于表示html标签的数据结构，存储了标签类型、标签属性、标签中的文本内容
接口：	读取操作：type/attributes/content
		修改操作：addAttribute/pushContent
		查询操作：hasAttribute
*/

class HtmlTag
{
	CharString _type;				// 标签类型
	Vector<TagAttribute> _attr;		// 标签属性
	CharString _content;			// 文本内容
public:
	HtmlTag();
	HtmlTag(const CharString &type);
	~HtmlTag();
	const CharString &type() const { return _type; }
	const Vector<TagAttribute>& attributes() const { return _attr; }
	const CharString &content() const { return _content; }
	
	// 添加属性，只有键没有值
	void addAttribute(const CharString &key);
	// 添加属性，键值对
	void addAttribute(const CharString &key, const CharString &value);
	// concat到文本内容尾部
	void pushContent(const CharString &content);

	// 查找是否有给定属性
	bool hasAttribute(const CharString &key) const;
	bool hasAttribute(const CharString &key, const CharString &value) const;
	CharString attributeValue(const CharString &key) const;
};

