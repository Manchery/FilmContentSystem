#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <stdexcept>

/*
Stack
˵����	ģ��STLʵ�ֵ�ģ���࣬ʵ����ջ�Ļ�������
		��̬�����ڴ棬ʵ�� push ������̯���Ӷ� O(1)
�ӿڣ�	ջ������push/pop
		״̬��ѯ��size/capacity/top/empty
		�ڴ���䣺reserve
*/

template<typename value_t>			// value_t ֵ����
class Stack
{
public:
	using iterator = value_t * ;	// ����������
private:
	value_t *_stack;
	int _top, _capacity;			// top��ʾջ�����±�
									// ���� top<=capacity-1, top==size-1
public:
	Stack() {
		_top = -1; _capacity = 0;
		_stack = nullptr;
	}
	Stack(int initCap) {			// ������ʼ��������
		_top = -1; _capacity = initCap;
		_stack = new value_t[initCap];
	}
	Stack(const Stack &b) {			// �������캯��
		_capacity = b.size(); _top = _capacity - 1;
		_stack = new value_t[_capacity];
		for (int i = 0; i <= _top; i++)
			_stack[i] = b._stack[i];
	}
	Stack& operator=(const Stack &b) {	// ���������
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
	void reserve(int cap) {			// ����������ֻ��Ҫ���������ڵ�ǰ����ʱ�������·����ڴ�
		if (_capacity < cap) {
			value_t* newStack = new value_t[cap];
			for (int i = 0; i <= _top; i++) newStack[i] = _stack[i];
			delete[] _stack; _stack = newStack;
			_capacity = cap;
		}
	}
	void push(const value_t & item) {
		if (_capacity == 0 || _top == _capacity - 1) {
			// �����ǰ�������㣬�������������ò����ľ�̯���Ӷ�Ϊ O(1)
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

