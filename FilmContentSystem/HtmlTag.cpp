#include "HtmlTag.h"

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
