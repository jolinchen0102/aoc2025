#include "utils.h"
#include "logging.h"
#include <cstddef>
#include <sstream>
#include <vector>

size_t get_num_digits(long num) {
    size_t digits = 0;
    if (num == 0)
        digits = 1;
    while (num != 0) {
        num /= 10;
        digits++;
    }
    return digits;
}

std::vector<long> get_factors(long num) {
    std::vector<long> factors;
    if (num == 0)
        return factors;
    assert_msg(num > 0, "Can't handle negative number %lu", num);
    for (long i = 1; i * i <= num; i++) {
        if (num % i == 0) {
            factors.push_back(i);
            if (i * i != num) {
                factors.push_back(num / i);
            }
        }
    }
    return factors;
}

bool is_prime(long num) {
    assert_msg(num >= 0, "Can't handle negative number %lu", num);
    if (num <= 1)
        return false;
    if (num <= 3)
        return true;
    if (num % 2 == 0 || num % 3 == 0)
        return false;
    for (long i = 5; i * i <= num; i += 6) {
        if ((num % i == 0) || (num % (i + 2) == 0))
            return false;
    }
    return true;
}

int get_num_rows(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream file{filename.data()};
    if (!file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }
    size_t cnt = 0;
    std::string line;
    while (getline(file, line)) {
        cnt++;
    }
    return cnt;
}

int get_num_cols(std::string_view filename) {
    assert(filename.data() != NULL);
    std::ifstream file{filename.data()};
    if (!file.is_open()) {
        println("Couldn't open input file {}", filename);
        return -1;
    }
    int cnt = 0;
    std::string line;
    if (!getline(file, line))
        return -1;
    std::istringstream iss{line};
    std::string col;
    while (iss >> col) {
        cnt++;
    }
    return cnt;
}
