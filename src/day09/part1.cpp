#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

using Coord = std::array<ll, 2>;
using Pairs = std::vector<Coord>;

inline ll area(const Coord &a, const Coord &b) {
    return (std::abs(a[0] - b[0]) + 1) * (std::abs(a[1] - b[1]) + 1);
}

ll solve(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }
    std::string line;
    Pairs pairs;
    ll max_area = 0;
    while (getline(input_file, line) && !line.empty()) {
        auto p = split_by_delim<Coord>(line, ',');
        for (const auto &other_p : pairs) {
            max_area = std::max(max_area, area(p, other_p));
        }
        pairs.push_back(p);
    }
    return max_area;
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