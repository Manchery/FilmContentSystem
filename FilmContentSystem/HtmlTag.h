#pragma once

#include "Vector.hpp"
#include "CharString.h"
#include <cstdio>
#include <cstdlib>

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

class HtmlTag
{
	CharString _type;
	Vector<TagAttribute> _attr;
	CharString _content;
public:
	HtmlTag();
	HtmlTag(const CharString &type);
	~HtmlTag();
	const CharString &type() const { return _type; }
	const Vector<TagAttribute>& attributes() const { return _attr; }
	const CharString &content() const { return _content; }
	void addAttribute(const CharString &key);
	void addAttribute(const CharString &key, const CharString &value);
	void pushContent(const CharString &content);

	bool hasAttribute(const CharString &key) const;
	bool hasAttribute(const CharString &key, const CharString &value) const;
};

