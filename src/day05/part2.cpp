#include "logging.h"
#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

constexpr bool LOG = true;

using range = std::pair<ll, ll>;

static range parse_range(std::string_view line) {
    size_t pos = line.find('-');
    if (pos == std::string_view::npos) {
        assert_msg(false, "Invalid input %s", line.data());
    }

    ll start = std::stoll(line.substr(0, pos).data());
    ll end = std::stoll(line.substr(pos + 1).data());
    return {start, end};
}

static void print_ranges(std::vector<range> &ranges) {
    for (const auto it : ranges) {
        std::cout << it.first << '-' << it.second << ", ";
    }
    std::cout << std::endl;
}

/**
 * @brief Starting merging intervals
 *
 * @param ranges
 * @param idx
 * @param r
 */
static ll count_intervals(std::vector<range> &ranges) {
    assert(ranges.size() > 0);
    std::vector<range> merged;
    merged.push_back(ranges[0]);
    for (const auto &[start, end] : ranges | std::views::drop(1)) {
        auto &[prev_start, prev_end] = merged.back();
        if (prev_end >= start) {
            if (prev_end < end) {
                // extend prev
                prev_end = end;
            }
        } else {
            // insert new
            merged.emplace_back(start, end);
        }
    }
    print_ranges(merged);

    ll cnt = 0;
    for (const auto &[start, end] : merged) {
        cnt += end - start + 1;
    }
    return cnt;
}

static ll solve(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
    }
    ll cnt = 0;
    std::vector<range> ranges;
    std::string line;
    // get ranges
    while (std::getline(input_file, line)) {
        if (line.empty())
            break;
        range range = parse_range(line);
        ranges.emplace_back(range);
    }
    print_ranges(ranges);
    // sort by ascending first, descending second
    std::sort(ranges.begin(), ranges.end(),
              [](const range &left, const range &right) {
                  if (left.first != right.first) {
                      return left.first < right.first;
                  }
                  return left.second > right.second;
              });
    print_ranges(ranges);
    cnt = count_intervals(ranges);

    return cnt;
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