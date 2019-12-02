#pragma once
#include "DocumentList.h"
#include "CharString.h"

struct TermInfo {
	CharString term;
	int termId, DF, occur;
	DocumentList list; // TODO: memory leaks ?
	TermInfo():term(), termId(-1), DF(0), occur(0), list() {}
	bool empty() const { return list.empty(); }
};

template<typename index_t>
class InvertedIndex
{
	index_t<int, TermInfo> index;
public:
	InvertedIndex() { }
	~InvertedIndex() { }
	void inc(const CharString &word, int wordId, int docId) {
		TermInfo term = index[wordId];
		if (term.empty()) {
			term.term = word; term.termId = wordId;
			term.DF = 1; term.occur = 1;
			term.list.add(docId, 1);
			return;
		}
		term.occur++;
		int times = term.list.search(docId)
		if (times == 0) term.DF++;
		term.list.edit(docId, times + 1);
	}
	void add(const CharString &word, int wordId, int docId, double rating) {
		TermInfo term = index[wordId];
		term.term = word; term.termId = wordId;
		term.DF++; term.occur++;
		term.list.add(docId, rating);
	}
	TermInfo search(int wordId) {
		if (index.find(wordId))
			return index[wordId];
		else
			return TermInfo();
	}
};

