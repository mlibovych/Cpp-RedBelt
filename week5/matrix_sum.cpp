#include "test_runner.h"
#include <vector>
#include <numeric>
#include <future>

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

template <typename ContainerOfVectors>
int64_t SumSingleThread(const ContainerOfVectors& matrix) {
    int64_t sum = 0;

    for (auto row : matrix) {
        sum += std::accumulate(row.begin(), row.end(), 0);
    }
    return sum;
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    int64_t sum = 0;
    vector<future<int64_t>> futures;

    for (auto page : Paginate(matrix, 2000)) {
        futures.push_back(async([page]{return SumSingleThread(page);}));
    }
    for (auto& f : futures) {
        sum += f.get();
    }
    return sum;
}

// void TestCalculateMatrixSum() {
//   const vector<vector<int>> matrix = {
//     {1, 2, 3, 4},
//     {5, 6, 7, 8},
//     {9, 10, 11, 12},
//     {13, 14, 15, 16}
//   };
//   ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
// }

// int main() {
//   TestRunner tr;
//   RUN_TEST(tr, TestCalculateMatrixSum);
// }
