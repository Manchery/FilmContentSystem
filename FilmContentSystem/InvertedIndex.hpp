#pragma once
#include "DocumentList.h"
#include "CharString.h"

/*
TermInfo
说明：	倒排文档中存储索引对应相关信息的数据类型
*/

struct TermInfo {
	CharString term;		// 单词
	int termId, DF, occur;	// DF: 单词出现的文档数; occur: 单词出现总次数
	DocumentList list;		// 对应的文档链表
	TermInfo():term(), termId(-1), DF(0), occur(0), list() {}
	bool empty() const { return list.empty(); }
};

/*
InvertedIndex 模板类
说明：	倒排文档，由索引数据结构和文档链表两部分组成
		能够实现根据内容查询文档的快速操作
接口：	修改：inc, add
		查询：search
*/

// index_t 索引数据结构，需要支持 find, at, operator [] 接口
template<template <typename T, typename U> typename index_t>
class InvertedIndex
{
	index_t<CharString, TermInfo> index;
	int idCnt;	// id 总数，默认新加入一个单词，赋予最大的id
public:
	InvertedIndex():idCnt(0) { }
	~InvertedIndex() { }
	// 在 word 对应的文档链表中找到 docId 对应的文档，并将出现次数加一
	void inc(const CharString &word, int docId) {
		TermInfo &term = index[word];
		if (term.empty()) {	// 若单词在index中不存在
			term.term = word; term.termId = idCnt++;
			term.DF = 1; term.occur = 1;
			term.list.add(docId, 1);
			return;
		}
		term.occur++;
		int times = static_cast<int>(term.list.search(docId));
		if (times == 0) {	// 若docId在链表中不存在
			term.DF++;
			term.list.add(docId, 1);
		}
		else
			term.list.edit(docId, times + 1);
	}
	// 在 word 对应的文档链表中插入文档 (docId, rating)
	// 假设 docId 在链表中不存在
	void add(const CharString &word, int docId, double rating) {
		TermInfo &term = index[word];
		if (term.empty()) {	// 若单词在index中不存在
			term.term = word; term.termId = idCnt++;
		}
		term.DF++; term.occur++;
		term.list.add(docId, rating);
	}
	// 查找 word 对应的TermInfo
	const TermInfo& search(const CharString &word) const {
		return index.at(word);
	}
};

