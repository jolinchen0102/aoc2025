#include "logging.h"
#include "utils.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string_view>
#include <utility>
#include <vector>
#define LOG false

#define NUM_CODES 12

static void zero_arr_from_idx(char *arr, size_t arr_len, size_t idx) {
    if (idx == arr_len)
        return;
    assert_msg(idx < arr_len, "invalid idx=%zu >= arr_len=%zu", idx, arr_len);
    for (size_t i = idx; i < arr_len; i++) {
        arr[i] = 0;
    }
}

static ll chars_to_ll(const char *arr, size_t arr_len) {
    ll res = 0;
    for (size_t i = 0; i < arr_len; i++) {
        char c = arr[i];
        assert_msg(c > 48, "arr[%zu] not populated with value %hhd", i, c);
        res +=
            static_cast<int>(c - char2int_shift) * pow(10, NUM_CODES - i - 1);
    }
    clogf("got code %lld", res);
    return res;
}

static ll get_max_twelves(std::string_view line) {
    assert(!line.empty());
    size_t len = line.size();
    if (len <= NUM_CODES) {
        return std::stoll(line.data());
    }
    char codes[NUM_CODES]{0};
    for (size_t i = 0; i < len; i++) {
        char c = line[i];
        for (size_t ci = 0; ci < NUM_CODES; ci++) {
            size_t len_to_replace = NUM_CODES - ci - 1;
            if (c > codes[ci] && i < len - len_to_replace) {
                codes[ci] = c;
                // clear the remaining and break
                zero_arr_from_idx(codes, NUM_CODES, ci + 1);
                break;
            }
        }
    }
    ll num = chars_to_ll(codes, NUM_CODES);
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
        res += get_max_twelves(line);
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