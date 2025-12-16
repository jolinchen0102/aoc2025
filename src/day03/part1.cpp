#include "logging.h"
#include "utils.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string_view>
#include <utility>
#define LOG false

static ll get_max_two(std::string_view line) {
    assert(!line.empty());
    std::pair<char, char> max_two({0, 0});
    size_t len = line.size();
    for (size_t i = 0; i < len; i++) {
        char c = line[i];
        if (c > max_two.first && i < len - 1) {
            max_two.first = c;
            max_two.second = 0;
        } else if (c >= max_two.second) {
            max_two.second = c;
        }
    }
    assert_msg(max_two.first >= 48, "fail to populate max 1: %d",
               max_two.first);
    assert_msg(max_two.second >= 48, "fail to populate max 2: %d",
               max_two.second);
    ll num = static_cast<int>(max_two.first - char2int_shift) * 10 +
             static_cast<int>(max_two.second - char2int_shift);
    clogf("Got max %d%d from %s, convert to %lld", max_two.first,
          max_two.second, line.data(), num);
    return num;
}

static ll solve(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file(filename.data());
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
    }
    std::string line;
    ll res = 0;
    while (getline(input_file, line)) {
        if (line.empty())
            break;
        res += get_max_two(line);
    }
    return res;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        println(std::cerr, "Usage: {} <input_file>", argv[0]);
    }
    std::string_view filename = argv[1];
    println("Using file {}", filename);
    ll res = solve(filename);
    std::println("res={}", res);
    return 0;
}