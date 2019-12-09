#include "DocumentList.h"

DocumentList::DocumentList(int termId) : termId(termId), head(nullptr) {

}

DocumentList::DocumentList(const DocumentList & b)
{
	head = nullptr; termId = b.termId;
	node *tail = nullptr, *p = b.head;
	while (p) tail = p, p = p->next;
	p = tail; 
	while (p) 
		add(p->id, p->rating), p = p->prev;
}

DocumentList::~DocumentList() {
	destructList(head);
}

DocumentList & DocumentList::operator=(const DocumentList & b)
{
	destructList(head);

	head = nullptr; termId = b.termId;
	node *tail = nullptr, *p = b.head;
	while (p) tail = p, p = p->next;
	p = tail;
	while (p) add(p->id, p->rating), p = p->prev;
	return *this;
}

bool DocumentList::add(int id, double rating) {
	if (findNode(id) != nullptr) return false;
	node *x = new node(id, rating);
	x->next = head; if (head) head->prev = x;
	head = x;
	adjust(head); return true;
}

bool DocumentList::remove(int id) {
	node *x = findNode(id);
	if (x == nullptr) return false;
	if (x->next) x->next->prev = x->prev;
	if (x->prev) x->prev->next = x->next;
	if (x == head) head = head->next;
	delete x; return true;
}

double DocumentList::search(int id) const {
	node *x = findNode(id);
	return x ? x->rating : 0;
}

bool DocumentList::edit(int id, double rating) {
	node *x = findNode(id);
	if (x == nullptr) return false;
	x->rating = rating; adjust(x);
	return true;
}

void DocumentList::destructList(node * x) {
	if (x == nullptr) return;
	destructList(x->next); delete x;
}

DocumentList::node * DocumentList::findNode(int id) const {
	node *x = head;
	while (x != nullptr)
		if (x->id == id)
			return x;
		else
			x = x->next;
	return nullptr;
}

void DocumentList::adjust(node * x) {
	while (true) {
		if (x->next != nullptr && x->rating < x->next->rating) {
			std::swap(x->id, x->next->id); std::swap(x->rating, x->next->rating);
			x = x->next;
		}
		else if (x->prev != nullptr && x->rating > x->prev->rating) {
			std::swap(x->id, x->prev->id); std::swap(x->rating, x->prev->rating);
			x = x->prev;
		}
		else
			break;
	}
}
