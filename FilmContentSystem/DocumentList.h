#pragma once
#include <algorithm>

/*
DocumentList 
˵����	�ĵ����������еĽڵ�洢���ĵ���ص���Ϣ
		�����нڵ㰴��rating������ʵ�ֵ����ĵ��Ļ����ṹ
�ӿڣ�	�޸ģ�add, remove, edit
		��ѯ��search, empty
		������begin, end
*/

class DocumentList{
public:
	struct node {
		int id;			// �ĵ� id
		double rating;	// �ĵ��������е��������ݣ�����أ�FilmContentSystemApplication�У�
						// �����ݷִʽ����ĵ����ĵ��У�rating��ʾ **���ʳ��ִ���**
						// �����ݵ�Ӱ���ͽ����ĵ����ĵ��У�rating��ʾ��Ӱ������
		node *prev, *next;
		node(int id = -1, double rating = -1) :id(id), rating(rating), prev(nullptr), next(nullptr) { }
		~node() { }
	};
	// ���ʵ�id
	int termId;

	// ���������ͼ���ز���
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
	// �ֱ𷵻ؿ�ʼ�������ͽ���������
	// begin��end��operator++ ���ʹ�ÿ�ʵ�ֶ�����ı�������
	iterator begin() const { return iterator(head); }
	iterator end() const { return iterator(nullptr); }

	DocumentList(int termId = -1);
	DocumentList(const DocumentList &b);
	~DocumentList();

	DocumentList& operator = (const DocumentList &b);

	// �����ĵ�����ά�� rating ����
	// ���ز����Ƿ�ɹ�����id�Ѵ��ڣ��򲻳ɹ�
	bool add(int id, double rating = 1);
	// ɾ���ĵ��������Ƿ�ɹ�����id�����ڣ��򲻳ɹ�
	bool remove(int id);
	// ���� id�������ض�Ӧ��rating�����ʳ��ִ���
	double search(int id) const;
	// �޸� id ��Ӧ��rating����ά������
	bool edit(int id, double rating);

	bool empty() const { return head == nullptr; }
protected:
	node *head;
	// �ͷ�����
	void destructList(node *x);
	// ���� id ��Ӧ�Ľڵ��ַ
	node *findNode(int id) const;
	// ���� x ������λ��ʹ������������Ҫ��֤��������ڵ�����
	void adjust(node * x);
};
