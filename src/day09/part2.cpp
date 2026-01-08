#include "logging.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <deque>
#include <fstream>
#include <iostream>
#include <queue>
#include <sys/wait.h>
#include <unordered_map>
#include <utility>
#include <vector>

using Coord = std::array<ll, 2>;
using FourPts = std::deque<Coord>;
using Pairs = std::vector<Coord>;

constexpr bool LOG = true;

inline ll area(const Coord &a, const Coord &b) {
    return (std::abs(a[0] - b[0]) + 1) * (std::abs(a[1] - b[1]) + 1);
}

bool is_on_edge(const Coord &p, const Coord &a, const Coord &b) {
    if (a[0] == b[0] && p[0] == a[0]) {
        // Vertical segment
        ll min_y = std::min(a[1], b[1]);
        ll max_y = std::max(a[1], b[1]);
        return p[1] >= min_y && p[1] <= max_y;
    }
    if (a[1] == b[1] && p[1] == a[1]) {
        // Horizontal segment
        ll min_x = std::min(a[0], b[0]);
        ll max_x = std::max(a[0], b[0]);
        return p[0] >= min_x && p[0] <= max_x;
    }
    return false;
}

bool is_inside(const Coord &p, const Pairs &poly) {
    int crossings = 0;
    size_t n = poly.size();

    for (size_t i = 0; i < n; ++i) {
        const auto &a = poly[i];
        const auto &b = poly[(i + 1) % n];

        // Check if point is on this edge
        if (is_on_edge(p, a, b)) {
            return true;
        }

        // Skip horizontal edges for ray casting
        if (a[1] == b[1]) {
            continue;
        }

        // Vertical edge: a[0] == b[0]
        if (a[0] == b[0]) {
            ll edge_x = a[0];
            ll min_y = std::min(a[1], b[1]);
            ll max_y = std::max(a[1], b[1]);

            // Ray goes right (+X), so edge must be to the RIGHT of point
            // Use half-open interval [min_y, max_y) to avoid double-counting
            // vertices
            if (edge_x > p[0] && p[1] >= min_y && p[1] < max_y) {
                crossings++;
            }
        }
        // Non-axis-aligned edges shouldn't exist in this problem
    }

    return (crossings % 2) == 1;
}

ll solve(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }
    std::string line;
    FourPts candidates;
    Pairs pairs;
    ll max_area = 0;
    Coord first;
    while (getline(input_file, line) && !line.empty()) {
        pairs.emplace_back(split_by_delim<Coord>(line, ','));
    }
    auto num_pts = pairs.size();
    for (int i = 0; i < num_pts; i++) {
        for (int j = i + 1; j < num_pts; j++) {
            // p1 p2 are diagonal
            const auto &p1 = pairs[i];
            const auto &p2 = pairs[j];
            if (p1[0] == p2[0] || p1[1] == p2[1]) {
                continue;
            }
            auto p3 = Coord{p1[0], p2[1]};
            if (!is_inside(p3, pairs))
                continue;
            auto p4 = Coord{p2[0], p1[1]};
            if (!is_inside(p4, pairs))
                continue;
            max_area = std::max(max_area, area(p1, p2));
        }
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