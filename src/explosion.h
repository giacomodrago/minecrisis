#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "geom.h"
#include "utils.h"

#define EXPL_ELLIPSOID_A 2.5
#define EXPL_ELLIPSOID_B 0.5
#define EXPL_ELLIPSOID_C 1.5

#define PHASE_1_THRESHOLD 0.030
#define PHASE_2_THRESHOLD 0.120
#define SPEED_FACTOR 20.0
#define COLOR_SPEED_FACTOR 0.08
#define ROT_SPEED_FACTOR 700.0
#define TRIANGLE_SCALE_MIN 0.2
#define TRIANGLE_SCALE_MAX 0.7
#define POINTS_SPREAD 0.025

static const float point_color[] = {1.0, 1.0, 0.5, 1.0};
static const float triangle_color_front[] = {0.9, 0.9, 0.9, 1.0};
static const float triangle_color_back[] = {0.5, 0.5, 0.5, 1.0};

typedef struct {
    point_t position;
    vector_t speed;
} expl_point_t;

typedef struct {
    point_t position;
    vector_t speed;
    vector_t orientation;
    vector_t orientation_speed;
    vector_t scale;
} expl_triangle_t;


typedef struct {
    int n_points, n_triangles;
    expl_point_t* points;
    expl_triangle_t* triangles;
    timestamp_t clock;
    float curr_point_color[4];
    int phase;
} explosion_t;

explosion_t* explosion_create(int n_points, int n_triangles);
void explosion_reset(explosion_t* explosion);
void explosion_destroy(explosion_t* explosion);
void explosion_draw(explosion_t* explosion);
void explosion_update(explosion_t* explosion, timestamp_t delta_t_us);

#endif
