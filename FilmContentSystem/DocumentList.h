#pragma once
#include <algorithm>

class DocumentList{
public:
	struct node {
		int id;
		double rating;
		node *prev, *next;
		node(int id = -1, double rating = -1) :id(id), rating(rating), prev(nullptr), next(nullptr) { }
		~node() { }
	};
	int termId;

	class iterator {
		node *cur;
	public:
		iterator(node * n) :cur(n) {}
		iterator& operator ++ () { cur = cur->next; return *this; }
		iterator next() const { return iterator(cur->next); }
		bool operator == (const iterator & b) { return cur == b.cur; }
		bool operator != (const iterator & b) { return cur != b.cur; }
		int id() const { return cur->id; }
		double rating() const { return cur->rating; }
	};
	// 分别返回开始迭代器和结束迭代器
	// begin、end、operator++ 配合使用可实现对链表的遍历操作
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(nullptr); }

	DocumentList(int termId = -1);
	~DocumentList();

	bool add(int id, double rating = 1);
	bool remove(int id);
	double search(int id) const;
	bool edit(int id, double rating);

	bool empty() const { return head == nullptr; }
protected:
	node *head;
	void destructList(node *x);
	node *findNode(int id) const;
	void adjust(node * x);
};
