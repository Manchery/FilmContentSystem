#pragma once
#include <algorithm>

/*
DocumentList 
说明：	文档链表，链表中的节点存储了文档相关的信息
		链表中节点按照rating排序，是实现倒排文档的基本结构
接口：	修改：add, remove, edit
		查询：search, empty
		遍历：begin, end
*/

class DocumentList{
public:
	struct node {
		int id;			// 文档 id
		double rating;	// 文档在链表中的排序依据，具体地，FilmContentSystemApplication中：
						// 在依据分词建立的倒排文档中，rating表示 **单词出现次数**
						// 在依据电影类型建立的倒排文档中，rating表示电影的评分
		node *prev, *next;
		node(int id = -1, double rating = -1) :id(id), rating(rating), prev(nullptr), next(nullptr) { }
		~node() { }
	};
	// 单词的id
	int termId;

	// 迭代器类型及相关操作
	class iterator {
		node *cur;
	public:
		iterator(node * n) :cur(n) {}
		iterator& operator ++ () { cur = cur->next; return *this; }
		iterator next() const { return iterator(cur->next); }
		bool operator == (const iterator & b) const { return cur == b.cur; }
		bool operator != (const iterator & b) const { return cur != b.cur; }
		int id() const { return cur->id; }
		double rating() const { return cur->rating; }
	};
	// 分别返回开始迭代器和结束迭代器
	// begin、end、operator++ 配合使用可实现对链表的遍历操作
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(nullptr); }

	DocumentList(int termId = -1);
	DocumentList(const DocumentList &b);
	~DocumentList();

	DocumentList& operator = (const DocumentList &b);

	// 插入文档，并维持 rating 降序
	// 返回插入是否成功（若id已存在，则不成功
	bool add(int id, double rating = 1);
	// 删除文档，返回是否成功（若id不存在，则不成功
	bool remove(int id);
	// 查找 id，并返回对应的rating（单词出现次数
	double search(int id) const;
	// 修改 id 对应的rating，并维持有序
	bool edit(int id, double rating);

	bool empty() const { return head == nullptr; }
protected:
	node *head;
	// 释放链表
	void destructList(node *x);
	// 查找 id 对应的节点地址
	node *findNode(int id) const;
	// 调整 x 到合适位置使得链表有序，需要保证链表其余节点有序
	void adjust(node * x);
};
