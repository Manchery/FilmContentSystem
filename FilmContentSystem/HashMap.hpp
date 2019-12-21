#pragma once

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include <stdexcept>

using hash_t = unsigned long long;	// hashֵ������

const unsigned LEN_HASH_SIZES = 27;	// HASH_SIZES ����Ĵ�С
const unsigned HASH_SIZES[] = {		// ����2�Ĵ��ݵ���С����������Hash���ģ��
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
˵����	ģ���࣬ʵ���˹�ϣ��Ļ�������
		��̬�����ڴ棬����洢
�ӿڣ�	HashMap������insert/find/ȡ�±�
		״̬��ѯ��size
		�ڴ���䣺reserve
*/

// key_t, value_t: ��ֵ����; hashFunc: ��ϣ����
template<typename key_t, typename value_t>
class HashMap
{
private:
	struct List {
		key_t key;
		value_t value;
		List* next;
	};
	unsigned hashSize;	// ��������ΪHash���ģ����Ҳ����Hash���ͷ����head�Ĵ�С
	List **head;		// ��ͷ
	HashFunc<key_t> hashFunc;	// hash����
	value_t emptyValue;

	// ��������
	void destructList(List *node) {
		if (node == nullptr) return;
		destructList(node->next);
		delete node;
	}
	
	int _size;			// Ԫ������

	void realloc(unsigned required_size) {	// ��������
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

		if (head != nullptr) {	// ��ԭ��������ڵ㿽�����µı�ͷ��
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
	// ��δʵ�֣�����Ŀ�в���Ҫ�õ�HashMap����������������Ϊɾ��
	// �����ڴ�����ʵ�ַ�ʽ������ʱ����������
	HashMap(const HashMap&) = delete;
	HashMap& operator=(const HashMap&) = delete;

	int size() const {
		return _size;
	}

	void reserve(int capacity) {		// ����������������飬��ֹ����תΪ�޷���
		if (capacity < 0) throw std::length_error("Capacity should be non-negative!");
		realloc(capacity);
	}
	// ����һ����ֵ��
	void insert(const key_t &key, const value_t & value) {
		if (hashSize == 0 || _size >= hashSize/2) realloc(hashSize + 1);	// ����ϵ�� 0.5
		unsigned h = hashFunc(key) % hashSize;
		List* node = new List{ key, value, nullptr }; node->next = head[h]; head[h] = node;
		_size++;
	}
	// ���key�Ƿ��ڱ��д���
	bool find(const key_t &key) const {
		unsigned h = hashFunc(key) % hashSize;
		for (List* p = head[h]; p; p = p->next)
			if (p->key == key)
				return true;
		return false;
	}
	// ����key���������ڣ��򷵻ؿ�ֵ
	const value_t& at(const key_t &key) const {
		unsigned h = hashFunc(key) % hashSize;
		for (List* p = head[h]; p; p = p->next)
			if (p->key == key)
				return p->value;
		return emptyValue;
	}
	// ����key����key�ڱ��в����ڣ��򴴽�һ�ڵ�
	value_t& operator [](const key_t &key) {
		if (hashSize > 0) {
			unsigned h = hashFunc(key) % hashSize;
			for (List* p = head[h]; p; p = p->next)
				if (p->key == key)
					return p->value;
		}
		// key�����ڣ������½ڵ�
		if (hashSize == 0 || _size >= hashSize/2) realloc(hashSize + 1);	// ����ϵ�� 0.5
		unsigned h = hashFunc(key) % hashSize;
		List* node = new List{ key, value_t(), nullptr }; node->next = head[h]; head[h] = node;
		_size++;
		return node->value;
	}
};