#include "logging.h"
#include "utils.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

constexpr bool LOG = true;
using BeamPos = std::vector<bool>;

void init_puzzle(std::string_view line, size_t &width, BeamPos &pos) {
    width = line.size();
    pos = BeamPos(width, false);
    for (size_t i = 0; i < width; i++) {
        if (line[i] == 'S')
            pos[i] = true;
    }
}

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
    BeamPos pos_has_beam;
    while (getline(input_file, line)) {
        if (line.empty())
            break;
        if (is_first_line) {
            init_puzzle(line, width, pos_has_beam);
            is_first_line = false;
            continue;
        }
        for (size_t i = 0; i < width; i++) {
            if (line[i] == '^' && pos_has_beam[i]) {
                pos_has_beam[i] = false;
                res++;
                if (i - 1 >= 0) {
                    pos_has_beam[i - 1] = true;
                }
                if (i + 1 < width) {
                    pos_has_beam[i + 1] = true;
                }
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