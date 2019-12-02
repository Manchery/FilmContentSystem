#pragma once
#include <algorithm>

class DocumentList{
public:
	struct node {
		int id;
		double rating;
		node *prev, *next;
		node(int id = -1, double rating = -1) :id(id), rating(rating), prev(nullptr), next(nullptr) { }
		~node(){ }
	};
	int termId;
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
