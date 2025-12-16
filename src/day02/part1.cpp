#include "logging.h"
#include "utils.h"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>

using std::println;

#define RESTORE_NUM_FROM_L(l, factor) (((l) * (factor)) + (l))

size_t count_invalid(std::string_view first, std::string_view last) {
    assert(first.size() > 0);
    assert_msg(last.size() > 0, "%zu", last.size());
    /*
    1. xxx - xxx   -> 0
    2. xxx - xxxx+ -> ?
    3. xx - xx   -> ?
    4. xx - xx+  -> ?
    */
    size_t len_first = first.length();
    size_t len_last = last.length();
    if (len_last < len_first)
        return 0;
    if ((len_first == len_last) && len_first % 2 != 0)
        return 0;

    size_t invalid_sum = 0;
    long num_first = std::stol(first.data());
    long num_last = std::stol(last.data());
    println("before: {}({})-{}({})", num_first, len_first, num_last, len_last);
    if (len_first % 2 != 0) {
        /* aaa - bbbb+, convert aaa to nearest even num */
        num_first = std::pow(10, len_first);
        len_first++;
    }
    if (len_last % 2 != 0) {
        /* _ - bbb, convert bbb largest even */
        num_last = std::pow(10, len_last - 1) - 1;
        len_last--;
    }
    println("after {}({})-{}({})", num_first, len_first, num_last, len_last);
    assert((len_first % 2 == 0) && (len_last % 2 == 0));
    size_t half_digits_first = len_first / 2;
    long factor_first = std::pow(10, half_digits_first);
    long first_l = num_first / factor_first;
    long first_r = num_first % factor_first;

    size_t half_digits_last = len_last / 2;
    long factor_last = std::pow(10, half_digits_last);
    long last_l = num_last / factor_last;
    long last_r = num_last % factor_last;
    if (first_l == last_l) {
        // early return
        if (first_l >= first_r && first_l <= last_r) {
            invalid_sum += RESTORE_NUM_FROM_L(first_l, factor_first);
            clogf("+%ld", RESTORE_NUM_FROM_L(first_l, factor_first));
        }
        return invalid_sum;
    }
    /*
    1. 20 - 21
    2, 20 - 23
    3. 20 - 90
    4. 20 - 8000
    */
    assert_msg(first_l <= last_l, "first_l=%lu, last_l=%lu", first_l, last_l);
    long tmp_l = first_l;
    while (tmp_l <= last_l) {
        if (tmp_l == first_l) {
            if (first_l >= first_r) {
                invalid_sum += RESTORE_NUM_FROM_L(first_l, factor_first);
                clogf("+%ld", RESTORE_NUM_FROM_L(first_l, factor_first));
            }
        } else if (tmp_l == last_l) {
            if (last_l <= last_r) {
                invalid_sum += RESTORE_NUM_FROM_L(last_l, factor_last);
                clogf("+%ld", RESTORE_NUM_FROM_L(last_l, factor_last));
            }
        } else {
            size_t num_digits = get_num_digits(tmp_l);
            size_t factor = std::pow(10, num_digits);
            invalid_sum += RESTORE_NUM_FROM_L(tmp_l, factor);
            println("num_digits={}, +{}", num_digits,
                    RESTORE_NUM_FROM_L(tmp_l, factor));
        }
        tmp_l++;
    }
    println("invalid sum = {}", invalid_sum);
    return invalid_sum;
}

size_t solve(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file(filename.data());
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
    }
    std::string range; // e.g. 11-22
    long long res = 0;
    while (getline(input_file, range, ',')) {
        if (range.empty())
            break;
        size_t pos = range.find('-');
        assert(pos != std::string_view::npos);
        std::string_view range_view = std::string_view(range);
        std::string_view first = range_view.substr(0, pos);
        std::string_view last = range_view.substr(pos + 1);
        assert_msg(!first.empty(), "invalid first=%s", first.data());
        assert_msg(!last.empty(), "invalid last=%s", last.data());
        println("first={}, last={}", first, last);
        res += count_invalid(first, last);
        println("res={}", res);
    }
    return res;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        println(std::cerr, "Usage: {} <input_file>", argv[0]);
    }
    std::string_view filename = argv[1];
    println("Using file {}", filename);
    long long res = solve(filename);
    println("res={}", res);
    return 0;
}