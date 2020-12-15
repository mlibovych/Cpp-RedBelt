#include <string>
#include <list>
#include "test_runner.h"
using namespace std;

class Editor {
    using Position = list<char>::iterator;
    Position pos;
    list<char> data;
    vector<char> buff;
public:
    Editor() {
        pos = data.begin();
    }
    void Left() {
        if (pos != data.begin()) {
            --pos;
        }
    }
    void Right() {
        if (pos != data.end() || pos == data.begin()) {
            ++pos;
        }
    }
    void Insert(char token) {
        data.insert(pos, token);
        // Right();
    }
    void Cut(size_t tokens = 1) {
        buff.clear();
        while (tokens != 0 && pos != data.end()) {
            buff.push_back(*pos);
            pos = data.erase(pos);
            --tokens;
        }
    }
    void Copy(size_t tokens = 1) {
        auto b_pos = pos;

        buff.clear();
        while (tokens != 0) {
            buff.push_back(*b_pos);
            b_pos++;
            --tokens;
        }
    }
    void Paste() {
        for (const auto& c : buff) {
            Insert(c);
        }
    }
    string GetText() const {
        return {data.begin(), data.end()};
    }
};

void TypeText(Editor& editor, const string& text) {
  for(char c : text) {
    editor.Insert(c);
  }
}

void TestEditing() {
  {
    Editor editor;

    const size_t text_len = 12;
    const size_t first_part_len = 7;
    TypeText(editor, "hello, world");
    cout << editor.GetText() << "|||" << endl;

    for(size_t i = 0; i < text_len; ++i) {
      editor.Left();
    }
    cout << editor.GetText() << "|||" << endl;
    editor.Cut(first_part_len);
    cout << editor.GetText() << "|||" << endl;

    for(size_t i = 0; i < text_len - first_part_len; ++i) {
      editor.Right();
    }
    TypeText(editor, ", ");
    cout << editor.GetText() << "|||" << endl;
    editor.Paste();
    cout << editor.GetText() << "|||" << endl;
    editor.Left();
    editor.Left();
    cout << editor.GetText() << "|||" << endl;
    editor.Cut(3);
    cout << editor.GetText() << "|||" << endl;
    
    ASSERT_EQUAL(editor.GetText(), "world, hello");
  }

  {
    Editor editor;
    
    TypeText(editor, "misprnit");
    editor.Left();
    editor.Left();
    editor.Left();
    editor.Cut(1);
    editor.Right();
    editor.Paste();
    
    ASSERT_EQUAL(editor.GetText(), "misprint");
  }
}

void TestReverse() {
  Editor editor;

  const string text = "2esreveR";
  for(char c : text) {
    editor.Insert(c);
    cout << editor.GetText() << endl;
    editor.Left();
  }

  ASSERT_EQUAL(editor.GetText(), "Reverse2");
}

void TestNoText() {
  Editor editor;
  ASSERT_EQUAL(editor.GetText(), "");
  
  editor.Left();
  editor.Left();
  editor.Right();
  editor.Right();
  editor.Copy(0);
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
  Editor editor;

  editor.Paste();
  TypeText(editor, "example");
  editor.Left();
  editor.Left();
  editor.Paste();
  editor.Right();
  editor.Paste();
  editor.Copy(0);
  editor.Paste();
  editor.Left();
  editor.Cut(0);
  editor.Paste();
  
  ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestEditing);
  RUN_TEST(tr, TestReverse);
  RUN_TEST(tr, TestNoText);
  RUN_TEST(tr, TestEmptyBuffer);
  return 0;
}