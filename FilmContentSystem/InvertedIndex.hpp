#pragma once
#include "DocumentList.h"
#include "CharString.h"

struct TermInfo {
	CharString term;
	int termId, DF, occur; // DF: 出现的文档数; occur: 出现总次数
	DocumentList list;
	TermInfo():term(), termId(-1), DF(0), occur(0), list() {}
	bool empty() const { return list.empty(); }
};

template<template <typename T, typename U> typename index_t>
class InvertedIndex
{
	index_t<CharString, TermInfo> index;
	int idCnt;
public:
	InvertedIndex():idCnt(0) { }
	~InvertedIndex() { }
	void inc(const CharString &word, int docId) {
		TermInfo &term = index[word];
		if (term.empty()) {
			term.term = word; term.termId = idCnt++;
			term.DF = 1; term.occur = 1;
			term.list.add(docId, 1);
			return;
		}
		term.occur++;
		int times = static_cast<int>(term.list.search(docId));
		if (times == 0) {
			term.DF++;
			term.list.add(docId, 1);
		}
		else
			term.list.edit(docId, times + 1);
	}
	void add(const CharString &word, int docId, double rating) {
		TermInfo &term = index[word];
		if (term.empty()) {
			term.term = word; term.termId = idCnt++;
		}
		term.DF++; term.occur++;
		term.list.add(docId, rating);
	}
	TermInfo search(const CharString &word) const {
		if (index.find(word))
			return index.at(word);
		else
			return TermInfo();
	}
};

