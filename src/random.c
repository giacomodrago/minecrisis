/*
 * MineCrisis
 * Copyright (C) 2010-2011 Giacomo Drago
 * http://giacomodrago.com/go/minecrisis
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

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
