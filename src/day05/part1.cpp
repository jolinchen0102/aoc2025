#include "logging.h"
#include "utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
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
static void merge_intervals(std::vector<range> &ranges) {
    assert(ranges.size() > 0);
    size_t pos = 0;
    while (pos + 1 < ranges.size()) {
        // [-----]
        //   [--]
        if (ranges[pos].second >= ranges[pos + 1].second) {
            // erase pos+1
            ranges.erase(ranges.begin() + pos + 1);
        }
        // [----]      [---]
        //   [----]          [--]
        else {
            pos++;
        }
    }
    print_ranges(ranges);
}

static bool elem_in_ranges(std::vector<range> &ranges, ll elem) {
    assert(!ranges.empty());
    range fake_range = range{elem, elem};
    auto rg_r_inc = std::lower_bound(ranges.begin(), ranges.end(), fake_range);
    if (rg_r_inc == ranges.begin()) {
        if (elem == rg_r_inc->first)
            return true;
        return false;
    }
    if (rg_r_inc != ranges.end() && elem == rg_r_inc->first)
        return true;
    auto it = rg_r_inc - 1;
    assert(elem > it->first);
    if (elem <= it->second)
        return true;
    return false;
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
    merge_intervals(ranges);
    std::vector<ll> elems;
    // get numbers
    while (std::getline(input_file, line)) {
        if (line.empty())
            break;
        ll elem = std::stoll(line.data());
        cnt += elem_in_ranges(ranges, elem) ? 1 : 0;
    }

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