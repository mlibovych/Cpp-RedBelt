#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 3) {
          return;
    }
	// vector<typename RandomIt::value_type> elements(
    //   make_move_iterator(range_begin), make_move_iterator(range_end));

	vector<typename RandomIt::value_type> elements;
	elements.reserve(range_end - range_begin);
  	for (auto it = range_begin; it != range_end; ++it) {
    	elements.push_back(move(*it));
	}
	auto first = elements.begin() + elements.size() / 3;
	auto second = first + elements.size() / 3;
	MergeSort(elements.begin(), first);
	MergeSort(first, second);
	MergeSort(second, elements.end());
	vector<typename RandomIt::value_type> tmp;
	merge(make_move_iterator(elements.begin()), make_move_iterator(first), make_move_iterator(first), make_move_iterator(second), back_inserter(tmp));
	merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()), make_move_iterator(second), make_move_iterator(elements.end()), range_begin);
}

// void TestIntVector() {
// 	vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
// 	MergeSort(begin(numbers), end(numbers));
// 	ASSERT(is_sorted(begin(numbers), end(numbers)));
// 	}

// int main() {
// 	TestRunner tr;
// 	RUN_TEST(tr, TestIntVector);
// 	return 0;
// }
