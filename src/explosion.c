#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "opengl.h"
#include "explosion.h"
#include "random.h"


static inline vector_t _randVector(bool ellipsoid, bool circle) {

    double phi, theta;

    phi = rand_gen_range(0.0, 2.0*PI);
    if (!circle) theta = rand_gen_range(0.0, PI);
    else theta = rand_gen_range(PI_HALF-POINTS_SPREAD, PI_HALF+POINTS_SPREAD);

    double x, y, z;

    if (ellipsoid) {
        x = EXPL_ELLIPSOID_A * sin(theta) * cos(phi);
        y = EXPL_ELLIPSOID_B * cos(theta);
        z = EXPL_ELLIPSOID_C * sin(theta) * sin(phi);
    } else { // sphere
        x = sin(theta) * cos(phi);
        y = cos(theta);
        z = sin(theta) * sin(phi);
    }

    double magnitude = sqrt(x * x + y * y + z * z);

    if (ellipsoid) {
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    } else { // sphere
        x = rand_gen_range(0.5, 1.5) * x / magnitude;
        y = rand_gen_range(0.5, 1.5) * y / magnitude;
        z = rand_gen_range(0.5, 1.5) * z / magnitude;
    }

    vector_t result = create_vector(x, y, z);

    return result;

}


explosion_t* explosion_create(int n_points, int n_triangles) {

    rand_seed();

    explosion_t* explosion = (explosion_t*) malloc(sizeof(explosion_t));
    explosion->n_points = n_points;
    explosion->n_triangles = n_triangles;
    explosion->points = (expl_point_t*) malloc(n_points * sizeof(expl_point_t));
    explosion->triangles = (expl_triangle_t*) malloc(n_triangles * sizeof(expl_triangle_t));
    explosion->clock = 0;
    explosion->phase = 0;

    for (int i = 0; i < n_points; i++) {
        vector_t speed = _randVector(false, true);
        expl_point_t point;
        point.position = ORIGIN;
        point.speed = speed;
        explosion->points[i] = point;
    }

    memcpy(explosion->curr_point_color, point_color, sizeof(point_color));

    for (int i = 0; i < n_triangles; i++) {

        vector_t scale;
        scale.x = rand_gen_range(TRIANGLE_SCALE_MIN, TRIANGLE_SCALE_MAX);
        scale.y = rand_gen_range(TRIANGLE_SCALE_MIN, TRIANGLE_SCALE_MAX);
        scale.z = rand_gen_range(TRIANGLE_SCALE_MIN, TRIANGLE_SCALE_MAX);

        vector_t orientation;
        orientation.x = rand_gen_range(0.0, 180.0);
        orientation.y = rand_gen_range(0.0, 180.0);
        orientation.z = rand_gen_range(0.0, 180.0);

        bool ellipsoid = true;
        if (i > n_triangles / 2) ellipsoid = false;

        vector_t speed = _randVector(ellipsoid, false);
        vector_t orientation_speed = _randVector(false, false);

        expl_triangle_t triangle;
        triangle.position = ORIGIN;
        triangle.speed = speed;
        triangle.orientation = orientation;
        triangle.orientation_speed = orientation_speed;
        triangle.scale = scale;

        explosion->triangles[i] = triangle;

    }

    return explosion;

}

void explosion_reset(explosion_t* explosion) {
    explosion->clock = 0;
    explosion->phase = 0;
    for (int i = 0; i < explosion->n_points; i++) {
        explosion->points[i].position = ORIGIN;
    }
    memcpy(explosion->curr_point_color, point_color, sizeof(point_color));
    for (int i = 0; i < explosion->n_triangles; i++) {
        explosion->triangles[i].position = ORIGIN;
    }
}


void explosion_destroy(explosion_t* explosion) {
    free(explosion->points);
    free(explosion->triangles);
    free(explosion);
}


