#include "test_runner.h"
#include "http_request.h"
#include "stats.h"

#include <map>
#include <string_view>
using namespace std;

Stats ServeRequests(istream& input) {
  Stats result;
  for (string line; getline(input, line); ) {
    const HttpRequest req = ParseRequest(line);
    result.AddUri(req.uri);
    result.AddMethod(req.method);
  }
  return result;
}

void TestBasic() {
  const string input =
    R"(GET / HTTP/1.1
    POST /order HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    POST /product HTTP/1.1
    GET /order HTTP/1.1
    PUT /product HTTP/1.1
    GET /basket HTTP/1.1
    DELETE /product HTTP/1.1
    GET / HTTP/1.1
    GET / HTTP/1.1
    GET /help HTTP/1.1
    GET /upyachka HTTP/1.1
    GET /unexpected HTTP/1.1
    HEAD / HTTP/1.1)";

  const map<string_view, int> expected_method_count = {
    {"GET", 8},
    {"PUT", 1},
    {"POST", 4},
    {"DELETE", 1},
    {"UNKNOWN", 1},
  };
  const map<string_view, int> expected_url_count = {
    {"/", 4},
    {"/order", 2},
    {"/product", 5},
    {"/basket", 1},
    {"/help", 1},
    {"unknown", 2},
  };

  istringstream is(input);
  const Stats stats = ServeRequests(is);

  ASSERT_EQUAL(stats.GetMethodStats(), expected_method_count);
  ASSERT_EQUAL(stats.GetUriStats(), expected_url_count);
}

void TestAbsentParts() {
  // РњРµС‚РѕРґС‹ GetMethodStats Рё GetUriStats РґРѕР»Р¶РЅС‹ РІРѕР·РІСЂР°С‰Р°С‚СЊ СЃР»РѕРІР°СЂРё
  // СЃ РїРѕР»РЅС‹Рј РЅР°Р±РѕСЂРѕРј РєР»СЋС‡РµР№, РґР°Р¶Рµ РµСЃР»Рё РєР°РєРѕР№-С‚Рѕ РёР· РЅРёС… РЅРµ РІСЃС‚СЂРµС‡Р°Р»СЃСЏ

  const map<string_view, int> expected_method_count = {
    {"GET", 0},
    {"PUT", 0},
    {"POST", 0},
    {"DELETE", 0},
    {"UNKNOWN", 0},
  };
  const map<string_view, int> expected_url_count = {
    {"/", 0},
    {"/order", 0},
    {"/product", 0},
    {"/basket", 0},
    {"/help", 0},
    {"unknown", 0},
  };
  const Stats default_constructed;
  for (const auto &[key, value] : default_constructed.GetMethodStats()) {
    std::cout << key << std::endl;
  }
  ASSERT_EQUAL(default_constructed.GetMethodStats(), expected_method_count);
  ASSERT_EQUAL(default_constructed.GetUriStats(), expected_url_count);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
  RUN_TEST(tr, TestAbsentParts);
  // for (string line; getline(cin, line); ) {
  //   const HttpRequest req = ParseRequest(line);
	//   std::cout << req.method << std::endl << req.uri << std::endl << req.protocol << std::endl;
  // }
}