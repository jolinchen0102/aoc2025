#include "logging.h"
#include "utils.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

constexpr bool LOG = true;

using graph = std::vector<std::vector<int>>;

constexpr bool cell_empty(int elem) { return elem < 0; }
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

static inline void _alter_top_right(graph &g, size_t row, size_t col,
                                    int delta) {
    if (cell_empty(g[row - 1][col + 1]))
        return;
    g[row - 1][col + 1] += delta;
    g[row][col] += 1;
}

static inline void _alter_top_left(graph &g, size_t row, size_t col,
                                   int delta) {
    if (cell_empty(g[row - 1][col - 1]))
        return;
    g[row - 1][col - 1] += delta;
    g[row][col] += 1;
}

static inline void _alter_up(graph &g, size_t row, size_t col, int delta) {
    if (cell_empty(g[row - 1][col]))
        return;
    g[row - 1][col] += delta;
    g[row][col] += 1;
}

static inline void _alter_left(graph &g, size_t row, size_t col, int delta) {
    if (cell_empty(g[row][col - 1])) {
        return;
    }
    g[row][col - 1] += delta;
    g[row][col] += 1;
}

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
    if (row == 0) {
        // Modify left only
        if (col - 1 >= 0) {
            _alter_left(g, row, col, 1);
        }
    } else {
        size_t max_col_ex = g[row - 1].size();
        _alter_up(g, row, col, 1);
        if (col == 0) {
            assert(col + 1 < max_col_ex);
            _alter_top_right(g, row, col, 1);
            return;
        } else if (col < max_col_ex - 1) {
            assert(col + 1 < max_col_ex);
            _alter_top_right(g, row, col, 1);
        }
        assert(col - 1 >= 0);
        _alter_top_left(g, row, col, 1);
        _alter_left(g, row, col, 1);
    }
}

static void decrement_neighbors(graph &g, size_t row, size_t col) {
    size_t row_cnt = g.size();
    size_t col_cnt = g[0].size();
    if (row > 0) {
        decr_elem(g[row - 1][col]);
        if (col > 0) {
            decr_elem(g[row - 1][col - 1]);
        }
        if (col < col_cnt - 1) {
            decr_elem(g[row - 1][col + 1]);
        }
    }
    if (row < row_cnt - 1) {
        decr_elem(g[row + 1][col]);
        if (col > 0) {
            decr_elem(g[row + 1][col - 1]);
        }
        if (col < col_cnt - 1) {
            decr_elem(g[row + 1][col + 1]);
        }
    }
    if (col > 0) {
        decr_elem(g[row][col - 1]);
    }
    if (col < col_cnt - 1) {
        decr_elem(g[row][col + 1]);
    }
    print_graph(g);
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
                g[row][col] = -2;
                decrement_neighbors(g, row, col);
                // print_graph(g);
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