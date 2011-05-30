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

#ifndef BOUNDING_BOXES_H
#define BOUNDING_BOXES_H

#include <stdbool.h>

typedef struct {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double z_min;
    double z_max;
} bounding_box_t;

typedef struct {
    int n_boxes;
    bounding_box_t* array;
} bounding_boxes_t;

bounding_boxes_t boundingboxes_load(const char* filename, bool swapYZ);
void boundingboxes_destroy(bounding_boxes_t* boxes);

#endif
