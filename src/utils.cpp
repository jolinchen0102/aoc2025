#include "utils.h"
#include "logging.h"
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
