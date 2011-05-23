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
