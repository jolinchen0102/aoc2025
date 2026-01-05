#include "logging.h"
#include "utils.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <ostream>
#include <vector>

constexpr bool LOG = true;
using PathCnt = std::vector<ll>;

static void init_puzzle(std::string_view line, size_t &width,
                        PathCnt &path_cnt) {
    width = line.size();
    path_cnt = PathCnt(width, 0);
    for (size_t i = 0; i < width; i++) {
        if (line[i] == 'S') {
            path_cnt[i] = 1;
            return;
        }
    }
}

static ll inline sum_cnt(PathCnt &path) {
    ll res = 0;
    for (const auto e : path) {
        res += e;
    }
    return res;
}

static void print_graph(PathCnt &path, std::string_view line) {
    auto size = path.size();
    for (size_t i = 0; i < size; i++) {
        if (path[i] > 0) {
            std::cout << path[i];
        } else {
            std::cout << line[i];
        }
    }
    std::cout << std::endl;
}

static inline bool has_beam(ll prev_cnt) { return prev_cnt > 0; }

ll solve(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file{filename.data()};
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }

    ll res = 0;
    std::string line;
    bool is_first_line = true;
    size_t width = 0;
    PathCnt prev_cnt;
    while (getline(input_file, line)) {
        if (line.empty())
            break;
        if (is_first_line) {
            init_puzzle(line, width, prev_cnt);
            is_first_line = false;
            continue;
        }
        for (size_t i = 0; i < width; i++) {
            if (line[i] == '^' && has_beam(prev_cnt[i])) {
                if (i - 1 >= 0) {
                    prev_cnt[i - 1] = has_beam(prev_cnt[i - 1])
                                          ? prev_cnt[i - 1] + prev_cnt[i]
                                          : prev_cnt[i];
                }
                if (i + 1 < width) {
                    prev_cnt[i + 1] = has_beam(prev_cnt[i + 1])
                                          ? prev_cnt[i + 1] + prev_cnt[i]
                                          : prev_cnt[i];
                }
                prev_cnt[i] = 0;
            }
        }
        // print_graph(prev_cnt, line);
    }
    clogf("%lld", std::accumulate(prev_cnt.begin(), prev_cnt.end(), 0LL));
    clogf("%d", std::accumulate(prev_cnt.begin(), prev_cnt.end(), 0));

    return sum_cnt(prev_cnt);
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