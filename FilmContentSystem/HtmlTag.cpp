#include "HtmlTag.h"

HtmlTag::HtmlTag() {}

HtmlTag::HtmlTag(const CharString & type) :_type(type), _attr(), _content()
{
}

HtmlTag::~HtmlTag()
{
}

void HtmlTag::addAttribute(const CharString & key) {
	_attr.push_back(TagAttribute(key));
}

void HtmlTag::addAttribute(const CharString & key, const CharString & value) {
	_attr.push_back(TagAttribute(key, value));
}

void HtmlTag::pushContent(const CharString & content) {
	_content += content;
}

bool HtmlTag::hasAttribute(const CharString & key) const
{
	for (int i = 0; i < _attr.size(); i++)
		if (_attr[i].key() == key)
			return true;
	return false;
}

bool HtmlTag::hasAttribute(const CharString & key, const CharString & value) const
{
	for (int i = 0; i < _attr.size(); i++)
		if (_attr[i].key() == key && _attr[i].value() == value)
			return true;
	return false;
}

CharString HtmlTag::attributeValue(const CharString & key) const
{
	for (int i = 0; i < _attr.size(); i++)
		if (_attr[i].key() == key)
			return _attr[i].value();
	return CharString();
}
