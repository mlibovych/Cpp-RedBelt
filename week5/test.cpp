#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <string_view>
#include <future>
#include <functional>
#include <algorithm>

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

template <typename ContainerOfVectors >
void GenerateSingleThread (
ContainerOfVectors & result,
size_t first_row ,
size_t column_size
) {
for (auto& row : result) {
row.reserve( column_size );
for (size_t column = 0; column < column_size ; ++ column) {
row. push_back ( first_row ^ column);
}
++ first_row ;
}
}

vector <vector <int>> GenerateSingleThread (size_t size) {
vector <vector <int>> result(size);
GenerateSingleThread (result , 0, size);
return result;
}



vector <vector <int>> GenerateMultiThread (
size_t size , size_t page_size
) {
vector <vector <int>> result(size);
vector <future <void >> futures;
size_t first_row = 0;
for (auto page : Paginate(result , page_size )) {
futures. push_back (
async ([page , first_row , size] {
GenerateSingleThread (page , first_row , size);
})
);
first_row += page_size ;
}
return result;
}

int main () {
LOG_DURATION ("Total");
const size_t matrix_size = 7000;
vector <vector <int>> matrix;
{
LOG_DURATION ("Single thread generation");
matrix = GenerateSingleThread ( matrix_size );
}
{
LOG_DURATION ("Multi thread generation");
matrix = GenerateMultiThread (matrix_size , 2000);
}
}