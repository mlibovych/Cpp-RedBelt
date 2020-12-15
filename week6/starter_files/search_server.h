#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <string_view>

// #include "profile.h"
// #include "duration.h"
using namespace std;

class InvertedIndex {
public:
	void Add(const string& document);
	list<size_t> Lookup(const string& word) const;

	const string& GetDocument(size_t id) const {
		return docs[id];
	}
	size_t getSize() {
		return docs.size();
	}

private:
	map<string, list<size_t>> index;
	vector<string> docs;
};

class SearchServer {
public:
	SearchServer() = default;
	explicit SearchServer(istream& document_input);
	void UpdateDocumentBase(istream& document_input);
	void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  	InvertedIndex index;
};
