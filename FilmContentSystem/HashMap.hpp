#pragma once

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <stdexcept>

using hash_t = unsigned long long;	// hash值的类型

const unsigned LEN_HASH_SIZES = 27;	// HASH_SIZES 数组的大小
const unsigned HASH_SIZES[] = {		// 大于2的次幂的最小质数，用作Hash表的模数
	2, 3, 5, 11, 
	17, 37, 67, 131, 
	257, 521, 1031, 2053, 
	4099, 8209, 16411, 32771, 
	65537, 131101, 262147, 524309, 
	1048583, 2097169, 4194319, 8388617, 
	16777259, 33554467, 67108879
}; 

template<typename key_t>
class HashFunc {
public:
	hash_t operator ()(const key_t&) const { return hash_t(); }
};

/*
HashMap
说明：	模板类，实现了哈希表的基本功能
		动态分配内存，链表存储
接口：	HashMap操作：insert/find/取下标
		状态查询：size
		内存分配：reserve
*/

// key_t, value_t: 键值类型; hashFunc: 哈希函数
template<typename key_t, typename value_t>
class HashMap
{
private:
	struct List {
		key_t key;
		value_t value;
		List* next;
	};
	unsigned hashSize;	// 质数，作为Hash表的模数，也就是Hash表表头数组head的大小
	List **head;		// 表头
	HashFunc<key_t> hashFunc;	// hash函数
	value_t emptyValue;

	// 链表析构
	void destructList(List *node) {
		if (node == nullptr) return;
		destructList(node->next);
		delete node;
	}
	
	int _size;			// 元素总数

	void realloc(unsigned required_size) {	// 扩充容量
		if (hashSize >= required_size) return;
		int newSize = hashSize;
		for (int i=0;i<LEN_HASH_SIZES;i++)
			if (HASH_SIZES[i] >= required_size) {
				newSize = HASH_SIZES[i];
				break;
			}
		if (newSize == hashSize)
			throw std::length_error("Creating a too big hash table!"); 

		List** newHead = new List*[newSize]{nullptr};
		for (int h = 0; h < newSize; h++) assert(newHead[h] == nullptr);

		if (head != nullptr) {	// 将原来的链表节点拷贝到新的表头上
			for (unsigned h = 0; h < hashSize; h++) {
				for (List* p = head[h],*last_next; p; p = last_next) {
					unsigned newH = hashFunc(p->key) % newSize;
					last_next = p->next; p->next = newHead[newH]; newHead[newH] = p;
				}
			}
		}

		delete[] head; head = newHead; hashSize = newSize;
	}
public:
	HashMap() {
		hashSize = 0; head = nullptr;
		_size = 0; 
		emptyValue = value_t();
	}
	~HashMap() {
		if (head != nullptr) {
			for (unsigned h = 0; h < hashSize; h++) {
				destructList(head[h]);
			}
			delete[] head;
		}
	}

	// TODO (wujialong)
	// 暂未实现，本项目中不需要用到HashMap拷贝操作，故声明为删除
	// 由于内存分配的实现方式，拷贝时必须采用深拷贝
	HashMap(const HashMap&) = delete;
	HashMap& operator=(const HashMap&) = delete;

	int size() const {
		return _size;
	}

	void reserve(int capacity) {		// 扩充容量，参数检查，防止负数转为无符号
		if (capacity < 0) throw std::length_error("Capacity should be non-negative!");
		realloc(capacity);
	}
	// 插入一个键值对
	void insert(const key_t &key, const value_t & value) {
		if (hashSize == 0 || _size >= hashSize/2) realloc(hashSize + 1);	// 负载系数 0.5
		unsigned h = hashFunc(key) % hashSize;
		List* node = new List{ key, value, nullptr }; node->next = head[h]; head[h] = node;
		_size++;
	}
	// 检查key是否在表中存在
	bool find(const key_t &key) const {
		unsigned h = hashFunc(key) % hashSize;
		for (List* p = head[h]; p; p = p->next)
			if (p->key == key)
				return true;
		return false;
	}
	// 索引key，若不存在，则返回空值
	const value_t& at(const key_t &key) const {
		unsigned h = hashFunc(key) % hashSize;
		for (List* p = head[h]; p; p = p->next)
			if (p->key == key)
				return p->value;
		return emptyValue;
	}
	// 索引key，若key在表中不存在，则创建一节点
	value_t& operator [](const key_t &key) {
		if (hashSize > 0) {
			unsigned h = hashFunc(key) % hashSize;
			for (List* p = head[h]; p; p = p->next)
				if (p->key == key)
					return p->value;
		}
		// key不存在，创建新节点
		if (hashSize == 0 || _size >= hashSize/2) realloc(hashSize + 1);	// 负载系数 0.5
		unsigned h = hashFunc(key) % hashSize;
		List* node = new List{ key, value_t(), nullptr }; node->next = head[h]; head[h] = node;
		_size++;
		return node->value;
	}
};