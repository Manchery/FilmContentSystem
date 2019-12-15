#pragma once
#include "DocumentList.h"
#include "CharString.h"

/*
TermInfo
˵����	�����ĵ��д洢������Ӧ�����Ϣ����������
*/

struct TermInfo {
	CharString term;		// ����
	int termId, DF, occur;	// DF: ���ʳ��ֵ��ĵ���; occur: ���ʳ����ܴ���
	DocumentList list;		// ��Ӧ���ĵ�����
	TermInfo():term(), termId(-1), DF(0), occur(0), list() {}
	bool empty() const { return list.empty(); }
};

/*
InvertedIndex ģ����
˵����	�����ĵ������������ݽṹ���ĵ��������������
		�ܹ�ʵ�ָ������ݲ�ѯ�ĵ��Ŀ��ٲ���
�ӿڣ�	�޸ģ�inc, add
		��ѯ��search
*/

// index_t �������ݽṹ����Ҫ֧�� find, at, operator [] �ӿ�
template<template <typename T, typename U> typename index_t>
class InvertedIndex
{
	index_t<CharString, TermInfo> index;
	int idCnt;	// id ������Ĭ���¼���һ�����ʣ���������id
public:
	InvertedIndex():idCnt(0) { }
	~InvertedIndex() { }
	// �� word ��Ӧ���ĵ��������ҵ� docId ��Ӧ���ĵ����������ִ�����һ
	void inc(const CharString &word, int docId) {
		TermInfo &term = index[word];
		if (term.empty()) {	// ��������index�в�����
			term.term = word; term.termId = idCnt++;
			term.DF = 1; term.occur = 1;
			term.list.add(docId, 1);
			return;
		}
		term.occur++;
		int times = static_cast<int>(term.list.search(docId));
		if (times == 0) {	// ��docId�������в�����
			term.DF++;
			term.list.add(docId, 1);
		}
		else
			term.list.edit(docId, times + 1);
	}
	// �� word ��Ӧ���ĵ������в����ĵ� (docId, rating)
	// ���� docId �������в�����
	void add(const CharString &word, int docId, double rating) {
		TermInfo &term = index[word];
		if (term.empty()) {	// ��������index�в�����
			term.term = word; term.termId = idCnt++;
		}
		term.DF++; term.occur++;
		term.list.add(docId, rating);
	}
	// ���� word ��Ӧ��TermInfo
	const TermInfo& search(const CharString &word) const {
		return index.at(word);
	}
};

