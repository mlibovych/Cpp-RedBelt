#include "test_runner.h"

using namespace std;

template <typename T>
class Table {
    vector<vector<T>> data;
public:
    Table(size_t first, size_t second) {
        Resize(first, second);
    };

    void Resize(size_t first, size_t second) {
        data.resize(first);
        for (auto& it : data) {
            it.resize(second);
        }
    }

    pair<size_t, size_t> Size() const {
        if (!data.size() || !data[0].size()) {
            return make_pair(0, 0);
        }
        else {
            return make_pair(data.size(), data[0].size());
        }
    }

    const vector<T>& operator[](size_t id) const {
        return data[id];
    };

    vector<T>& operator[](size_t id) {
        return data[id];
    };
};

// void TestTable() {
//     Table<int> t(1, 1);
//     ASSERT_EQUAL(t.Size().first, 1u);
//     ASSERT_EQUAL(t.Size().second, 1u);
//     t[0][0] = 42;
//     ASSERT_EQUAL(t[0][0], 42);
//     t.Resize(3, 4);
//     ASSERT_EQUAL(t.Size().first, 3u);
//     ASSERT_EQUAL(t.Size().second, 4u);
// }

// int main() {
//     TestRunner tr;
//     RUN_TEST(tr, TestTable);
//     return 0;
// }