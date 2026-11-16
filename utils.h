#ifndef UTILS_H
#define UTILS_H

#include <time.h>
#include <unistd.h>

static inline unsigned int lcg_rand(unsigned int *st) {
    *st = (*st * 1103515245 + 12345) & 0x7FFFFFFF;
    return *st;
}

static inline double get_ts() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec / 1e9;
}

static inline void w_str(const char *s) {
    int n = 0;
    while (s[n]) n++;
    write(1, s, n);
}

static inline void w_ll(long long v) {
    char b[32];
    int i = 30;
    b[31] = '\0';
    if (v == 0) { write(1, "0", 1); return; }
    int neg = 0;
    if (v < 0) { neg = 1; v = -v; }
    while (v > 0) {
        b[i--] = (v % 10) + '0';
        v /= 10;
    }
    if (neg) b[i--] = '-';
    write(1, &b[i + 1], 30 - i);
}

static inline void w_f(double v) {
    long long ip = (long long)v;
    w_ll(ip);
    w_str(".");
    double fp = v - ip;
    if (fp < 0) fp = -fp;
    int fi = (int)(fp * 10000);
    if (fi < 1000) w_str("0");
    if (fi < 100) w_str("0");
    if (fi < 10) w_str("0");
    w_ll(fi);
}

#endif