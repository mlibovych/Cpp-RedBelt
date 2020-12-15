#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <string_view>
#include <future>
#include <functional>
#include <algorithm>
using namespace std;

template <typename Iterator>
class IteratorRange {
    const Iterator first, last;
public:
    IteratorRange(Iterator f, Iterator l) : first(f), last(l) {}

    size_t size() const {
        return last - first;
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

};

template <typename Iterator>
class Paginator {
    vector<IteratorRange<Iterator>> pages;
public:
    Paginator(Iterator begin, Iterator end, size_t page_size) {
        for (auto it = begin; it < end; it += page_size) {
            pages.push_back({it, min(it + page_size, end)});
        }
    }

    size_t size() const {
        return pages.size();
    }

    auto begin() const {
        return pages.begin();
    }

    auto end() const {
        return pages.end();
    }
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
    return Paginator(c.begin(), c.end(), page_size);
}

struct Stats {
  	map<string, int> word_frequences;

	void operator += (const Stats& other) {
	  	for (const auto& [key, value]  : other.word_frequences) {
		  	word_frequences[key] += value;
	  	}
  	}
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
	Stats res;
	stringstream stream(line);

	stream >> ws;
	for (string word; getline(stream, word, ' '); ) {
		if (key_words.count(word)) {
			res.word_frequences[word]++;
		}
    }
	return res;
}

Stats ExploreKeyWordsSingleThread(
  const set<string>& key_words, istream& input
) {
  Stats result;
  for (string line; getline(input, line); ) {
	//   std::cout << line << std::endl;
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWordsSingleThreadMy(
    const set<string>& key_words, const vector<string>& vec) {
  Stats result;

  for (auto it = vec.begin(); it != vec.end(); ++it) {
    //   std::cout << *it << "aaa" << std::endl;
    result += ExploreLine(key_words, *it);
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	Stats res;
	vector<future<Stats>> futures;
	vector<string> strings;
	size_t page_size = 2000;

	input >> ws;
	
	for (string line; getline(input, line); ) {
		strings.push_back(line);
  	}
	for(auto& page : Paginate(strings, page_size)) {
        vector<string> vec = {page.begin(), page.end()};
        futures.push_back(async(ExploreKeyWordsSingleThreadMy, ref(key_words), std::move(vec)));
	}
	for (auto& f : futures) {
		res += f.get();
	}
	return res;
}

// string generateStr() {
// 	string str;

// 	for (int i = 0; i < 80000; ++i) {
// 		str += " yoda";
// 	}
// 	return str;
// }

// void TestBasic() {
//   const set<string> key_words = {"yangle", "rocks", "sucks", "all", "yoda"};

//   stringstream ss;
//   ss << "this new yangle service really rocks\n";
//   ss << "It sucks when yangle isn't available\n";
//   ss << "10 reasons why yangle is the best IT company\n";
//   ss << "yangle rocks others suck\n";
//   ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

//   const auto stats = ExploreKeyWords(key_words, ss);
//   const map<string, int> expected = {
//     {"yangle", 6},
//     {"rocks", 2},
//     {"sucks", 1}
//   };
//   ASSERT_EQUAL(stats.word_frequences, expected);

// 	stringstream ss1;
// 	ss1 << generateStr();
// 	{
// 		LOG_DURATION("single")
// 		const auto stats1 = ExploreKeyWordsSingleThread(key_words, ss1);
// 		string s = "yoda";
// 		std::cout << stats1.word_frequences.at("yoda") << std::endl;
// 	}
// 	stringstream ss2;
// 	ss2 << generateStr();
// 	{
// 		LOG_DURATION("multi")
// 		const auto stats2 = ExploreKeyWords(key_words, ss2);
// 		std::cout << stats2.word_frequences.at("yoda") << std::endl;
// 	}
// }

// int main() {
//   TestRunner tr;
//   RUN_TEST(tr, TestBasic);
// }
