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

	void push_back(node *newNode);

public:
	class iterator {
		node *cur;
	public:
		iterator(node * n):cur(n) {}
		CharString &operator *() { return cur->str; }
		iterator& operator ++ () { cur = cur->next; return *this; }
		iterator next() const { return iterator(cur->next); }
		bool operator == (const iterator & b) { return cur == b.cur; }
		bool operator != (const iterator & b) { return cur != b.cur; }
	};
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(nullptr); }

public:
	CharStringLink();
	CharStringLink(const CharStringLink &b);
	~CharStringLink();

	CharStringLink& operator = (const CharStringLink &b);
	
	void push_back(const CharString &s);
	void push_front(const CharString &s);
	void add(const CharString &s);
	int search(const CharString &s);
	void remove(node *x);
	void remove(const CharString &s);
	void concat(const CharStringLink &b);
	bool empty() const { return head == nullptr; }

	friend std::wostream & operator<<(std::wostream & os, const CharStringLink & str);
};

std::wostream & operator<<(std::wostream & os, const CharStringLink & str);

