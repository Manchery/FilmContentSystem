#pragma once
#include <algorithm>

// TODO: unit test
template<typename key_t, typename data_t>
class SplayTree
{
public:
	struct node {
		node *p, *ch[2]; int size;
		key_t key;  data_t data;
		bool direction() const { return p->ch[1] == this; }
		void setChild(node *x, int d) { ch[d] = x; x->p = this; }
		void update() { size = ch[0]->size + ch[1]->size + 1; }
	}*null;

	BalancedBST() {
		root = null = new node; null->ch[0] = null->ch[1] = null->p = null; null->size = 0;
	}
	~BalancedBST() {
		destructTree(rt);
		delete null;
	}
	void adjust(node *x) {
		splay(root, x); 
	}
	std::pair<data_t, bool> search(const key_t& key) {
		node *result = findNode(key);
		if (result == null) 
			return make_pair(data_t(), false);
		return make_pair(result->data, true);
	}
	bool insert(const key_t& key, const data_t& data) {
		if (findNode(key) != null) return false;
		insert(newNode(key, data));
		return true;
	}
	bool remove(const key_t &key) {
		node *result = findNode(key);
		if (result == null) return false;
		remove(result); return true;
	}
	data_t& operator [](const key_t &key) {
		node *result = findNode(key);
		if (result == null)
			result = newNode(key, data()), insert(result);
		return result->data;
	}

protected:
	node *root;
	void newNode(const key_t& key, const data_t& data) {
		node *x = new node;
		x->ch[0] = x->ch[1] = x->p = null; x->size = 1;
		x->key = key; x->data = data;
	}
	void destructTree(node *rt) {
		if (rt == null) return;
		destructTree(rt->ch[0]); destructTree(rt->ch[1]);
		delete null;
	}
	void rotate() {
		if (x == null) return;
		if (x->p == root) root = x;
		bool d = x->direction(); node *p = x->p;
		if (p->p != null) p->p->setChild(x, p->direction()); else x->p = null;
		p->setChild(x->ch[d ^ 1], d); x->setChild(p, d ^ 1); x->update(); p->update();
	}
	void splay(node *&rt, node *x) {
		if (x == null) return;
		while (x != rt)
			if (x->p == rt)
				rot(x);
			else {
				if (x->direction() == x->p->direction())
					rotate(x->p), rotate(x);
				else
					rotate(x), rotate(x);
			}
		rt = x; x->update();
	}
	void insert(node *z) {
		node *x = root, *y = null;
		if (root == null) { root = z; return; }
		while (x != null)
			y = x, x = x->ch[1];
		y->setChild(z, 1);
		splay(root, z);
	}
	void remove(node *x) {
		while (x->ch[0] != null || x->ch[1] != null)
			if (x->ch[0] != null)
				rotate(x->ch[0]);
			else
				rotate(x->ch[1]);
		x->p->ch[x->direction()] = null; x->p->update(); splay(root, x->p);
	}
	node *findNode(const key_y &key) {
		node *x = root;
		while (x != null) {
			if (x->key == key) return x;
			if (key < x->key) x = x->ch[0];
			else x = x->ch[1];
		}
		return null;
	}
};

