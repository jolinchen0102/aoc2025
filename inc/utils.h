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
    using size_vec = std::vector<size_t>;
    using parent_vec = std::vector<T>;
    explicit UnionFindBySize(size_t n) {
        parent_.resize(n);
        size_.resize(n);
        for (T i = 0; i < n; i++) {
            parent_[i] = static_cast<T>(i);
            size_[i] = 1;
        }
        max_size_ = 1;
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
            max_size_ =
                (size_[parent_b] > max_size_) ? size_[parent_b] : max_size_;
            size_[parent_a] = 0;
        } else {
            parent_[parent_b] = parent_a;
            size_[parent_a] += size_[parent_b];
            max_size_ =
                (size_[parent_a] > max_size_) ? size_[parent_a] : max_size_;
            size_[parent_b] = 0;
        }
    };

    size_vec getSize(void) { return size_; }
    size_t getMaxSize(void) { return max_size_; }

  private:
    parent_vec parent_;
    size_vec size_;
    size_t max_size_;
};

#endif /* _UTILS_H */