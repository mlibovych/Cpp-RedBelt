#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
	istringstream words_input(line);
	return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  	UpdateDocumentBase(document_input);
}

void UpdateDocumentBaseSingle(istream& document_input, Synchronized<InvertedIndex>& index_handle) {
	InvertedIndex new_index;

	for (string current_document; getline(document_input, current_document); ) {
		new_index.Add(move(current_document));
	}

	swap(index_handle.GetAccess().ref_to_value, new_index);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	futures.push_back(async(UpdateDocumentBaseSingle, ref(document_input), ref(index)));
}

void AddQueriesStreamSingle(
  	istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& index_handle) {
	vector<pair<size_t, size_t>> search_results;
	auto access = index_handle.GetAccess();

	search_results.reserve(access.ref_to_value.getSize());
  	for (string current_query; getline(query_input, current_query); ) {
    	const auto words = SplitIntoWords(current_query);

		search_results.assign(access.ref_to_value.getSize(), { 0, 0 });;
		for (const auto& word : words) {
			for (const size_t docid : access.ref_to_value.Lookup(word)) {
				search_results[docid].first = docid;
				search_results[docid].second++;
			}
		}
    	partial_sort(
			search_results.begin(),
			search_results.begin() + min<size_t>(5, search_results.size()),
			search_results.end(),
			[](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
				int64_t lhs_docid = lhs.first;
				auto lhs_hit_count = lhs.second;
				int64_t rhs_docid = rhs.first;
				auto rhs_hit_count = rhs.second;
				return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
			}
		);

    	search_results_output << current_query << ':';
		for (auto [docid, hitcount] : Head(search_results, 5)) {
			if (hitcount) {
			search_results_output << " {"
				<< "docid: " << docid << ", "
				<< "hitcount: " << hitcount << '}';
			}
		}
    	search_results_output << endl;
  	}
}


void SearchServer::AddQueriesStream(
  	istream& query_input, ostream& search_results_output) {
	futures.push_back(async(AddQueriesStreamSingle, ref(query_input), ref(search_results_output), ref(index)));
}

void InvertedIndex::Add(const string& document) {
  	docs.push_back(document);

  	const size_t docid = docs.size() - 1;
  	for (const auto& word : SplitIntoWords(document)) {
    	index[word].push_back(docid);
  	}
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
  	if (auto it = index.find(word); it != index.end()) {
    	return it->second;
  	} else {
    	return {};
  	}
}
