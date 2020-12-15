#include "test_runner.h"

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Logger {
public:
    explicit Logger(ostream& output_stream) : os(output_stream) {
    }

    void SetLogLine(bool value) { log_line = value; }
    void SetLogFile(bool value) { log_file = value; }

    void SetLine(int line_) { line = line_; }
    void SetFile(const string&file_) { file = file_; }

    void Log(const string& message) {
        if (log_line && log_file) {
            os << file << ':' << line << ' ';
        }
        else if (log_file) {
            os << file << ' ';
        }
        else if (log_line) {
            os << "Line " << line << ' ';
        }
        os << message << endl;
    }

private:
  ostream& os;
  bool log_line = false;
  bool log_file = false;
  string file;
  int line;
};


#define LOG(logger, message) {  \
    logger.SetFile(__FILE__);           \
    logger.SetLine(__LINE__);           \
    logger.Log(message);        \
}

// void TestLog() {
// #line 1 "logger.cpp"

//   ostringstream logs;
//   Logger l(logs);
//   LOG(l, "hello");

//   l.SetLogFile(true);
//   LOG(l, "hello");

//   l.SetLogLine(true);
//   LOG(l, "hello");

//   l.SetLogFile(false);
//   LOG(l, "hello");

//   string expected = "hello\n";
//   expected += "logger.cpp hello\n";
//   expected += "logger.cpp:10 hello\n";
//   expected += "Line 13 hello\n";
//   ASSERT_EQUAL(logs.str(), expected);
// }

// int main() {
//   TestRunner tr;
//   RUN_TEST(tr, TestLog);
// }