#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include "utils.h"

long long p_ll(const char *s) {
    long long r = 0;
    int sg = 1;
    if (*s == '-') { sg = -1; s++; }
    while (*s >= '0' && *s <= '9') {
        r = r * 10 + (*s - '0');
        s++;
    }
    return r * sg;
}

int main(int argc, char **argv) {
    double rad = 1.0;
    long long pts = 1000000;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'r') {
            rad = (double)p_ll(argv[++i]);
        } else if (argv[i][0] == '-' && argv[i][1] == 'n') {
            pts = p_ll(argv[++i]);
        }
    }

    w_str("SEQ MODE\n");
    w_str("R: "); w_ll((long long)rad); w_str("\n");
    w_str("N: "); w_ll(pts); w_str("\n");

    unsigned int st = 54321;
    double r2 = rad * rad;
    long long h = 0;

    double t0 = get_ts();

    for (long long i = 0; i < pts; i++) {
        double x = ((double)lcg_rand(&st) / 0x7FFFFFFF) * 2.0 * rad - rad;
        double y = ((double)lcg_rand(&st) / 0x7FFFFFFF) * 2.0 * rad - rad;
        if (x * x + y * y <= r2) {
            h++;
        }
    }

    double t1 = get_ts();

    double sq = 4.0 * rad * rad;
    double res = sq * ((double)h / pts);

    w_str("T: "); w_f(t1 - t0); w_str(" s\n");
    w_str("S: "); w_f(res); w_str("\n");

    return 0;
}