void explosion_draw(explosion_t* explosion) {

    glDisable(GL_DEPTH_TEST);
    glPointSize(1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < explosion->n_points; i++) {
        point_t position = explosion->points[i].position;
        glColor4fv(explosion->curr_point_color);
        glVertex3d(position.x, position.y, position.z);
    }
    glEnd();
    glEnable(GL_DEPTH_TEST);

    int max_triangles;
    if (explosion->phase < 2) max_triangles = explosion->n_triangles / 2;
    else max_triangles = explosion->n_triangles;

    for (int i = 0; i < max_triangles; i++) {

        point_t position = explosion->triangles[i].position;
        vector_t orientation = explosion->triangles[i].orientation;
        vector_t scale = explosion->triangles[i].scale;

        glPushMatrix();

            glTranslatef(position.x, position.y, position.z);

            glRotatef(orientation.x, 1.0, 0.0, 0.0);
            glRotatef(orientation.y, 0.0, 1.0, 0.0);
            glRotatef(orientation.z, 0.0, 0.0, 1.0);

            glScalef(scale.x, scale.y, scale.z);

            glColor4fv(triangle_color_front);
            glBegin(GL_TRIANGLES);
                glVertex3f(0.0, 0.3, 0.0);
                glVertex3f(-0.15, 0.0, 0.0);
                glVertex3f(0.15, 0.0, 0.0);
            glEnd();
            glColor4fv(triangle_color_back);
            glBegin(GL_TRIANGLES);
                glVertex3f(0.15, 0.0, 0.0);
                glVertex3f(-0.15, 0.0, 0.0);
                glVertex3f(0.0, 0.3, 0.0);
            glEnd();

        glPopMatrix();

    }

}


void explosion_update(explosion_t* explosion, timestamp_t delta_t_us) {

    explosion->clock += delta_t_us;
    double delta_t = usToSecs(delta_t_us);
    if (usToSecs(explosion->clock) > PHASE_2_THRESHOLD) explosion->phase = 2;
    else if (usToSecs(explosion->clock) > PHASE_1_THRESHOLD) explosion->phase = 1;

    for (int i = 0; i < explosion->n_points; i++) {

        expl_point_t* point = &explosion->points[i];

        point->position.x += point->speed.x * delta_t * SPEED_FACTOR;
        point->position.y += point->speed.y * delta_t * SPEED_FACTOR;
        point->position.z += point->speed.z * delta_t * SPEED_FACTOR;

    }

    explosion->curr_point_color[0] -= COLOR_SPEED_FACTOR * delta_t;
    explosion->curr_point_color[1] -= COLOR_SPEED_FACTOR * delta_t;
    explosion->curr_point_color[2] -= COLOR_SPEED_FACTOR * delta_t;
    explosion->curr_point_color[3] -= COLOR_SPEED_FACTOR * delta_t;
    if (explosion->curr_point_color[0] < 0.0) explosion->curr_point_color[0] = 0.0;
    if (explosion->curr_point_color[1] < 0.0) explosion->curr_point_color[1] = 0.0;
    if (explosion->curr_point_color[2] < 0.0) explosion->curr_point_color[2] = 0.0;
    if (explosion->curr_point_color[3] < 0.0) explosion->curr_point_color[3] = 0.0;

    int max_triangles;
    if (explosion->phase < 2) max_triangles = explosion->n_triangles/2;
    else max_triangles = explosion->n_triangles;

    for (int i = 0; i < max_triangles; i++) {

        expl_triangle_t* triangle = &explosion->triangles[i];

        triangle->position.x += triangle->speed.x * delta_t * SPEED_FACTOR;
        triangle->position.y += triangle->speed.y * delta_t * SPEED_FACTOR;
        triangle->position.z += triangle->speed.z * delta_t * SPEED_FACTOR;

        triangle->orientation.x += triangle->orientation_speed.x * delta_t * ROT_SPEED_FACTOR;
        triangle->orientation.y += triangle->orientation_speed.y * delta_t * ROT_SPEED_FACTOR;
        triangle->orientation.z += triangle->orientation_speed.z * delta_t * ROT_SPEED_FACTOR;

    }

}

