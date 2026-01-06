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
#include <sstream>
#include <string>
#include <vector>

constexpr bool LOG = true;

// max heap with vectro: keep top k smallest {dist, pos}
// vector: store pairs
// connect pos in heap

constexpr size_t num_coords = 3;
constexpr size_t topk = 3;
using pos = std::array<ll, num_coords>;
struct Link;
using heap = std::vector<Link>;
using pairs = std::vector<pos>;

struct Link {
    double dist;
    // index into the pairs vector, each represent a pos
    size_t idx_a;
    size_t idx_b;
};
// min heap
static inline auto link_comp = [](const Link &a, const Link &b) {
    return a.dist > b.dist;
};

static pos split(const std::string &line) {
    std::stringstream ss(line);
    std::string token;
    pos nums;
    for (size_t i = 0; i < num_coords; i++) {
        assert(getline(ss, token, ','));
        nums[i] = std::stoll(token);
    }
    return nums;
}

static double get_dist(const pos &pos_a, const pos &pos_b) {
    ll dist_sqr = 0;
    for (size_t i = 0; i < num_coords; i++) {
        dist_sqr += (pos_a[i] - pos_b[i]) * (pos_a[i] - pos_b[i]);
    }
    assert(dist_sqr > 0);
    return std::sqrt(dist_sqr);
}

static void insert_to_heap(heap &heap, Link &l) {
    heap.emplace_back(l);
    std::push_heap(heap.begin(), heap.end(), link_comp);
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
    pairs pairs; // store all positions
    heap smallestk;
    size_t new_idx = 0;
    while (getline(input_file, line)) {
        if (line.empty())
            break;
        pos new_pos = split(line);
        auto cur_size = pairs.size();
        for (size_t i = 0; i < cur_size; i++) {
            auto pos = pairs[i];
            auto dist = get_dist(pos, new_pos);
            Link l{dist, new_idx, i};
            insert_to_heap(smallestk, l);
        }
        pairs.emplace_back(new_pos);
        new_idx++;
    }
    // construct disjoint set
    auto num_pos = pairs.size();
    auto disjoint_set = UnionFindBySize<size_t>(num_pos);
    while (smallestk.size()) {
        std::pop_heap(smallestk.begin(), smallestk.end(), link_comp);
        auto [dist, a, b] = smallestk.back();
        smallestk.pop_back();
        disjoint_set.unionBySize(a, b);
        if (disjoint_set.getMaxSize() == num_conn) {
            return pairs[a][0] * pairs[b][0];
        }
    }
    assert_msg(false, "shouldn't reach");
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