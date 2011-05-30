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

#ifndef GEOM_H
#define GEOM_H

#include <stdbool.h>
#include "bounding_boxes.h"

#define PI 3.1415926536
#define PI_HALF 1.570796327
#define SMALL_DELTA 0.0001
#define BIG_DELTA 0.1
#define SMALL_DELTA_ANGLE 0.00000001


double radToDeg(double x);
double degToRad(double x);

typedef struct {
    double x;
    double y;
    double z;
} point_t;

typedef struct {
    double l;
    double m;
    double n;
    double x0;
    double y0;
    double z0;
} line_t;

typedef point_t vector_t;

const static point_t ORIGIN = {0.0, 0.0, 0.0};
const static vector_t X_AXIS = {1.0, 0.0, 0.0};
const static vector_t Y_AXIS = {0.0, 1.0, 0.0};
const static vector_t Z_AXIS = {0.0, 0.0, 1.0};

point_t create_point(double x, double y, double z);
line_t create_line(double l, double m, double n, double x0, double y0, double z0);
vector_t create_vector(double x, double y, double z);

double distance(point_t p1, point_t p2);
point_t middle_point(point_t p1, point_t p2);
point_t translate(point_t point, point_t ref_point);
point_t rotate(point_t p, vector_t axis, double t);
point_t transform(double matrix[9], point_t point);
double point_line_dist(point_t p, line_t line);
double lines_min_dist(line_t line1, line_t line2, point_t* _p, point_t* _q);

double vector_magnitude(vector_t v);
vector_t vectors_sum(vector_t v1, vector_t v2);
vector_t vectors_diff(vector_t v1, vector_t v2);
vector_t scalar_vector_product(double r, vector_t v);
double dot_product(vector_t u, vector_t v);
vector_t cross_product(vector_t u, vector_t v);
vector_t normalize(vector_t v);
vector_t resize(vector_t vector, double new_magnitude);

void generate_transform_matrix(double matrix[9], vector_t vector_look, vector_t vector_up);
bool cull_object(double transform_matrix[9], double fovx, double fovy, double dist_threshold,
                 double object_radius, point_t observer_pos, point_t object_pos);
void generate_ellipsoid_points(point_t* ellipsoid_points, int ellipsoid_points_sqrt,
        double a, double b, double c);
bool object_sphere_collision(double transform_matrix[9],
        point_t object_pos, point_t sphere_pos,
        point_t* object_points, int n_object_points,
        double sphere_radius);
bool ellipsoid_small_sphere_collision(double transform_matrix[9],
        point_t ellipsoid_pos, point_t sphere_pos,
        double a, double b, double c,
        double sphere_radius);
bool object_small_sphere_collision(double transform_matrix[9],
        point_t object_pos, point_t sphere_pos,
        bounding_boxes_t* bounding_boxes);
vector_t calculate_shot_direction(point_t object_pos, vector_t object_direction, double object_speed,
        point_t shooter_pos, double shot_speed);

#endif
