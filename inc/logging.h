#ifndef _LOGGING_H
#define _LOGGING_H

#define clogf(fmt, x...)                                                       \
    do {                                                                       \
        printf("[%s] %s:%d " fmt "\n", __func__, __FILE__, __LINE__, x);       \
    } while (0)

#define clog(x)                                                                \
    do {                                                                       \
        printf("[%s] %s:%d " x "\n", __func__, __FILE__, __LINE__);            \
    } while (0)

#define assert_msg(condition, fmt, ...)                                        \
    do {                                                                       \
        if (!(condition)) {                                                    \
            fprintf(stderr, fmt "\n", ##__VA_ARGS__);                          \
            assert(condition);                                                 \
        }                                                                      \
    } while (false)
#endif /* _LOGGING_H */