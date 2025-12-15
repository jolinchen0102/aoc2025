#include "logging.h"
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <type_traits>

using std::getline;
using std::ifstream;
using std::string;
using iss = std::istringstream;
using std::max;
using std::println;

#define START_POS 50
#define SIGN(num) ((num) < 0)

int dial(int pos, int num, int *resp) {
    /*
    if num > 0: compare num & 100 - pos
    else: compare num and pos

    1. 0 -> _       : round
    2. 0 -> 100     : round
    3. _ -> 0/100   : round + 1
    4. _ -> _       : round (cross?)
    */
    assert_msg(pos >= 0 && pos < 100, "pos=%d", pos);

    size_t round = abs(num / 100);
    (*resp) += round;

    int step = num % 100;
    size_t new_pos = (pos + step + 100) % 100;
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

int solve(const char *filename) {
    assert(filename != NULL);
    std::ifstream input_file(filename);
    if (!input_file.is_open()) {
        println("Couldn't open input file {}", filename);
    }

    int pos = START_POS;
    int res = 0;
    string cmd;
    while (getline(input_file, cmd)) {
        if (cmd.empty())
            break;
        char dir = cmd[0];
        int num = std::atoi(cmd.substr(1).c_str());
        assert(num >= 0 && "Got negtive num?");
        switch (dir) {
        case 'L':
            num = -num;
            break;
        case 'R':
        default:
            break;
        }
        pos = dial(pos, num, &res);
    }
    return res;
}

int main(int argc, char **argv) {
    assert(argc >= 2 && "Give an input file as argument");
    char *filename = argv[1];
    println("Using input file: {}", filename);
    int res = solve(filename);
    println("password: {}", res);
}