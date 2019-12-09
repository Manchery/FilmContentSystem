#pragma once

#include "CharString.h"
#include <cstdio>
#include <cstdlib>

/*
CharStringLink
˵����	ʵ�����ַ�������Ļ�������
		����˫������ʵ�֣�ÿ���ڵ�洢��CharString���͵�����
		����ڵ㶯̬�����ڴ档
�ӿڣ�	�޸ģ���ֵ����� , push_back, push_front, add���൱��push_back��, remove, concat
		��ѯ��empty, search
		���������������͡�begin��end
*/

class CharStringLink
{
private:
	struct node {		// ˫������ڵ�����
		CharString str;
		node *prev, *next;
	};
	node *head, *tail;
	void destructList(node *cur);	// ��������

	void push_back(node *newNode);

public:
	// ����������
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
	// �ֱ𷵻ؿ�ʼ�������ͽ���������
	// begin��end��operator++ ���ʹ�ÿ�ʵ�ֶ�����ı�������
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(nullptr); }

public:
	CharStringLink();
	CharStringLink(const CharStringLink &b);
	~CharStringLink();

	CharStringLink& operator = (const CharStringLink &b);
	
	// ������ĩβ����
	void push_back(const CharString &s);
	// �������ͷ����
	void push_front(const CharString &s);
	// �൱�� push_back
	void add(const CharString &s);
	// ����ĳԪ���������г��ֵ�*��һ��λ��*���±���0��ʼ�������ڷ��� -1
	int search(const CharString &s);
	// ɾ��ĳ���ڵ�
	void remove(node *x);
	// ɾ��ĳԪ���������г��ֵĵ�һ��λ�ã�ע�� *ֻɾ����һ���ڵ�*
	void remove(const CharString &s);
	// ���������������ÿ�������ʽʵ�֣�b�����Կ���
	void concat(const CharStringLink &b);
	bool empty() const { return head == nullptr; }

	CharString& front() { return head->str; }
	CharString& back() { return tail->str; }

	// ת��Ϊ CharString��Ԫ�ؼ��� " / " �ָ�
	CharString toCharString() const;

	friend std::wostream & operator<<(std::wostream & os, const CharStringLink & str);
};

std::wostream & operator<<(std::wostream & os, const CharStringLink & str);

int intersectionSize(const CharStringLink &a, const CharStringLink &b, int maxSize = -1);
double IoU(const CharStringLink &a, const CharStringLink &b);