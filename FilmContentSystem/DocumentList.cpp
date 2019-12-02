#include "DocumentList.h"

DocumentList::DocumentList() {
	head = nullptr;
}

DocumentList::~DocumentList() {
	destructList(head);
}

bool DocumentList::add(int id, double rating) {
	if (findNode(id) != nullptr) return false;
	node *x = new node(id, rating);
	x->next = head; head = x;
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
	return findNode(id)->rating;
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
			std::swap(x->id, x->next->id); x = x->next;
		}
		else if (x->prev != nullptr && x->rating > x->prev->rating) {
			std::swap(x->id, x->prev->id); x = x->prev;
		}
		else
			break;
	}
}
