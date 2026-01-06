#ifndef _UTILS_H
#define _UTILS_H

#include <cstddef>
#include <fstream>
#include <vector>

constexpr int char2int_shift = 48;
using ll = long long;

size_t get_num_digits(long num);
std::vector<long> get_factors(long num);
bool is_prime(long num);
bool all_digits_are_the_same(long num);
int get_num_rows(std::string_view file);
int get_num_cols(std::string_view file);

// T must be int compatible
template <typename T> class UnionFindBySize {
  public:
    explicit UnionFindBySize(size_t n) {
        parent_.resize(n);
        size_.resize(n);
        for (T i = 0; i < n; i++) {
            parent_[i] = static_cast<T>(i);
            size_[i] = static_cast<T>(1);
        }
    };

    T find(T elem) {
        while (elem != parent_[elem]) {
            elem = parent_[elem];
        }
        return elem;
    };

    void unionBySize(T a, T b) {
        if (a == b)
            return;
        auto parent_a = find(a);
        auto parent_b = find(b);
        if (parent_a == parent_b)
            return;
        auto size_a = size_[parent_a];
        auto size_b = size_[parent_b];
        if (size_a < size_b) {
            parent_[parent_a] = parent_b;
            size_[parent_b] += size_[parent_a];
            size_[parent_a] = 0; // BUG: is this allowed?
        } else {
            parent_[parent_b] = parent_a;
            size_[parent_a] += size_[parent_b];
            size_[parent_b] = 0; // BUG: is this allowed?
        }
    };

    std::vector<T> getSize(void) { return size_; }

  private:
    std::vector<T> parent_;
    std::vector<T> size_;
};

#endif /* _UTILS_H */