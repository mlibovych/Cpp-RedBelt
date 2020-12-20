#include "stats.h"

#include <map>
#include <vector>
#include <iostream> 
#include <string_view>
#include <algorithm>
using namespace std;

void Stats::AddMethod(string_view method) {
    if (method_count.count(method)) {
        method_count[method]++;
    }
    else {
        method_count["UNKNOWN"]++;
    }
}
void Stats::AddUri(string_view uri) {
    if (uri_count.count(uri)) {
        uri_count[uri]++;
    }
    else {
        uri_count["unknown"]++;
    }
}
const map<string_view, int>& Stats::GetMethodStats() const {
    return method_count;
}
const map<string_view, int>& Stats::GetUriStats() const {
    return uri_count;
}

void LeftStrip(string_view& sv) {
  while (!sv.empty() && isspace(sv[0])) {
    sv.remove_prefix(1);
  }
}

string_view ReadToken(string_view& sv) {
  LeftStrip(sv);

  auto pos = sv.find(' ');
  auto result = sv.substr(0, pos);
  sv.remove_prefix(pos != sv.npos ? pos : sv.size());
  return result;
}

HttpRequest ParseRequest(string_view line) {
  auto method = ReadToken(line);
  auto uri = ReadToken(line);
  return {method, uri, ReadToken(line)};
}