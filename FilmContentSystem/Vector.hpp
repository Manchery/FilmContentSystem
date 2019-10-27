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
	Vector() {
		_size = 0; _capacity = 0;
		_vector = nullptr;
	}
	Vector(int initCap) {
		_size = 0; _capacity = initCap;
		_vector = new value_t[initCap];
	}
	Vector(const Vector &b) {
		_size = _capacity = b.size();
		_vector = new value_t[_capacity];
		for (int i = 0; i < _size; i++)
			_vector[i] = b[i];
	}
	Vector& operator=(const Vector &b) {
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
	value_t &operator[](int x) {
		return _vector[x];
	}
	const value_t &operator[](int x) const {
		return _vector[x];
	}
	const value_t &at(int x) const {
		if (x < 0 || x >= _size)
			throw std::out_of_range("Index of vector out of range!");
		return _vector[x];
	}
	bool empty() const {
		return _size == 0;
	}
	
};

