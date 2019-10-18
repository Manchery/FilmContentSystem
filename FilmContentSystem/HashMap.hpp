#pragma once

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <algorithm>

using hash_t = unsigned long long;
const unsigned LEN_HASH_SIZES = 27;
const unsigned HASH_SIZES[] = { // 大于2的次幂的最小质数
	2, 3, 5, 11, 
	17, 37, 67, 131, 
	257, 521, 1031, 2053, 
	4099, 8209, 16411, 32771, 
	65537, 131101, 262147, 524309, 
	1048583, 2097169, 4194319, 8388617, 
	16777259, 33554467, 67108879
}; 

template<typename key_t, typename value_t, hash_t *hashFunc(const key_t&)>
class HashMap
{
private:
	struct List {
		key_t key;
		value_t value;
		List* next;
	};
	unsigned hashSize;
	List **head;

	void destructList(List *node) {
		if (node == nullptr) return;
		destructList(node->next);
		delete node;
	}
	
	int _size;

	void realloc(unsigned required_size = hashSize+1) {
		if (hashSize >= required_size) return;
		int newSize = newSize;
		for (int i=0;i<LEN_HASH_SIZES;i++)
			if (HASH_SIZES[i] >= required_size) {
				newSize = HASH_SIZES[i];
				break;
			}
		if (newSize == hashSize)
			throw std::bad_alloc();

		List** newHead = new List*[newSize]{nullptr};
		for (int h = 0; h < newSize; h++) assert(newHead[h] == nullptr);

		if (head != nullptr) {
			for (unsigned h = 0; h < hashSize; h++) {
				for (List* p = head[h]; p; p = p->next) {
					unsigned newH = hashFunc(p->key) % hashSize;
					p->next = newHead[newH]; newHead[newH] = p;
				}
			}
		}

		delete[] head; head = newHead; hashSize = newSize;
	}
public:
	HashMap() {
		head = nullptr; 
		_size = 0; hashSize = 0;
	}
	~HashMap() {
		if (head != nullptr) {
			for (unsigned h = 0; h < hashSize; h++) {
				destructList(head[h]);
			}
			delete[] head;
		}
	}

	void reserve(int capacity) {
		if (capacity < 0) throw std::length_error();
		realloc(capacity);
	}
	void insert(const key_t &key, const value_t & value) {
		if (hashSize == 0 || _size == hashSize) realloc();
		unsigned h = hashFunc(key) % hashSize;
		List* node = new List{ key, value, nullptr }; node->next = head[h]; head[h] = node;
		_size++;
	}
	int size() const { 
		return _size; 
	}

	bool find(const key_t &key) {
		unsigned h = hashFunc(key) % hashSize;
		for (List* p = head[h]; p; p = p->next)
			if (p->key == key)
				return true;
		return false;
	}
	value_t& operator [](const key_t &key) {
		unsigned h = hashFunc(key) % hashSize;
		for (List* p = head[h]; p; p = p->next)
			if (p->key == key)
				return p->value;

		if (hashSize == 0 || _size == hashSize) realloc();
		h = hashFunc(key) % hashSize;
		List* node = new List{ key, value_t(), nullptr }; node->next = head[h]; head[h] = node;
		return node->value;
	}
};

