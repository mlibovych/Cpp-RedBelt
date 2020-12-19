#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <string_view>
#include <future>
#include <deque>
#include <numeric>

#include "synchronized.h"

using namespace std;

class InvertedIndex {
public:
	InvertedIndex() = default;
	InvertedIndex(istream& document_input);

	void Add(const string& document);
	const vector<pair<size_t, size_t>> &Lookup(string_view word) const;

	const string& GetDocument(size_t id) const {
		return docs[id];
	}
	size_t getSize() {
		return docs.size();
	}

private:
	unordered_map<string_view, vector<pair<size_t, size_t>>> index;
	deque<string> docs;
};

class SearchServer {
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input);
	void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  	Synchronized<InvertedIndex> index;
	vector<future<void>> futures;
};
