#include "logging.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string_view>

using std::getline;
using std::ifstream;
using std::string;
using iss = std::istringstream;
using std::println;
using std::string_view;
#define LOG false

constexpr int START_POS = 50;
constexpr int DIAL_SIZE = 100;

/**
 * @brief
 *
 * @param pos
 * @param num
 * @param resp
 * @return size_t in range [1, 100]
 */
size_t dial(size_t pos, long num, int *resp) {
    /*
    if num > 0: compare num & 100 - pos
    else: compare num and pos

    1. 0 -> _       : round
    2. 0 -> 100     : round
    3. _ -> 0/100   : round + 1
    4. _ -> _       : round (cross?)
    */
    assert_msg(pos >= 0 && pos < 100, "pos=%zu", pos);

    size_t round = abs(num / 100);
    (*resp) += round;

    long step = num % 100;
    size_t new_pos = static_cast<size_t>((pos + step + 100) % 100);
    println("old_pos={}, new_pos={}, num={}, round={}, step={}", pos, new_pos,
            num, round, step);
    if (step == 0 || pos == 0) {
        return new_pos;
    }

    if (step > 0) {
        int rem = 100 - pos;
        if (step >= rem) {
            (*resp)++;
            clog("+1");
        }
    } else {
        step = abs(step);
        if (step >= pos) {
            (*resp)++;
            clog("+1");
        }
    }
    return new_pos;
}

int solve(string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream input_file(filename.data());
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
    }

    size_t pos = START_POS;
    int res = 0;
    string cmd;
    while (getline(input_file, cmd)) {
        if (cmd.empty())
            break;
        char dir = cmd[0];
        long num = std::stol(cmd.substr(1).c_str());
        assert(num >= 0 && "Got negtive num?");
        if (dir == 'L')
            num = -num;
        pos = dial(pos, num, &res);
    }
    return res;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        println(std::cerr, "Usage: {} <input_file>", argv[0]);
    }
    try {
        string_view filename = argv[1];
        size_t res = solve(filename);
        println("password: {}", res);
    } catch (const std::exception &e) {
        std::println(std::cerr, "An unexpected error occurred: {}", e.what());
        return 1;
    } catch (...) {
        std::println(std::cerr, "An unknown error occurred.");
        return 1;
    }
    return 0;
}