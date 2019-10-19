#include "CharStringLink.h"
#include <cassert>

CharStringLink::CharStringLink() {
	head = tail = nullptr;
}

CharStringLink::CharStringLink(const CharStringLink & b)
{
	head = tail = nullptr;
	for (node *p = b.head; p; p = p->next) {
		node *newNode = new node{ p->str, nullptr, nullptr };
		if (!head) head = newNode;
		newNode->prev = tail; if (tail) tail->next = newNode; tail = newNode;
	}
}

CharStringLink & CharStringLink::operator=(const CharStringLink & b)
{
	destructList(head);
	head = tail = nullptr;
	for (node *p = b.head; p; p = p->next) {
		node *newNode = new node{ p->str, nullptr, nullptr };
		if (!head) head = newNode;
		newNode->prev = tail; if (tail) tail->next = newNode; tail = newNode;
	}
	return *this;
}

CharStringLink::~CharStringLink() {
	destructList(head);
}

void CharStringLink::destructList(node * cur) {
	if (cur == nullptr) return;
	destructList(cur->next);
	delete cur;
}

void CharStringLink::add_front(const CharString & s) {
	node *newNode = new node{ s, nullptr, nullptr };
	if (!tail) tail = newNode;
	newNode->next = head; if (head) head->prev = newNode; head = newNode;
}

void CharStringLink::add_back(const CharString & s) {
	node *newNode = new node{ s, nullptr, nullptr };
	if (!head) head = newNode;
	newNode->prev = tail; if (tail) tail->next = newNode; tail = newNode;
}

void CharStringLink::add(const CharString & s) { add_back(s); }

int CharStringLink::search(const CharString & s) {
	int idx = 0;
	for (node* p = head; p; p = p->next, idx++) {
		if (p->str == s)
			return idx;
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
			remove(p);
			return;
		}
}

void CharStringLink::concat(const CharStringLink & b)
{
	for (node *p = b.head; p; p = p->next) {
		node *newNode = new node{ p->str, nullptr, nullptr };
		if (!head) head = newNode;
		newNode->prev = tail; if (tail) tail->next = newNode; tail = newNode;
	}
}

//void CharStringLink::concat_move(const CharStringLink & b)
//{
//	if (head == nullptr) {
//		assert(tail == nullptr);
//		head = b.head; tail = b.tail;
//		return;
//	}
//	tail->next = b.head;
//	if (b.head) b.head->prev = tail;
//	if (b.tail) tail = b.tail;
//}

