#ifndef _UTILS_H
#define _UTILS_H

#include <charconv>
#include <cstddef>
#include <fstream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

constexpr int char2int_shift = 48;
using ll = long long;

size_t get_num_digits(long num);
std::vector<long> get_factors(long num);
bool is_prime(long num);
bool all_digits_are_the_same(long num);
int get_num_rows(std::string_view file);
int get_num_cols(std::string_view file);
constexpr inline int pow(int base, int exp) noexcept {
    auto res = 1;
    for (int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

template <typename T> T split_by_delim(std::string_view line, char delim) {
    T res{};
    size_t idx = 0;
    for (auto part : line | std::views::split(delim)) {
        std::string_view sv(part.begin(), part.end());
        std::from_chars(sv.data(), sv.data() + sv.size(), res[idx++]);
    }
    return res;
}

// T must be int compatible
template <typename T = size_t> class UnionFindBySize {
  public:
    using size_vec = std::vector<size_t>;
    using parent_vec = std::vector<T>;
    explicit UnionFindBySize(size_t n)
        : parent_(n), size_(n, 1), max_size_(1), num_components_(n) {
        // Fills the range [first, last) with sequentially increasing values,
        // starting with value and repetitively evaluating ++value.
        std::iota(parent_.begin(), parent_.end(), T{0});
    };

    T find(T elem) const {
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
        if (size_[parent_a] < size_[parent_b]) {
            std::swap(parent_a, parent_b);
        }
        // a >= b
        parent_[parent_b] = parent_a;
        size_[parent_a] += size_[parent_b];
        max_size_ = std::max(max_size_, size_[parent_a]);
        size_[parent_b] = 0; // optional?
        num_components_--;
    };

    size_vec get_size_vec(void) { return size_; }
    size_t get_max_size(void) const { return max_size_; }
    size_t num_components() const { return num_components_; }
    bool is_connected(T a, T b) const { return find(a) == find(b); }

  private:
    parent_vec parent_;
    size_vec size_;
    size_t max_size_;
    size_t num_components_;
};

#endif /* _UTILS_H */