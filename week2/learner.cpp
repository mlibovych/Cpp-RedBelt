#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
// #include "profile.h"
#include <set>

using namespace std;

class Learner {
 private:
  set<string> sorted;

 public:
  int Learn(const vector<string>& words) {
    int newWords = 0;
    for (const auto& word : words) {
        if (!sorted.count(word)) {
            ++newWords;
            sorted.insert(word);
        }
    }
    return newWords;
  }

  vector<string> KnownWords() {
    vector<string> dict;

    for (const string& word : sorted) {
        dict.push_back(word);
    }
    return dict;
  }
};

// int main() {
//   Learner learner;
//   string line;
//   while (getline(cin, line)) {
//     vector<string> words;
//     stringstream ss(line);
//     string word;
//     while (ss >> word) {
//       words.push_back(word);
//     }

//     {
//         LOG_DURATION("input")
//         cout << learner.Learn(words) << "\n";
//     }

//   }
//   cout << "=== known words ===\n";

//   {
//     LOG_DURATION("output")
//     for (auto word : learner.KnownWords()) {
//         cout << word << "\n";
//     }
//   }
// }