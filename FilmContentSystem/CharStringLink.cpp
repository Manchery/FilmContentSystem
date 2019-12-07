#include "CharStringLink.h"
#include <cassert>

CharStringLink::CharStringLink() {
	head = tail = nullptr;
}

CharStringLink::CharStringLink(const CharStringLink & b)
{
	head = tail = nullptr;
	for (node *p = b.head; p; p = p->next) {
		push_back(new node{ p->str, nullptr, nullptr });
	}
}

CharStringLink::~CharStringLink() {
	destructList(head);
}

void CharStringLink::destructList(node * cur) {
	if (cur == nullptr) return;
	destructList(cur->next);
	delete cur;
}

void CharStringLink::push_back(node * newNode)
{
	if (!head) head = newNode;
	newNode->prev = tail; if (tail) tail->next = newNode; tail = newNode;
}

CharStringLink & CharStringLink::operator=(const CharStringLink & b)
{
	destructList(head);
	head = tail = nullptr;
	for (node *p = b.head; p; p = p->next) {
		push_back(new node{ p->str, nullptr, nullptr });
	}
	return *this;
}

void CharStringLink::push_front(const CharString & s) {
	node *newNode = new node{ s, nullptr, nullptr };
	if (!tail) tail = newNode;
	newNode->next = head; if (head) head->prev = newNode; head = newNode;
}

void CharStringLink::push_back(const CharString & s) {
	push_back(new node{ s, nullptr, nullptr });
}

void CharStringLink::add(const CharString & s) { push_back(s); }

int CharStringLink::search(const CharString & s) {
	int idx = 0;
	for (node* p = head; p; p = p->next, idx++) {
		if (p->str == s)
			return idx;		// 第一个位置
	}
	return -1;
}

void CharStringLink::remove(node * x) {
	if (x == head && x == tail) {
		head = tail = nullptr;
	}else if (x == head) {
		head = x->next; x->next->prev = nullptr;
	}
	else if (x == tail) {
		tail = x->prev; x->prev->next = nullptr;
	}
	else {
		x->prev->next = x->next; x->next->prev = x->prev;
	}
	delete x;
}

void CharStringLink::remove(const CharString & s) {
	for (node* p = head; p; p = p->next)
		if (p->str == s) {
			remove(p);		// 只删除了一个节点
			return;
		}
}

void CharStringLink::concat(const CharStringLink & b)
{
	// 采用拷贝的形式实现，b链表仍可用
	for (node *p = b.head; p; p = p->next) {
		push_back(new node{ p->str, nullptr, nullptr });
	}
}

CharString CharStringLink::toCharString() const
{
	CharString res;
	for (auto it = begin(); it != end(); ++it) {
		if (it != begin()) res += L" / ";
		res += *it;
	}
	return res;
}

std::wostream & operator<<(std::wostream & os, const CharStringLink & str)
{
	for (auto it = str.begin(); it != str.end(); ++it) {
		if (it != str.begin()) os << " / ";
		os << *it;
	}
	return os;
}

int intersectionSize(const CharStringLink & a, const CharStringLink & b, int maxSize)
{
	int res = 0; int cntA = 0;
	for (auto i = a.begin(); i != a.end() && cntA != maxSize; ++i, ++cntA) {
		int cntB = 0;
		for (auto j = b.begin(); j != b.end() && cntB != maxSize; ++j, ++cntB)
			if (*i == *j) {
				res++;
				break;
			}
	}
	return res;
}

double IoU(const CharStringLink & a, const CharStringLink & b)
{
	int sizeA = 0, sizeB = 0;
	int inter = 0;
	for (auto j = b.begin(); j != b.end(); ++j) sizeB++;
	for (auto i = a.begin(); i != a.end(); ++i) {
		sizeA++;
		for (auto j = b.begin(); j != b.end(); ++j)
			if (*i == *j) {
				inter++;
				break;
			}
	}
	int unin = sizeA + sizeB - inter;
	return (double)inter / unin;
}