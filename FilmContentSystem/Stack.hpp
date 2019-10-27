#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stdexcept>

/*
Stack
说明：	模仿STL实现的模板类，实现了栈的基本功能
		动态分配内存，实现 push 操作均摊复杂度 O(1)
接口：	栈操作：push/pop
		状态查询：size/capacity/top/empty
		内存分配：reserve
*/

template<typename value_t>			// value_t 值类型
class Stack
{
public:
	using iterator = value_t * ;	// 迭代器类型
private:
	value_t *_stack;
	int _top, _capacity;			// top表示栈顶的下标
									// 满足 top<=capacity-1, top==size-1
public:
	Stack() {
		_top = -1; _capacity = 0;
		_stack = nullptr;
	}
	Stack(int initCap) {			// 给定初始容量构造
		_top = -1; _capacity = initCap;
		_stack = new value_t[initCap];
	}
	Stack(const Stack &b) {			// 拷贝构造函数
		_capacity = b.size(); _top = _capacity - 1;
		_stack = new value_t[_capacity];
		for (int i = 0; i <= _top; i++)
			_stack[i] = b._stack[i];
	}
	Stack& operator=(const Stack &b) {	// 拷贝运算符
		if (_capacity < b.size()) {
			delete[] _stack;
			_capacity = b.size(); _top = _capacity - 1;
			_stack = new value_t[_capacity];
			for (int i = 0; i <= _top; i++)
				_stack[i] = b._stack[i];
		}
		else {
			_top = b._top;
			for (int i = 0; i <= _top; i++)
				_stack[i] = b[i];
		}
		return *this;
	}
	~Stack() {
		delete[] _stack;
	}
	int size() const { return _top + 1; }
	int capacity() const { return _capacity; }
	void reserve(int cap) {			// 扩充容量，只有要求容量大于当前容量时，才重新分配内存
		if (_capacity < cap) {
			value_t* newStack = new value_t[cap];
			for (int i = 0; i <= _top; i++) newStack[i] = _stack[i];
			delete[] _stack; _stack = newStack;
			_capacity = cap;
		}
	}
	void push(const value_t & item) {
		if (_capacity == 0 || _top == _capacity - 1) {
			// 如果当前容量不足，将容量翻倍，该操作的均摊复杂度为 O(1)
			reserve(_capacity == 0 ? 1 : _capacity * 2);
		}
		_stack[++_top] = item;
	}
	void pop() {
		if (empty())
			throw std::logic_error("The stack is empty!");
		--_top;
	}
	value_t &top() const {
		if (empty())
			throw std::logic_error("The stack is empty!");
		return _stack[_top];
	}
	bool empty() const {
		return _top == -1;
	}
};

