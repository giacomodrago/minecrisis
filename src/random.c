#include <stdlib.h>
#include <time.h>
#include "random.h"

inline void rand_seed() {
    srand((unsigned)time(0));
}

inline double rand_gen_range(double min, double max) {
    double diff = max-min;
    double rnd = ((double)rand() / ((double)RAND_MAX + 1)) * diff + min;
    return rnd;
}

inline double rand_gen() {
    return (double)rand() / ((double)RAND_MAX + 1);
}

inline int rand_gen_int(int min, int max) {
    int diff = 1+max-min;
    int rnd = (int) ((((double)rand() / ((double)RAND_MAX + 1)) * diff) + min);
    return rnd;
}
