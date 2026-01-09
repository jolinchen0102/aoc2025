#include "logging.h"
#include "utils.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <print>
#include <string>
#include <string_view>
#include <vector>

using Coord = std::array<ll, 2>;
using Polygon = std::vector<Coord>;

inline ll area(const Coord &a, const Coord &b) {
    return (std::abs(a[0] - b[0]) + 1) * (std::abs(a[1] - b[1]) + 1);
}

bool is_on_edge(const Coord &p, const Coord &a, const Coord &b) {
    if (a[0] == b[0] && p[0] == a[0]) {
        ll min_y = std::min(a[1], b[1]);
        ll max_y = std::max(a[1], b[1]);
        return p[1] >= min_y && p[1] <= max_y;
    }
    if (a[1] == b[1] && p[1] == a[1]) {
        ll min_x = std::min(a[0], b[0]);
        ll max_x = std::max(a[0], b[0]);
        return p[0] >= min_x && p[0] <= max_x;
    }
    return false;
}

bool is_inside(const Coord &p, const Polygon &poly) {
    int crossings = 0;
    size_t n = poly.size();

    for (size_t i = 0; i < n; ++i) {
        const auto &a = poly[i];
        const auto &b = poly[(i + 1) % n];

        if (is_on_edge(p, a, b)) {
            return true;
        }

        if (a[0] != b[0]) {
            continue;
        }

        ll edge_x = a[0];
        ll min_y = std::min(a[1], b[1]);
        ll max_y = std::max(a[1], b[1]);

        if (edge_x > p[0] && p[1] >= min_y && p[1] < max_y) {
            crossings++;
        }
    }

    return (crossings % 2) == 1;
}

// Check if a polygon edge crosses through the INTERIOR of the rectangle
// (not just touching the boundary)
bool edge_crosses_rect_interior(const Coord &a, const Coord &b, ll min_x,
                                ll max_x, ll min_y, ll max_y) {
    // Vertical edge
    if (a[0] == b[0]) {
        ll x = a[0];
        ll e_min_y = std::min(a[1], b[1]);
        ll e_max_y = std::max(a[1], b[1]);

        // Edge is strictly inside x range
        if (x > min_x && x < max_x) {
            // Edge overlaps with rectangle's y range
            if (e_min_y < max_y && e_max_y > min_y) {
                return true;
            }
        }
    }
    // Horizontal edge
    else if (a[1] == b[1]) {
        ll y = a[1];
        ll e_min_x = std::min(a[0], b[0]);
        ll e_max_x = std::max(a[0], b[0]);

        // Edge is strictly inside y range
        if (y > min_y && y < max_y) {
            // Edge overlaps with rectangle's x range
            if (e_min_x < max_x && e_max_x > min_x) {
                return true;
            }
        }
    }

    return false;
}

// Check if rectangle is fully contained in polygon (no edges cross through)
bool rectangle_valid(const Coord &p1, const Coord &p2, const Polygon &poly) {
    Coord p3{p1[0], p2[1]};
    Coord p4{p2[0], p1[1]};

    // All 4 corners must be inside or on boundary
    if (!is_inside(p1, poly) || !is_inside(p2, poly) || !is_inside(p3, poly) ||
        !is_inside(p4, poly)) {
        return false;
    }

    // No polygon edge can cross through the interior
    ll min_x = std::min(p1[0], p2[0]);
    ll max_x = std::max(p1[0], p2[0]);
    ll min_y = std::min(p1[1], p2[1]);
    ll max_y = std::max(p1[1], p2[1]);

    size_t n = poly.size();
    for (size_t i = 0; i < n; ++i) {
        const auto &a = poly[i];
        const auto &b = poly[(i + 1) % n];

        if (edge_crosses_rect_interior(a, b, min_x, max_x, min_y, max_y)) {
            return false;
        }
    }

    return true;
}

ll solve(std::string_view filename) {
    std::ifstream input{filename.data()};
    if (!input) {
        std::println(std::cerr, "Couldn't open {}", filename);
        return -1;
    }

    Polygon red_tiles;
    std::string line;

    while (std::getline(input, line) && !line.empty()) {
        red_tiles.push_back(split_by_delim<Coord>(line, ','));
    }

    std::println("Loaded {} red tiles", red_tiles.size());

    ll max_area = 0;
    Coord best_p1{}, best_p2{};
    size_t n = red_tiles.size();
    size_t valid_count = 0;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            const auto &p1 = red_tiles[i];
            const auto &p2 = red_tiles[j];

            if (p1[0] == p2[0] || p1[1] == p2[1]) {
                continue;
            }

            if (rectangle_valid(p1, p2, red_tiles)) {
                valid_count++;
                ll a = area(p1, p2);
                if (a > max_area) {
                    max_area = a;
                    best_p1 = p1;
                    best_p2 = p2;
                }
            }
        }
    }

    std::println("Found {} valid rectangles", valid_count);
    std::println("Best: ({},{}) to ({},{}) = {}", best_p1[0], best_p1[1],
                 best_p2[0], best_p2[1], max_area);

    return max_area;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::println(std::cerr, "Usage: {} <input_file>", argv[0]);
        return 1;
    }

    std::println("Result: {}", solve(argv[1]));
    return 0;
}