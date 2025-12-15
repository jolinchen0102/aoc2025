#ifndef _LOGGING_H
#define _LOGGING_H

#define clogf(fmt, x...)                                                                                               \
    do {                                                                                                               \
        printf("[%s] %s:%d " fmt, __func__, __FILE__, __LINE__, x);                                                    \
    } while (0)

#define clog(x)                                                                                                        \
    do {                                                                                                               \
        printf("[%s] %s:%d " x, __func__, __FILE__, __LINE__);                                                         \
    } while (0)

#endif /* _LOGGING_H */