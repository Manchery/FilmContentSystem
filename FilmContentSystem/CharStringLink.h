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
	class iterator {
		node *cur;
	public:
		iterator(node * n):cur(n) {}
		CharString &operator *() { return cur->str; }
		iterator& operator ++ () { cur = cur->next; return *this; }
		bool operator == (const iterator & b) { return cur == b.cur; }
		bool operator != (const iterator & b) { return cur != b.cur; }
	};
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(nullptr); }

public:
	CharStringLink();
	CharStringLink(const CharStringLink &b);
	CharStringLink& operator = (const CharStringLink &b);
	~CharStringLink();
	void add_back(const CharString &s);
	void add_front(const CharString &s);
	void add(const CharString &s);
	int search(const CharString &s);
	void remove(node *x);
	void remove(const CharString &s);
	void concat(const CharStringLink &b);
	// void concat_move(const CharStringLink &b); // TODO: moved ֮����delete
};

