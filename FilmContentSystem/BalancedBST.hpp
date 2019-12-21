#pragma once
#include "Vector.hpp"
#include <algorithm>

/*
SplayTree 平衡二叉树 模板类
说明：	平衡二叉树，采用 Splay tree 实现
		Reference: [Splay tree - Wikipedia](https://en.wikipedia.org/wiki/Splay_tree)
接口：	修改：adjust, insert, remove
		查询：search, find, at, operator [], list
*/

// key_t 键类型；data_t 值类型
template<typename key_t, typename data_t>
class SplayTree
{
public:
	struct node {	// 树上节点类型
		// p 父亲; ch 儿子; size 子树大小
		node *p, *ch[2]; int size;
		key_t key;  data_t data;
		// 是左儿子返回 0，右儿子返回 1
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
	// 调整使二叉树平衡，具体地将 x 旋转到根
	void adjust(node *x) {
		splay(root, x); 
	}
	// 查找，返回对应的值和查找是否成功
	std::pair<data_t, bool> search(const key_t& key) {
		node *result = findNode(key);
		if (result == null) 
			return make_pair(data_t(), false);
		return make_pair(result->data, true);
	}
	// 插入，返回插入是否成功（若key已经存在，则不成功
	bool insert(const key_t& key, const data_t& data) {
		if (findNode(key) != null) return false;
		insert(newNode(key, data));
		return true;
	}
	// 删除，返回是否成功，若key不存在，则不成功
	bool remove(const key_t &key) {
		node *result = findNode(key);
		if (result == null) return false;
		remove(result); return true;
	}
	// 索引，若key不存在，则加入新节点
	data_t& operator [](const key_t &key) {
		node *result = findNode(key);
		if (result == null)
			result = newNode(key, data_t()), insert(result);
		return result->data;
	}
	// 索引，若key不存在，返回空的data
	const data_t &at(const key_t &key) const {
		node *result = findNode(key);
		if (result == null) return emptyData;
		return result->data;
	}
	// 查找 key 是否存在
	bool find(const key_t &key) const {
		return findNode(key) != nullptr;
	}
	// 按照顺序转化为 Vector
	Vector<std::pair<key_t, data_t>> list() {
		Vector<std::pair<key_t, data_t>> res;
		_list(res, root);
		return res;
	}

protected:
	node *root;
	// 创造新节点
	node* newNode(const key_t& key, const data_t& data) {
		node *x = new node;
		x->ch[0] = x->ch[1] = x->p = null; x->size = 1;
		x->key = key; x->data = data;
		return x;
	}
	// 释放整棵树
	void destructTree(node *rt) {
		if (rt == null) return;
		destructTree(rt->ch[0]); destructTree(rt->ch[1]);
		delete rt;
	}
	// 旋转，将 x 旋转到父亲
	void rotate(node *x) {
		if (x == null) return;
		if (x->p == root) root = x;
		bool d = x->direction(); node *p = x->p;
		if (p->p != null) p->p->setChild(x, p->direction()); else x->p = null;
		p->setChild(x->ch[d ^ 1], d); x->setChild(p, d ^ 1); x->update(); p->update();
	}
	// 旋转，将 x 旋转到祖先 rt 的位置
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
	// 插入节点
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
	// 删除节点，先将 x 旋转到叶子
	void remove(node *x) {
		while (x->ch[0] != null || x->ch[1] != null)
			if (x->ch[0] != null)
				rotate(x->ch[0]);
			else
				rotate(x->ch[1]);
		x->p->ch[x->direction()] = null; x->p->update(); splay(root, x->p);
	}
	// 查找节点，返回节点地址
	node *findNode(const key_t &key) const {
		node *x = root;
		while (x != null) {
			if (x->key == key) return x;
			if (key < x->key) x = x->ch[0];
			else x = x->ch[1];
		}
		return null;
	}
	// 递归地，将节点按序加入 vec
	void _list(Vector<std::pair<key_t, data_t>> &vec, node *x) const {
		if (x == null) return;
		_list(vec, x->ch[0]);
		vec.push_back(std::make_pair(x->key, x->data));
		_list(vec, x->ch[1]);
	}
};

