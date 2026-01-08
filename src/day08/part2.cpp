#include "logging.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <print>
#include <queue>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

constexpr bool LOG = true;

// max heap with vectro: keep top k smallest {dist, Pos}
// vector: store positions
// connect Pos in heap

constexpr size_t num_coords = 3;
constexpr size_t topk = 3;
using Pos = std::array<ll, num_coords>;
using Positions = std::vector<Pos>;
struct Link {
    double dist;
    // index into the positions vector, each represent a Pos
    size_t idx_a;
    size_t idx_b;

    // min heap, reversed
    auto operator<=>(const Link &other) const { return other.dist <=> dist; }
};
using MinHeap = std::priority_queue<Link>;

static double get_dist(const Pos &pos_a, const Pos &pos_b) {
    ll dist_sqr = 0;
    for (size_t i = 0; i < num_coords; i++) {
        dist_sqr += (pos_a[i] - pos_b[i]) * (pos_a[i] - pos_b[i]);
    }
    assert(dist_sqr > 0);
    return std::sqrt(static_cast<double>(dist_sqr));
}

static ll solve(std::string_view filename, size_t num_conn) {
    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }
    ll res = 1;
    std::string line;
    Positions positions; // store all positions
    MinHeap smallestk;
    size_t new_idx = 0;
    while (getline(input_file, line) && !line.empty()) {
        Pos new_pos = split_by_delim<Pos>(line, ',');
        auto cur_size = positions.size();
        new_idx = cur_size;
        for (size_t i = 0; i < cur_size; i++) {
            auto dist = get_dist(positions[i], new_pos);
            smallestk.push({dist, new_idx, i});
        }
        positions.emplace_back(new_pos);
    }
    // construct disjoint set
    auto num_pos = positions.size();
    UnionFindBySize disjoint_set(num_pos);
    while (!smallestk.empty()) {
        auto [dist, a, b] = smallestk.top();
        smallestk.pop();
        if (disjoint_set.is_connected(a, b))
            continue;
        disjoint_set.unionBySize(a, b);
        if (disjoint_set.get_max_size() == num_conn) {
            return positions[a][0] * positions[b][0];
        }
    }
    std::println(std::cerr, "Failed");
    return -1;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        println(std::cerr, "Usage: {} <input_file> <num_conn>", argv[0]);
    }
    std::string_view filename = argv[1];
    size_t num_conn = std::stod(argv[2]);
    println("Using file {}, number of conn {}", filename, num_conn);
    ll res = solve(filename, num_conn);
    std::println("res={}", res);
    return 0;
}