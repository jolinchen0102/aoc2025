#include "logging.h"
#include "utils.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#define LOG true

using graph = std::vector<std::vector<int>>;

static inline void _alter_top_right(graph &g, size_t row, size_t col) {
    if (g[row - 1][col + 1] == -1)
        return;
    g[row - 1][col + 1] += 1;
    g[row][col] += 1;
}

static inline void _alter_top_left(graph &g, size_t row, size_t col) {
    if (g[row - 1][col - 1] == -1)
        return;
    g[row - 1][col - 1] += 1;
    g[row][col] += 1;
}

static inline void _alter_up(graph &g, size_t row, size_t col) {
    if (g[row - 1][col] == -1)
        return;
    g[row - 1][col] += 1;
    g[row][col] += 1;
}

static inline void _alter_left(graph &g, size_t row, size_t col) {
    if (g[row][col - 1] == -1) {
        return;
    }
    g[row][col - 1] += 1;
    g[row][col] += 1;
}

static void print_graph(graph &g) {
    size_t row_cnt = g.size();
    size_t col_cnt = g[0].size();
    for (const auto &row : g) {
        for (const auto elem : row) {
            if (elem == -1) {
                std::cout << '.';
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
    if (row == 0) {
        // Modify left only
        if (col - 1 >= 0) {
            _alter_left(g, row, col);
        }
    } else {
        size_t max_col_ex = g[row - 1].size();
        _alter_up(g, row, col);
        if (col == 0) {
            assert(col + 1 < max_col_ex);
            _alter_top_right(g, row, col);
            return;
        } else if (col < max_col_ex - 1) {
            assert(col + 1 < max_col_ex);
            _alter_top_right(g, row, col);
        }
        assert(col - 1 >= 0);
        _alter_top_left(g, row, col);
        _alter_left(g, row, col);
    }
}

static ll count_rolls(const graph &g) {
    size_t row_cnt = g.size();
    size_t col_cnt = g[0].size();
    ll cnt = 0;
    for (const auto &row : g) {
        for (const auto elem : row) {
            if (elem != -1 && elem < 4)
                cnt++;
        }
    }
    return cnt;
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