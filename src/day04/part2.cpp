#include "logging.h"
#include "utils.h"
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

constexpr bool LOG = true;

using graph = std::vector<std::vector<int>>;
enum class Cell : int {
    Empty = -1,
    Removed = -2,
};

constexpr std::array<std::pair<int, int>, 8> NEIGHBORS = {
    {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}}};

constexpr void decr_elem(int &elem) {
    if (elem <= 0)
        return;
    elem = std::max(0, elem - 1);
}

constexpr bool can_remove_elem(int elem) {
    if (elem >= 0 && elem < 4)
        return true;
    return false;
}

static inline bool is_roll(int cell) { return cell >= 0; }

static void print_graph(graph &g) {
    size_t row_cnt = g.size();
    size_t col_cnt = g[0].size();
    for (const auto &row : g) {
        for (const auto elem : row) {
            if (elem == -1) {
                std::cout << '.';
            } else if (elem == -2) {
                std::cout << 'x';
            } else {
                std::cout << elem;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

static void alter_neighbors(graph &g, size_t row, size_t col) {
    assert(row >= 0 && col >= 0);
    g[row][col] = 0;
    for (auto [dr, dc] : NEIGHBORS) {
        auto nr = static_cast<int>(row) + dr;
        auto nc = static_cast<int>(col) + dc;
        if (nr < 0 || nc < 0)
            continue;
        if (static_cast<size_t>(nr) >= g.size())
            continue;
        if (static_cast<size_t>(nc) >= g[nr].size())
            continue;

        if (is_roll(g[nr][nc])) {
            g[nr][nc]++;
            g[row][col]++;
        }
    }
}

static void decrement_neighbors(graph &g, size_t row, size_t col) {
    int row_cnt = g.size();
    int col_cnt = g[0].size();
    for (auto [dr, dc] : NEIGHBORS) {
        auto nr = static_cast<int>(row) + dr;
        auto nc = static_cast<int>(col) + dc;
        if (nr < 0 || nr >= row_cnt || nc < 0 || nc >= col_cnt)
            continue;
        decr_elem(g[nr][nc]);
    }
    // print_graph(g);
}

static size_t remove_elements(graph &g) {
    size_t row_cnt = g.size();
    size_t col_cnt = g[0].size();
    size_t removed_cnt = 0;
    // print_graph(g);
    for (size_t row = 0; row < row_cnt; row++) {
        for (size_t col = 0; col < col_cnt; col++) {
            auto elem = g[row][col];
            if (can_remove_elem(elem)) {
                g[row][col] = static_cast<int>(Cell::Removed);
                decrement_neighbors(g, row, col);
                removed_cnt++;
            }
        }
    }
    // print_graph(g);
    return removed_cnt;
}

static ll count_rolls(graph &g) {
    size_t remove_cnts = 0;
    ll res = 0;
    do {
        remove_cnts = remove_elements(g);
        res += remove_cnts;
    } while (remove_cnts > 0);

    return res;
}

static ll solve(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
    }

    ll res = 0;
    std::string line;
    graph graph;
    size_t row = 0;
    while (getline(input_file, line)) {
        size_t line_len = line.length();
        graph.emplace_back(line_len, -1);
        for (size_t col = 0; col < line_len; col++) {
            char c = line[col];
            if (c == '.') {
                graph[row][col] = -1;
            } else {
                assert(c == '@');
                alter_neighbors(graph, row, col);
            }
        }
        row++;
        // print_graph(graph);
    }
    res = count_rolls(graph);
    return res;
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