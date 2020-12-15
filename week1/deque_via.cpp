#include <string>
#include <vector>
#include <iostream>

using namespace std;

// template <typename T>
// T GetAbs(vector<T> first, vector<T> second) {
//     if (first.size()) {
//         return *--first.end();
//     }
//     else {
//         return *second.begin();
//     }
// }

template <typename T>
class Deque {
    vector<T> first, second;

    T& GetAbs(vector<T>& first_, vector<T>& second_) {
        if (first_.size()) {
            return *--first_.end();
        }
        return *second_.begin();
    }

    T& GetIdx(int index) {
        if (index >= first.size() + second.size()) {
            throw out_of_range("At" + to_string(index));
        }
        if (index < first.size()) {
            return first[first.size() - index - 1];
        }
        return second[index - first.size()];
    }
public:
    Deque() {};

    const bool Empty() const {
        if (!first.size() && !second.size()) {
            return true;
        }
        return false;
    };

    T& operator[](size_t index) {
        return GetIdx(index);
    };
    
    const T& operator[](size_t index) const {
        return GetIdx(index);
    };

    T& At(size_t index) {
        return GetIdx(index);
    };

    const T& At(size_t index) const {
        return GetIdx(index);
    };

    T& Front() {
        // return GetAbs(first, second);
        if (first.size()) {
            return *--first.end();
        }
        return *second.begin();
    };

    const T& Front() const {
        // return GetAbs(first, second);
        if (first.size()) {
            return *--first.end();
        }
        return *second.begin();
    };

    T& Back() {
        // return GetAbs(second, first);
        // if (second.size()) {
        //     return *--second.end();
        // }
        // return *first.begin();
        return second.empty() ? first.front() : second.back();
    };

    const T& Back() const {
        // return GetAbs(second, first);
        // if (second.size()) {
        //     return *--second.end();
        // }
        // return *first.begin();
        return second.empty() ? first.front() : second.back();
    };

    void PushFront(T el) {
        first.push_back(el);
    };

    void PushBack(T el) {
        second.push_back(el);
    };

    const size_t Size() const {
        return first.size() + second.size();
    }
};

// int main() {
//     Deque<int> Dec;

//     // Dec.PushFront(2);
//     // Dec.PushFront(1);
//     Dec.PushBack(3);
//     Dec.PushBack(4);
//     /* 1 2 3 */
//     // Dec.Front() = 55;
//     cout << Dec.Back() << endl; // 4
//     cout << Dec.At(1) << endl; // 2

//     return 0;
// }

