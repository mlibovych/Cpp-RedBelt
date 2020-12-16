// #include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
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
bool operator==(const Item<T>& lhs, const Item<T>& rhs) {
   if(lhs.priority != rhs.priority || lhs.id != rhs.id || lhs.data != rhs.data) {
     return 0;
   }
    return 1;
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
		while (range_begin != range_end) {
      		*ids_begin++ = Add(move(*range_begin++));
		}
	}

    bool IsValid(Id id) const {
		if(find(elements.begin(), elements.end(), *id) != elements.end()) {
			return true;
		}
		return false;
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
		const auto item = prev(elements.end());
		return {item->data, item->priority};
    }

    pair<T, int> PopMax() {
		Id id = prev(elements.end());
		Item<T> element = move(elements.extract(id).value());
		pair<T, int> result = std::make_pair(std::move(element.data), element.priority);

		return result;
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

// void TestNoCopy() {
//   PriorityCollection<StringNonCopyable> strings;
//   const auto white_id = strings.Add("white");
//   const auto yellow_id = strings.Add("yellow");
//   const auto red_id = strings.Add("red");

//   auto a = white_id;

//   strings.Promote(yellow_id);
//   for (int i = 0; i < 2; ++i) {
//     strings.Promote(red_id);
//   }
//   if(strings.IsValid(red_id)) {
//     cout << "aaa" << endl;
//   }

//   strings.Promote(yellow_id);
//   strings.GetMax();
//   {
//     const auto item = strings.PopMax();
//     ASSERT_EQUAL(item.first, "red");
//     ASSERT_EQUAL(item.second, 2);
//   }
//   if(strings.IsValid(red_id)) {
//     cout << "aaa" << endl;
//   }
//   {
//     const auto item = strings.PopMax();
//     ASSERT_EQUAL(item.first, "yellow");
//     ASSERT_EQUAL(item.second, 2);
//   }
//   {
//     const auto item = strings.PopMax();
//     ASSERT_EQUAL(item.first, "white");
//     ASSERT_EQUAL(item.second, 0);
//   }
// }

// int main() {
//   TestRunner tr;
//   RUN_TEST(tr, TestNoCopy);
//   return 0;
// }
