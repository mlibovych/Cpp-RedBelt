#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <list>
// #include <utility>
#include <vector>

using namespace std;

template <typename T>
struct Item {
	int priority = 0;
	int id = 0;
	T data;
};

template <typename T>
bool operator<(const Item<T>& lhs, const Item<T>& rhs) {
    if (lhs.priority != rhs.priority) {
        return lhs.priority < rhs.priority;;
    }
    return lhs.id < rhs.id;
}

template <typename T>
class PriorityCollection {
public:
    using Id = typename set<Item<T>>::iterator;

    Id Add(T object) {
        return elements.insert({0, static_cast<int>(elements.size()), move(object)}).first;
    }

    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
        	IdOutputIt ids_begin) {
		std::move(range_begin, range_end, ids_begin);
	}

    bool IsValid(Id id) const {
		if (std::find(elements.begin(), elements.end(), *id) != elements.end()) {
			return 1;
		}
		return 0;
	}


    const T& Get(Id id) const {
		return (*id).data;
	}

    void Promote(Id id) {
		Item<T> result = move(elements.extract(id).value());


		++result.priority;
		elements.insert(move(result));
    }

    pair<const T&, int> GetMax() const {
		return std::make_pair((*prev(elements.end())).data, (*prev(elements.end())).priority);
    }

    pair<T, int> PopMax() {
		Id id = prev(elements.end());
		Item<T> element = move(elements.extract(id).value());
		pair<T, int> result = std::make_pair(std::move(element.data), element.priority);

		return result;
    }

	void printCol() {
		for (const auto& item : elements) {
			std::cout << item.data << item.priority << std::endl;
		}
		std::cout << "----" << std::endl;
	}

private:
    std::set<Item<T>> elements;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }

  strings.Promote(yellow_id);
//   std::cout << strings.Get(red_id) << std::endl;
//   strings.printCol();
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
