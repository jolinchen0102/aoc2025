#ifndef _UTILS_H
#define _UTILS_H

#include <cstddef>
#include <vector>

size_t get_num_digits(long num);
std::vector<long> get_factors(long num);
bool is_prime(long num);
bool all_digits_are_the_same(long num);

#endif /* _UTILS_H */