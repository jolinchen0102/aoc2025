#include "utils.h"

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
