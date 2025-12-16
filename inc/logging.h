#ifndef _LOGGING_H
#define _LOGGING_H

#include <cassert>
#include <cstdio>

#ifdef ENABLE_LOGGING
#define clogf(fmt, x...)                                                       \
    do {                                                                       \
        if (LOG) {                                                             \
            printf("[%s] %s:%d " fmt "\n", __func__, __FILE__, __LINE__, x);   \
        }                                                                      \
    } while (0)

#define clog(x)                                                                \
    do {                                                                       \
        if (LOG) {                                                             \
            printf("[%s] %s:%d " x "\n", __func__, __FILE__, __LINE__);        \
        }                                                                      \
    } while (0)

#else
#define clogf(fmt, x...) ((void)0)
#define clog(x) ((void)0)
#endif // ENABLE_LOGGING

#define assert_msg(condition, fmt, ...)                                        \
    do {                                                                       \
        if (!(condition)) {                                                    \
            fprintf(stderr, "[%s] %s:%d " fmt "\n", __func__, __FILE__,        \
                    __LINE__, ##__VA_ARGS__);                                  \
            assert(condition);                                                 \
        }                                                                      \
    } while (0)

#endif /* _LOGGING_H */