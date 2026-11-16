#define _POSIX_C_SOURCE 200809L
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "utils.h"

typedef struct {
    int id;
    long long cnt;
    double r;
} Task;

long long g_hits = 0;
pthread_mutex_t mtx;

void* worker(void *arg) {
    Task *t = (Task*)arg;
    double r2 = t->r * t->r;
    long long lh = 0;
    unsigned int st = 54321 + t->id * 1337;

    for (long long i = 0; i < t->cnt; i++) {
        double x = ((double)lcg_rand(&st) / 0x7FFFFFFF) * 2.0 * t->r - t->r;
        double y = ((double)lcg_rand(&st) / 0x7FFFFFFF) * 2.0 * t->r - t->r;
        if (x * x + y * y <= r2) {
            lh++;
        }
    }

    pthread_mutex_lock(&mtx);
    g_hits += lh;
    pthread_mutex_unlock(&mtx);

    return NULL;
}

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
    int thr = 1;

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'r') {
            rad = (double)p_ll(argv[++i]);
        } else if (argv[i][0] == '-' && argv[i][1] == 'n') {
            pts = p_ll(argv[++i]);
        } else if (argv[i][0] == '-' && argv[i][1] == 't') {
            thr = (int)p_ll(argv[++i]);
        }
    }

    w_str("PAR MODE\n");
    w_str("T: "); w_ll(thr); w_str("\n");

    pthread_t *tids = malloc(thr * sizeof(pthread_t));
    Task *tasks = malloc(thr * sizeof(Task));
    
    pthread_mutex_init(&mtx, NULL);

    long long base = pts / thr;
    long long rem = pts % thr;

    double t0 = get_ts();

    for (int i = 0; i < thr; i++) {
        tasks[i].id = i;
        tasks[i].r = rad;
        tasks[i].cnt = base + (i == thr - 1 ? rem : 0);

        pthread_create(&tids[i], NULL, worker, &tasks[i]);
    }

    for (int i = 0; i < thr; i++) {
        pthread_join(tids[i], NULL);
    }

    double t1 = get_ts();

    double sq = 4.0 * rad * rad;
    double res = sq * ((double)g_hits / pts);

    w_str("T: "); w_f(t1 - t0); w_str(" s\n");
    w_str("S: "); w_f(res); w_str("\n");

    free(tids);
    free(tasks);
    pthread_mutex_destroy(&mtx);

    return 0;
}