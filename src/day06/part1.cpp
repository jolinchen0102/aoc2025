
#include "logging.h"
#include "utils.h"
#include <algorithm>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <vector>

constexpr bool LOG = true;
using iss = std::istringstream;
using matrix = std::vector<std::vector<ll>>;
using matrix_row = std::vector<ll>;

static ll calculate(matrix &ops, char op, size_t col) {
    assert(ops.size() > col);
    assert(ops[0].size() > 0);
    ll res = ops[col][0];
    switch (op) {
    case '+': {
        for (ll num : ops[col] | std::views::drop(1)) {
            res += num;
        }
        // how to turn this into a foreach?
        break;
    }
    case '*': {
        for (ll num : ops[col] | std::views::drop(1)) {
            res *= num;
        }
        break;
    }
    default: {
        assert_msg(false, "invalid symbol %c", op);
    }
    }
    return res;
}

static ll solve(std::string_view filename) {
    int num_rows = get_num_rows(filename);
    assert(num_rows > 0);
    int num_cols = get_num_cols(filename);
    assert(num_cols > 0);

    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }

    matrix ops(num_cols, matrix_row(num_rows - 1, 0));
    ll res = 0;
    std::string line;
    for (size_t r = 0; r < num_rows; r++) {
        if (!getline(input_file, line)) {
            if (line.empty()) {
                break;
            }
            assert_msg(false, "can't parse %s", line.c_str());
            return -1;
        }
        iss iss{line};
        if (r < num_rows - 1) {
            for (size_t c = 0; c < num_cols; c++) {
                iss >> ops[c][r];
            }
        } else {
            char op;
            for (size_t c = 0; c < num_cols; c++) {
                iss >> op;
                res += calculate(ops, op, c);
            }
        }
    }
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