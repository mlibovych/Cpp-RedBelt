#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <string_view>
#include <iostream>

// void LeftStrip(string_view& sv) {
//   while (!sv.empty() && isspace(sv[0])) {
//     sv.remove_prefix(1);
//   }
// }

// string_view ReadToken(string_view& sv) {
//   LeftStrip(sv);

//   auto pos = sv.find(' ');
//   auto result = sv.substr(0, pos);
//   sv.remove_prefix(pos != sv.npos ? pos : sv.size());
//   return result;
// }

// vector<string_view> SplitIntoWords(string_view line) {
//   vector<string_view> res;

//   for (string_view word = ReadToken(line); !word.empty(); word = ReadToken(line)) {
//     res.push_back(word);
//   }

//   return res;
// }

vector<string> SplitIntoWords(const string& line) {
  istringstream words_input(line);
  return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) : index(InvertedIndex(document_input)) {
  	
}

void UpdateDocumentBaseSingle(istream& document_input, Synchronized<InvertedIndex>& index_handle) {
	InvertedIndex new_index(document_input);

	swap(index_handle.GetAccess().ref_to_value, new_index);
}



void SearchServer::UpdateDocumentBase(istream& document_input) {
	// futures.push_back(async(UpdateDocumentBaseSingle, ref(document_input), ref(index)));

	InvertedIndex new_index(document_input);

	swap(index, new_index);
}

void AddQueriesStreamSingle(
  	istream& query_input, ostream& search_results_output, Synchronized<InvertedIndex>& index_handle) {
	vector<size_t> docid_count;
	vector<int64_t> docids;

	// docid_count.reserve(access.ref_to_value.getSize());
	// docids.reserve(access.ref_to_value.getSize());

  	for (string current_query; getline(query_input, current_query); ) {
    	const auto words = SplitIntoWords(current_query);

		auto access = index_handle.GetAccess();

		docid_count.assign(access.ref_to_value.getSize(), 0);
		docids.assign(access.ref_to_value.getSize(), 0);
		
		for (const auto& word : words) {
			for (const auto &docid : access.ref_to_value.Lookup(word)) {
				docid_count[docid.first] += docid.second;
			}
		}

		iota(docids.begin(), docids.end(), 0);

    	partial_sort(
			begin(docids),
			Head(docids, 5).end(),
			end(docids),
			[&docid_count](int64_t lhs, int64_t rhs) {
				return pair(docid_count[lhs], -lhs) > pair(docid_count[rhs], -rhs);
			}
		);

    	search_results_output << current_query << ':';
		for (size_t docid : Head(docids, 5)) {
			if (docid_count[docid]) {
			search_results_output << " {"
				<< "docid: " << docid << ", "
				<< "hitcount: " << docid_count[docid] << '}';
			}
			else {
				break;
			}
		}
    	search_results_output << endl;
  	}
}

void SearchServer::AddQueriesStream(
  	istream& query_input, ostream& search_results_output) {
	// futures.push_back(async(AddQueriesStreamSingle, ref(query_input), ref(search_results_output), ref(index)));
	vector<size_t> docid_count;
	vector<int64_t> docids;

	docid_count.reserve(index.getSize());
	docids.reserve(index.getSize());

  	for (string current_query; getline(query_input, current_query); ) {
    	const auto words = SplitIntoWords(current_query);

		{
			docid_count.assign(index.getSize(), 0);
			docids.assign(index.getSize(), 0);
			
			for (const auto& word : words) {
				for (const auto &docid : index.Lookup(word)) {
					docid_count[docid.first] += docid.second;
				}
			}
		}

		iota(docids.begin(), docids.end(), 0);

    	partial_sort(
			begin(docids),
			Head(docids, 5).end(),
			end(docids),
			[&docid_count](int64_t lhs, int64_t rhs) {
				return pair(docid_count[lhs], -lhs) > pair(docid_count[rhs], -rhs);
			}
		);

    	search_results_output << current_query << ':';
		for (size_t docid : Head(docids, 5)) {
			if (docid_count[docid]) {
			search_results_output << " {"
				<< "docid: " << docid << ", "
				<< "hitcount: " << docid_count[docid] << '}';
			}
			else {
				break;
			}
		}
    	search_results_output << endl;
  	}
}

void InvertedIndex::Add(const string& document) {
  	docs.push_back(move(document));

  	const size_t docid = docs.size() - 1;
  	for (const auto& word : SplitIntoWords(docs.back())) {
		vector<pair<size_t, size_t>> &docs = index[word];
		if (!docs.empty() && docs.back().first == docid) {
			++docs.back().second;
		}
		else {
			docs.push_back({docid, 1});
		}
  	}
}

vector<pair<size_t, size_t>> InvertedIndex::Lookup(const string &word) const {
  	if (auto it = index.find(word); it != index.end()) {
    	return it->second;
  	} else {
    	return {};
  	}
}

InvertedIndex::InvertedIndex(istream& document_input) {
	docs.reserve(50000);

	for (string current_document; getline(document_input, current_document); ) {
		docs.push_back(move(current_document));
		size_t docid = docs.size() - 1;
		for (const auto word : SplitIntoWords(docs.back())) {
			auto &documents = index[word];
			if (!documents.empty() && documents.back().first == docid) {
				++documents.back().second;
			}
			else {
				documents.push_back({docid, 1});
			}
		}
	}
}
