
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
#include <string>
#include <string_view>
#include <vector>

constexpr bool LOG = false;
using iss = std::istringstream;
using matrix = std::vector<std::vector<std::string>>;
using matrix_row = std::vector<std::string>;

struct col_ops {
    char op;
    size_t start;
    size_t end;
};

static void print_matrix_row(matrix_row &row) {
    for (const auto &elem : row) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;
}

static int parse_files(matrix_row &raw_nums, std::vector<col_ops> &ops,
                       std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }
    std::string line;
    size_t r = 0;
    size_t num_rows = raw_nums.size();
    size_t pos = 0;
    while (getline(input_file, line)) {
        if (line.empty())
            break;
        if (r < num_rows)
            raw_nums[r++] = line;
        else {
            pos = line.find_first_not_of(' ');
            size_t op_cnt = 0;
            while (pos < line.size() + 1) {
                char op = line[pos];
                size_t next_pos = line.find_first_not_of(' ', pos + 1);
                if (next_pos == std::string_view::npos)
                    next_pos = line.size() + 1;
                assert(op_cnt < ops.size());
                ops[op_cnt++] = col_ops{op, pos, next_pos - 1};
                pos = next_pos;
            }
        }
    }
    // print_matrix_row(raw_nums);
    return 0;
}

static ll calculate(matrix_row &raw_nums, std::vector<col_ops> &ops) {
    ll total = 0;
    size_t num_rows = raw_nums.size();
    assert(num_rows > 0);
    size_t num_cols = ops.size();
    assert(num_cols > 0);
    for (size_t i = 0; i < num_cols; i++) {
        auto [op, start, end] = ops[i];
        size_t num_cnt = end - start;
        ll res = 0;
        for (size_t j = 0; j < num_cnt; j++) {
            std::string num_str{};
            for (size_t row = 0; row < num_rows; row++) {
                num_str += raw_nums[row][start + j];
            }
            ll num = std::stoll(num_str);
            clogf("got num %llu", num);
            if (res == 0)
                res = num;
            else {
                switch (op) {
                case '*': {
                    res *= num;
                    break;
                }
                case '+': {
                    res += num;
                    break;
                }
                default:
                    assert_msg(false, "invalid op %c", op);
                }
            }
        }
        clogf("%zu ops got %llu", i, res);
        total += res;
    }

    return total;
}

static ll solve(std::string_view filename) {
    int num_rows = get_num_rows(filename);
    assert(num_rows > 0);
    int num_cols = get_num_cols(filename);
    assert(num_cols > 0);
    assert(filename.data() != NULL);

    matrix_row raw_nums(num_rows - 1);
    std::vector<col_ops> ops(num_cols);
    if (parse_files(raw_nums, ops, filename) < 0) {
        std::println("parsing failed");
        return -1;
    }
    ll res = calculate(raw_nums, ops);

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