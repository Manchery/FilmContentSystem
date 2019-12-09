#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stdexcept>

/*
Vector
说明：	模仿STL实现的模板类，实现了向量的基本功能
		动态分配内存，实现 push_back 操作均摊复杂度 O(1)
接口：	向量操作：push_back/pop_back/取下标
		状态查询：size/capacity/front/back/empty
		内存分配：reserve
*/

template<typename value_t>			// value_t 值类型
class Vector
{
public:
	using iterator = value_t * ;	// 迭代器类型
private:
	value_t *_vector;
	int _size, _capacity;			// 满足 size<=capacity
public:
	Vector() {
		_size = 0; _capacity = 0;
		_vector = nullptr;
	}
	Vector(int initCap) {			// 给定初始容量构造
		_size = 0; _capacity = initCap;
		_vector = new value_t[initCap];
	}
	Vector(const Vector &b) {		// 拷贝构造函数
		_size = _capacity = b.size();
		_vector = new value_t[_capacity];
		for (int i = 0; i < _size; i++)
			_vector[i] = b[i];
	}
	Vector& operator=(const Vector &b) {	// 拷贝运算符
		if (_capacity < b._size) {
			delete[] _vector;
			_size = _capacity = b.size();
			_vector = new value_t[_capacity];
			for (int i = 0; i < _size; i++)
				_vector[i] = b[i];
		}
		else {
			_size = b._size;
			for (int i = 0; i < _size; i++)
				_vector[i] = b[i];
		}
		return *this;
	}
	~Vector() {
		delete[] _vector;
	}

	int size() const { return _size; }
	int capacity() const { return _capacity; }
	value_t back() const {
		if (empty())
			throw std::logic_error("The vector is empty!");
		return _vector[_size - 1];
	}
	value_t front() const {
		if (empty())
			throw std::logic_error("The vector is empty!");
		return _vector[0];
	}

	void reserve(int cap) {			// 扩充容量，只有要求容量大于当前容量时，才重新分配内存
		if (_capacity < cap) {
			iterator newStack = new value_t[cap];
			for (int i = 0; i < _size; i++) newStack[i] = _vector[i];
			delete[] _vector; _vector = newStack;
			_capacity = cap;
		}
	}
	void resize(int siz) {
		reserve(siz);
		_size = siz;
	}
	void push_back(const value_t & item) {
		if (_capacity == 0 || _size == _capacity) {
			// 如果当前容量不足，将容量翻倍，该操作的均摊复杂度为 O(1)
			reserve(_capacity == 0 ? 1 : _capacity * 2);
		}
		_vector[_size++] = item;
	}
	void pop_back() {
		if (empty())
			throw std::logic_error("The vector is empty!");
		--_size;
	}
	// 无范围检查的下标操作
	value_t &operator[](int x) {
		return _vector[x];
	}
	const value_t &operator[](int x) const {
		return _vector[x];
	}
	// 有范围检查的下标操作
	const value_t &at(int x) const {
		if (x < 0 || x >= _size)
			throw std::out_of_range("Index of vector out of range!");
		return _vector[x];
	}
	bool empty() const {
		return _size == 0;
	}
	
	// 按照给定的比较函数排序
	typedef bool (*cmpFunc)(const value_t&, const value_t&);
	void sort(cmpFunc cmp) {
		std::sort(_vector, _vector + _size, cmp);
	}
	// 按照默认顺序排序
	void sort() {
		std::sort(_vector, _vector + _size);
	}
	// 去重，要求有序
	void unique() {
		int pnt = 0;
		for (int i = 0; i < _size; i++)
			if (i == 0 || !(_vector[i] == _vector[i - 1]))
				_vector[pnt++] = _vector[i];
		_size = pnt;
	}
};

