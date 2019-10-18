#pragma once

#include "CharString.h"
#include <cstdio>
#include <cstdlib>

class CharStringLink
{
private:
	struct node {
		CharString str;
		node *prev, *next;
	};
	node *head, *tail;
	void destructList(node *cur);
public:
	CharStringLink();
	~CharStringLink();
	void add_back(const CharString &s);
	void add_front(const CharString &s);
	void add(const CharString &s);
	int search(const CharString &s);
	void remove(node *x);
	void remove(const CharString &s);

	CharStringLink operator = (const CharStringLink &) = delete;
};

