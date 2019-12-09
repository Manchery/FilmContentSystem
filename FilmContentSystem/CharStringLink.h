#pragma once

#include "CharString.h"
#include <cstdio>
#include <cstdlib>

/*
CharStringLink
说明：	实现了字符串链表的基本功能
		采用双向链表实现，每个节点存储有CharString类型的数据
		链表节点动态分配内存。
接口：	修改：赋值运算符 , push_back, push_front, add（相当于push_back）, remove, concat
		查询：empty, search
		遍历：迭代器类型、begin、end
*/

class CharStringLink
{
private:
	struct node {		// 双向链表节点类型
		CharString str;
		node *prev, *next;
	};
	node *head, *tail;
	void destructList(node *cur);	// 用于析构

	void push_back(node *newNode);

public:
	// 迭代器类型
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
	// 分别返回开始迭代器和结束迭代器
	// begin、end、operator++ 配合使用可实现对链表的遍历操作
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(nullptr); }

public:
	CharStringLink();
	CharStringLink(const CharStringLink &b);
	~CharStringLink();

	CharStringLink& operator = (const CharStringLink &b);
	
	// 在链表末尾插入
	void push_back(const CharString &s);
	// 在链表表头插入
	void push_front(const CharString &s);
	// 相当于 push_back
	void add(const CharString &s);
	// 查找某元素在链表中出现的*第一个位置*，下标以0开始，不存在返回 -1
	int search(const CharString &s);
	// 删除某个节点
	void remove(node *x);
	// 删除某元素在链表中出现的第一个位置，注意 *只删除了一个节点*
	void remove(const CharString &s);
	// 连接两个链表，采用拷贝的形式实现，b链表仍可用
	void concat(const CharStringLink &b);
	bool empty() const { return head == nullptr; }

	CharString& front() { return head->str; }
	CharString& back() { return tail->str; }

	// 转化为 CharString，元素间用 " / " 分隔
	CharString toCharString() const;

	friend std::wostream & operator<<(std::wostream & os, const CharStringLink & str);
};

std::wostream & operator<<(std::wostream & os, const CharStringLink & str);

int intersectionSize(const CharStringLink &a, const CharStringLink &b, int maxSize = -1);
double IoU(const CharStringLink &a, const CharStringLink &b);