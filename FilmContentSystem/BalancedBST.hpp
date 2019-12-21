#pragma once
#include "Vector.hpp"
#include <algorithm>

/*
SplayTree ƽ������� ģ����
˵����	ƽ������������� Splay tree ʵ��
		Reference: [Splay tree - Wikipedia](https://en.wikipedia.org/wiki/Splay_tree)
�ӿڣ�	�޸ģ�adjust, insert, remove
		��ѯ��search, find, at, operator [], list
*/

// key_t �����ͣ�data_t ֵ����
template<typename key_t, typename data_t>
class SplayTree
{
public:
	struct node {	// ���Ͻڵ�����
		// p ����; ch ����; size ������С
		node *p, *ch[2]; int size;
		key_t key;  data_t data;
		// ������ӷ��� 0���Ҷ��ӷ��� 1
		bool direction() const { return p->ch[1] == this; }
		void setChild(node *x, int d) { ch[d] = x; x->p = this; }
		void update() { size = ch[0]->size + ch[1]->size + 1; }
	}*null;
	data_t emptyData;

	SplayTree() {
		root = null = new node; null->ch[0] = null->ch[1] = null->p = null; null->size = 0;
		emptyData = data_t();
	}
	~SplayTree() {
		destructTree(root);
		delete null;
	}
	// ����ʹ������ƽ�⣬����ؽ� x ��ת����
	void adjust(node *x) {
		splay(root, x); 
	}
	// ���ң����ض�Ӧ��ֵ�Ͳ����Ƿ�ɹ�
	std::pair<data_t, bool> search(const key_t& key) {
		node *result = findNode(key);
		if (result == null) 
			return make_pair(data_t(), false);
		return make_pair(result->data, true);
	}
	// ���룬���ز����Ƿ�ɹ�����key�Ѿ����ڣ��򲻳ɹ�
	bool insert(const key_t& key, const data_t& data) {
		if (findNode(key) != null) return false;
		insert(newNode(key, data));
		return true;
	}
	// ɾ���������Ƿ�ɹ�����key�����ڣ��򲻳ɹ�
	bool remove(const key_t &key) {
		node *result = findNode(key);
		if (result == null) return false;
		remove(result); return true;
	}
	// ��������key�����ڣ�������½ڵ�
	data_t& operator [](const key_t &key) {
		node *result = findNode(key);
		if (result == null)
			result = newNode(key, data_t()), insert(result);
		return result->data;
	}
	// ��������key�����ڣ����ؿյ�data
	const data_t &at(const key_t &key) const {
		node *result = findNode(key);
		if (result == null) return emptyData;
		return result->data;
	}
	// ���� key �Ƿ����
	bool find(const key_t &key) const {
		return findNode(key) != nullptr;
	}
	// ����˳��ת��Ϊ Vector
	Vector<std::pair<key_t, data_t>> list() {
		Vector<std::pair<key_t, data_t>> res;
		_list(res, root);
		return res;
	}

protected:
	node *root;
	// �����½ڵ�
	node* newNode(const key_t& key, const data_t& data) {
		node *x = new node;
		x->ch[0] = x->ch[1] = x->p = null; x->size = 1;
		x->key = key; x->data = data;
		return x;
	}
	// �ͷ�������
	void destructTree(node *rt) {
		if (rt == null) return;
		destructTree(rt->ch[0]); destructTree(rt->ch[1]);
		delete rt;
	}
	// ��ת���� x ��ת������
	void rotate(node *x) {
		if (x == null) return;
		if (x->p == root) root = x;
		bool d = x->direction(); node *p = x->p;
		if (p->p != null) p->p->setChild(x, p->direction()); else x->p = null;
		p->setChild(x->ch[d ^ 1], d); x->setChild(p, d ^ 1); x->update(); p->update();
	}
	// ��ת���� x ��ת������ rt ��λ��
	void splay(node *&rt, node *x) {
		if (x == null) return;
		while (x != rt)
			if (x->p == rt)
				rotate(x);
			else {
				if (x->direction() == x->p->direction())
					rotate(x->p), rotate(x);
				else
					rotate(x), rotate(x);
			}
		rt = x; x->update();
	}
	// ����ڵ�
	void insert(node *z) {
		node *x = root, *y = null;
		if (root == null) { root = z; return; }
		while (x != null)
			if (z->key < x->key)
				y = x, x = x->ch[0];
			else
				y = x, x = x->ch[1];
		if (z->key < y->key)
			y->setChild(z, 0);
		else
			y->setChild(z, 1);
		splay(root, z);
	}
	// ɾ���ڵ㣬�Ƚ� x ��ת��Ҷ��
	void remove(node *x) {
		while (x->ch[0] != null || x->ch[1] != null)
			if (x->ch[0] != null)
				rotate(x->ch[0]);
			else
				rotate(x->ch[1]);
		x->p->ch[x->direction()] = null; x->p->update(); splay(root, x->p);
	}
	// ���ҽڵ㣬���ؽڵ��ַ
	node *findNode(const key_t &key) const {
		node *x = root;
		while (x != null) {
			if (x->key == key) return x;
			if (key < x->key) x = x->ch[0];
			else x = x->ch[1];
		}
		return null;
	}
	// �ݹ�أ����ڵ㰴����� vec
	void _list(Vector<std::pair<key_t, data_t>> &vec, node *x) const {
		if (x == null) return;
		_list(vec, x->ch[0]);
		vec.push_back(std::make_pair(x->key, x->data));
		_list(vec, x->ch[1]);
	}
};

