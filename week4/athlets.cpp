#include <iostream>
#include <list>
#include <vector>
#include <set>

using namespace std;

int main() {
    const int MAX_ATHLETES = 100'000;
    using Position = list<int>::iterator;

    int n;
    list<int> athlets;
    vector<Position> position(MAX_ATHLETES + 1, athlets.end());

    for (cin >> n; n > 0; --n) {
        int first, second;

        cin >> first >> second;
        position[first] = athlets.insert(position[second],first);
    }
    for (int x : athlets) {
        cout << x << '\n';
    }
}