#include "utils.h"

size_t get_num_digits(long num) {
    size_t digits = 0;
    while (num / 10) {
        num /= 10;
        digits++;
    }
    return digits;
}
