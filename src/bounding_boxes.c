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
#include <stdio.h>
#include "bounding_boxes.h"

bounding_boxes_t boundingboxes_load(const char* filename, bool swapYZ) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    int n_boxes;
    if (fscanf(file, "%d", &n_boxes) <= 0) {
        fprintf(stderr, "Error reading '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    bounding_box_t* array = (bounding_box_t*) malloc(n_boxes * sizeof(bounding_box_t));
    bounding_boxes_t result = {n_boxes, array};
    for (int i = 0; i < n_boxes; i++) {
        double x_min, x_max, y_min, y_max, z_min, z_max;
        if (fscanf(file, "%lf %lf %lf %lf %lf %lf",
                   &x_min, &x_max, &y_min, &y_max, &z_min, &z_max) <= 0) {
            fprintf(stderr, "Error reading '%s'\n", filename);
            exit(EXIT_FAILURE);
        }
        if (swapYZ) {
            double temp1 = y_min;
            double temp2 = y_max;
            y_min = z_min;
            y_max = z_max;
            z_min = temp1;
            z_max = temp2;
        }
        bounding_box_t curr_box = {x_min, x_max, y_min, y_max, z_min, z_max};
        array[i] = curr_box;
    }
    fclose(file);
    return result;
}

void boundingboxes_destroy(bounding_boxes_t* boxes) {
    free(boxes->array);
}

