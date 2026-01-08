#include "logging.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
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

    auto operator<=>(const Link &other) const { return dist <=> other.dist; }
};
using MaxHeap = std::priority_queue<Link>;

static double get_dist(const Pos &pos_a, const Pos &pos_b) {
    ll dist_sqr = 0;
    for (size_t i = 0; i < num_coords; i++) {
        dist_sqr += (pos_a[i] - pos_b[i]) * (pos_a[i] - pos_b[i]);
    }
    assert(dist_sqr > 0);
    return std::sqrt(static_cast<double>(dist_sqr));
}

static void keep_smallest_k(MaxHeap &heap, Link link, size_t k) {
    heap.push(link);
    while (heap.size() > k) {
        heap.pop();
    }
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
    MaxHeap smallestk;
    size_t new_idx = 0;
    while (getline(input_file, line)) {
        if (line.empty())
            break;
        Pos new_pos = split_by_delim<Pos>(line, ',');
        auto cur_size = positions.size();
        new_idx = cur_size;
        for (size_t i = 0; i < cur_size; i++) {
            auto dist = get_dist(positions[i], new_pos);
            keep_smallest_k(smallestk, {dist, new_idx, i}, num_conn);
        }
        positions.emplace_back(new_pos);
    }
    assert(smallestk.size() == num_conn);
    // construct disjoint set
    auto num_pos = positions.size();
    UnionFindBySize<size_t> disjoint_set(num_pos);
    while (!smallestk.empty()) {
        auto [dist, a, b] = smallestk.top();
        smallestk.pop();
        disjoint_set.unionBySize(a, b);
    }

    auto sizes = disjoint_set.get_size_vec();
    std::ranges::partial_sort(sizes, sizes.begin() + topk, std::greater{});
    std::sort(sizes.begin(), sizes.end(), std::greater<size_t>());
    for (size_t i = 0; i < topk; i++) {
        res *= sizes[i];
    }
    return res;
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