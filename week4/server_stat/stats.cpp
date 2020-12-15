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

HttpRequest ParseRequest(string_view line)  {
    HttpRequest result;
    std::vector<string_view> parts;

    if (std::count(line.begin(), line.end(), '/') != 2) {
        return result;
    }
    parts.reserve(3);
    for (int i = 0; i < 3; ++i) {
        size_t space;
        string_view part;

        line.remove_prefix(line.find_first_not_of(' '));
        space = line.find(' ');
        part = line.substr(0, space);
        parts.push_back(part);
        line.remove_prefix(space + 1);
    }
    result.method = parts[0];
    result.uri = parts[1];
    result.protocol = parts[2];
    return result;
}