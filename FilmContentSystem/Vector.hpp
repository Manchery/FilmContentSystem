#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stdexcept>

template<typename value_t>
class Vector
{
public:
	using iterator = value_t * ;
private:
	value_t *_vector;
	int _size, _capacity;
public:
	Stack() {
		_size = 0; _capacity = 0;
		_vector = nullptr;
	}
	Stack(int initCap) {
		_size = 0; _capacity = initCap;
		_vector = new value_t[initCap];
	}
	~Stack() {
		delete[] _vector;
	}
	int size() const { return _size; }
	int capacity() const { return _capacity; }
	void reserve(int cap) {
		if (_capacity < cap) {
			iterator newStack = new value_t[cap];
			for (int i = 0; i < _size; i++) newStack[i] = _vector[i];
			delete[] _vector; _vector = newStack;
			_capacity = cap;
		}
	}
	void push_back(const value_t & item) {
		if (_capacity == 0 || _size == _capacity) {
			reserve(_capacity == 0 ? 1 : _capacity * 2);
		}
		_vector[_size++] = item;
	}
	void pop_back() {
		if (empty())
			throw std::logic_error("The vector is empty!");
		--_size;
	}
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
	bool empty() const {
		return _size == 0;
	}
};

