#pragma once

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <stdexcept>

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

template<typename key_t, typename value_t, hash_t hashFunc(const key_t&)>
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

	void realloc(unsigned required_size) {
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

		if (head != nullptr) {
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
	}
	~HashMap() {
		if (head != nullptr) {
			for (unsigned h = 0; h < hashSize; h++) {
				destructList(head[h]);
			}
			delete[] head;
		}
	}

	int size() const {
		return _size;
	}

	void reserve(int capacity) {
		if (capacity < 0) throw std::length_error("Capacity should be non-negative!"); //TODO
		realloc(capacity);
	}
	void insert(const key_t &key, const value_t & value) {
		if (hashSize == 0 || _size == hashSize) realloc(hashSize + 1);
		unsigned h = hashFunc(key) % hashSize;
		List* node = new List{ key, value, nullptr }; node->next = head[h]; head[h] = node;
		_size++;
	}
	bool find(const key_t &key) {
		unsigned h = hashFunc(key) % hashSize;
		for (List* p = head[h]; p; p = p->next)
			if (p->key == key)
				return true;
		return false;
	}
	value_t& operator [](const key_t &key) {
		if (hashSize > 0) {
			unsigned h = hashFunc(key) % hashSize;
			for (List* p = head[h]; p; p = p->next)
				if (p->key == key)
					return p->value;
		}

		if (hashSize == 0 || _size == hashSize) realloc(hashSize + 1);
		unsigned h = hashFunc(key) % hashSize;
		List* node = new List{ key, value_t(), nullptr }; node->next = head[h]; head[h] = node;
		_size++;
		return node->value;
	}
};

