#include "logging.h"
#include "utils.h"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#define LOG false

using std::println;

static long repeat_number_to_len(long num, size_t num_len, size_t target_len) {
    assert_msg(target_len % num_len == 0, "%zu is not factor of %zu", num_len,
               target_len);
    assert(num > 0);
    long res = 0;
    for (int i = target_len - num_len; i >= 0; i -= num_len) {
        res += num * (pow(10, i));
    }
    clogf("num=%zu, num_len=%zu, target_len=%zu, res=%zu", num, num_len,
          target_len, res);
    return res;
}

static bool is_invalid_num(long num, size_t num_digits) {
    auto factors = get_factors(num_digits);
    for (auto len : factors) {
        if (len == num_digits)
            continue;
        // get the least significant len digits
        long subnum = num % (static_cast<long>(std::pow(10, len)));
        if (subnum == 0)
            continue;
        long target = repeat_number_to_len(subnum, len, num_digits);
        if (target == num)
            return true;
    }
    return false;
}

static long repeate_digit_for_len(size_t digit, size_t len) {
    assert_msg(digit > 0 & digit < 10, "Invalid digit %zu", digit);
    assert_msg(len > 0, "Invalid len %zu", len);
    long res = 0;
    for (int d = len - 1; d >= 0; d--) {
        res += digit * pow(10, d);
    }
    return res;
}

static size_t count_invalid(std::string_view first, std::string_view last) {
    assert(first.size() > 0);
    assert_msg(last.size() > 0, "%zu", last.size());
    size_t len_first = first.length();
    size_t len_last = last.length();
    if (len_last < len_first)
        return 0;
    size_t invalid_sum = 0;
    long first_num = std::stol(first.data());
    long last_num = std::stol(last.data());
    long checking = first_num;
    clogf("checking %zu", checking);
    while (checking <= last_num) {
        size_t num_digits = get_num_digits(checking);
        clogf("num_digits=%zu", num_digits);
        if (is_prime(num_digits)) {
            // accunting for all possible invalid nums with all digits being the
            // same
            clogf("%zu is prime digits", checking);
            size_t first_digit = checking / pow(10, num_digits - 1);
            clogf("first_digit=%zu", first_digit);
            while (first_digit < 10) {
                long inv = repeate_digit_for_len(first_digit, num_digits);
                clogf("constucted invalid number: %zu", inv);
                if (inv >= first_num && inv <= last_num) {
                    invalid_sum += inv;
                    clogf("+%zu", inv);
                }
                if (inv >= last_num)
                    break;
                first_digit++;
            }
            // skip to next number of digits
            checking = pow(10, num_digits);
            clogf("skipping to %zu", checking);
        } else {
            clogf("%zu is not prime digits", checking);
            // invalid_sum += is_invalid_num(checking, num_digits) ? checking :
            // 0;
            bool is_invalid = is_invalid_num(checking, num_digits);
            if (is_invalid) {
                invalid_sum += checking;
                clogf("+%zu", checking);
            }
            checking++;
        }
    }

    return invalid_sum;
}

static size_t solve(std::string_view filename) {
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
        clogf("first=%s, last=%s", first.data(), last.data());
        res += count_invalid(first, last);
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