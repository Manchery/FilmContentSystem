#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stdexcept>

template<typename value_t>
class Stack
{
public:
	using iterator = value_t * ;
private:
	iterator _stack;
	int _top, _capacity;
public:
	Stack() {
		_top = -1; _capacity = 0;
		_stack = nullptr;
	}
	~Stack() {
		delete[] _stack;
	}
	int size() const { return _top + 1; }
	int capacity() const { return _capacity; }
	void reverse(int cap) {
		if (_capacity < cap) {
			iterator newStack = new value_t[cap];
			memcpy(newStack, _stack, size() * sizeof(value_t);
			delete[] _stack; _stack = newStack;
		}
	}
	void push(const value_t & item) {
		if (_capacity == 0 || _top == _capacity - 1) {
			reverse(_capacity == 0 ? 1 : _capacity * 2);
		}
		_stack[++_top] = item;
	}
	void pop() {
		if (empty())
			throw std::out_of_range(""); // TODO
		--_top;
	}
	void top() const {
		if (empty())
			throw std::out_of_range(""); // TODO 
		return _stack[_top];
	}
	bool empty() const {
		return _top == -1;
	}
};
