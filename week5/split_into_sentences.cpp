#include "test_runner.h"

#include <vector>
#include <utility>

using namespace std;

template <typename Token>
using Sentence = vector<Token>;

template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    vector<Sentence<Token>> res;
    Sentence<Token> sen;
    bool sentence = 0;

    for (Token& token : tokens) {
        if (!token.IsEndSentencePunctuation()) {
            if (sentence) {
                sentence = 0;
                res.push_back(move(sen));
                sen.clear();
            }
        }
        else {
            sentence = 1;
        }
        sen.push_back(move(token));
    }
    if (!sen.empty()) {
        res.push_back(move(sen));
        sen.clear();
    }
    return res;
}


// struct TestToken {
//   string data;
//   bool is_end_sentence_punctuation = false;

//   bool IsEndSentencePunctuation() const {
//     return is_end_sentence_punctuation;
//   }
//   bool operator==(const TestToken& other) const {
//     return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
//   }
// };

// ostream& operator<<(ostream& stream, const TestToken& token) {
//   return stream << token.data;
// }

// // Тест содержит копирования объектов класса TestToken.
// // Для проверки отсутствия копирований в функции SplitIntoSentences
// // необходимо написать отдельный тест.
// void TestSplitting() {
//   ASSERT_EQUAL(
//     SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!"}})),
//     vector<Sentence<TestToken>>({
//         {{"Split"}, {"into"}, {"sentences"}, {"!"}}
//     })
//   );

//   ASSERT_EQUAL(
//     SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}})),
//     vector<Sentence<TestToken>>({
//         {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
//     })
//   );

//   ASSERT_EQUAL(
//     SplitIntoSentences(vector<TestToken>({{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true}})),
//     vector<Sentence<TestToken>>({
//         {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
//         {{"Without"}, {"copies"}, {".", true}},
//     })
//   );
// }

// int main() {
//   TestRunner tr;
//   RUN_TEST(tr, TestSplitting);
//   return 0;
// }
