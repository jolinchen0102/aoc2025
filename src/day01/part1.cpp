#include "logging.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdio.h>

using std::getline;
using std::ifstream;
using std::string;
using iss = std::istringstream;
using std::println;

#define START_POS 50
#define WRAPPED_MOVE(start, num) (((start) + (num)) % 100);

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
        pos = WRAPPED_MOVE(pos, num);
        if (pos == 0) {
            res++;
        }
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