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
#include <math.h>
#include "geom.h"


inline double radToDeg(double x) {
    return x * 57.295779513;
}

inline double degToRad(double x) {
    return x / 57.295779513;
}


inline point_t create_point(double x, double y, double z) {
    point_t point = {x, y, z};
    return point;
}

inline line_t create_line(double l, double m, double n, double x0, double y0, double z0) {
    line_t line = {l, m, n, x0, y0, z0};
    return line;
}

inline vector_t create_vector(double x, double y, double z) {
    vector_t point = {x, y, z};
    return point;
}


inline double distance(point_t p1, point_t p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
}

inline point_t middle_point(point_t p1, point_t p2) {
    point_t result = create_point(
        (p1.x+p2.x) / 2.0,
        (p1.y+p2.y) / 2.0,
        (p1.z+p2.z) / 2.0
    );
    return result;
}

inline point_t translate(point_t point, point_t ref_point) {
    point_t result = create_point(
        point.x-ref_point.x, point.y-ref_point.y, point.z-ref_point.z);
    return result;
}

point_t rotate(point_t p, vector_t axis, double t) {
    axis = normalize(axis);
    double u = axis.x;
    double v = axis.y;
    double w = axis.z;
    double x = p.x;
    double y = p.y;
    double z = p.z;
    double sv = u*x+v*y+w*z;
    double q_x = u*sv + (x*(pow(v,2)+pow(w,2))-u*(v*y+w*z))*cos(t) + (-w*y+v*z)*sin(t);
    double q_y = v*sv + (y*(pow(u,2)+pow(w,2))-v*(u*x+w*z))*cos(t) + (w*x-u*z)*sin(t);
    double q_z = w*sv + (z*(pow(u,2)+pow(v,2))-w*(u*x+v*y))*cos(t) + (-v*x+u*y)*sin(t);
    point_t q = create_point(q_x, q_y, q_z);
    return q;
}

inline point_t transform(double matrix[9], point_t point) {
    double px = point.x;
    double py = point.y;
    double pz = point.z;
    double u = matrix[0]*px + matrix[1]*py + matrix[2]*pz;
    double v = matrix[3]*px + matrix[4]*py + matrix[5]*pz;
    double w = matrix[6]*px + matrix[7]*py + matrix[8]*pz;
    point_t result = create_vector(u, v, w);
    return result;
}

double point_line_dist(point_t p, line_t line) {

    double x0 = line.x0;
    double y0 = line.y0;
    double z0 = line.z0;
    double l = line.l;
    double m = line.m;
    double n = line.n;
    double xp = p.x;
    double yp = p.y;
    double zp = p.z;

    double t = (-l*x0 + l*xp - m*y0 + m*yp - n*z0 + n*zp)/(pow(l,2) + pow(m,2) + pow(n,2));

    double qx = x0 + l*t;
    double qy = y0 + m*t;
    double qz = z0 + n*t;

    point_t q = create_point(qx, qy, qz);

    return distance(p, q);

}

double lines_min_dist(line_t line1, line_t line2, point_t* _p, point_t* _q) {

    double x01 = line1.x0;
    double x02 = line2.x0;
    double y01 = line1.y0;
    double y02 = line2.y0;
    double z01 = line1.z0;
    double z02 = line2.z0;
    double l1 = line1.l;
    double l2 = line2.l;
    double m1 = line1.m;
    double m2 = line2.m;
    double n1 = line1.n;
    double n2 = line2.n;

    double t1 = (-l1*(pow(m2,2) + pow(n2,2))*(x01 - x02) - (m2*n1 - m1*n2)*(n2*(-y01 + y02) +
      m2*(z01 - z02)) + pow(l2,2)*(m1*(-y01 + y02) + n1*(-z01 + z02)) +
      l2*(m1*m2*(x01 - x02) + n1*n2*(x01 - x02) + l1*(m2*y01 - m2*y02 + n2*z01 - n2*z02))) /
      (pow(l2,2)*(pow(m1,2) + pow(n1,2)) + pow(m2*n1 - m1*n2, 2) -
       2*l1*l2*(m1*m2 + n1*n2) + pow(l1,2) * (pow(m2,2) + pow(n2,2)));
    double t2 = -((l1*l2 + m1*m2 + n1*n2)*(l1*(x01 - x02) + m1*(y01 - y02) +
       n1*(z01 - z02)) - (pow(l1,2) + pow(m1,2) + pow(n1,2))*(l2*(x01 - x02) +
       m2*(y01 - y02) + n2*(z01 - z02))) /
       (-pow(l1*l2 + m1*m2 + n1*n2, 2) + (pow(l1,2) + pow(m1,2) + pow(n1,2))*(pow(l2,2) + pow(m2,2) + pow(n2,2)));

    double p_x = x01 + l1 * t1;
    double p_y = y01 + m1 * t1;
    double p_z = z01 + n1 * t1;
    double q_x = x02 + l2 * t2;
    double q_y = y02 + m2 * t2;
    double q_z = z02 + n2 * t2;

    point_t p = create_point(p_x, p_y, p_z);
    point_t q = create_point(q_x, q_y, q_z);

    if (_p != NULL && _q != NULL) {
        *_p = p;
        *_q = q;
    }

    return distance(p, q);

}



inline double vector_magnitude(vector_t v) {
    return sqrt(pow(v.x,2)+pow(v.y,2)+pow(v.z,2));
}

vector_t vectors_sum(vector_t v1, vector_t v2) {
    double x = v1.x + v2.x;
    double y = v1.y + v2.y;
    double z = v1.z + v2.z;
    vector_t result = {x, y, z};
    return result;
}

vector_t vectors_diff(vector_t v1, vector_t v2) {
    double x = v1.x - v2.x;
    double y = v1.y - v2.y;
    double z = v1.z - v2.z;
    vector_t result = {x, y, z};
    return result;
}

vector_t scalar_vector_product(double r, vector_t v) {
    double x = r * v.x;
    double y = r * v.y;
    double z = r * v.z;
    vector_t result = create_vector(x, y, z);
    return result;
}

inline double dot_product(vector_t u, vector_t v) {
    return u.x*v.x + u.y*v.y + u.z*v.z;
}

vector_t cross_product(vector_t u, vector_t v) {
    double a1 = u.x;
    double a2 = u.y;
    double a3 = u.z;
    double b1 = v.x;
    double b2 = v.y;
    double b3 = v.z;
    double x = a2*b3 - a3*b2;
    double y = a3*b1 - a1*b3;
    double z = a1*b2 - a2*b1;
    vector_t result = create_vector(x, y, z);
    return result;
}

vector_t normalize(vector_t v) {
    double r = vector_magnitude(v);
    double x = v.x / r;
    double y = v.y / r;
    double z = v.z / r;
    vector_t result = create_vector(x, y, z);
    return result;
}

vector_t resize(vector_t vector, double new_magnitude) {
    vector_t norm = normalize(vector);
    vector_t result = scalar_vector_product(new_magnitude, norm);
    return result;
}



void generate_transform_matrix(double matrix[9], vector_t vector_look, vector_t vector_up) {
    vector_t vector_right = cross_product(vector_look, vector_up);
    double ux = vector_look.x;
    double uy = vector_look.y;
    double uz = vector_look.z;
    double vx = vector_up.x;
    double vy = vector_up.y;
    double vz = vector_up.z;
    double wx = vector_right.x;
    double wy = vector_right.y;
    double wz = vector_right.z;
    double denom = uz*vy*wx - uy*vz*wx - uz*vx*wy + ux*vz*wy + uy*vx*wz - ux*vy*wz;
    matrix[0] = (vz*wy - vy*wz) / denom;
    matrix[1] = (vz*wx - vx*wz) / -denom;
    matrix[2] = (vy*wx - vx*wy) / denom;
    matrix[3] = (uz*wy - uy*wz) / -denom;
    matrix[4] = (uz*wx - ux*wz) / denom;
    matrix[5] = (uy*wx - ux*wy) / -denom;
    matrix[6] = (uz*vy - uy*vz) / denom;
    matrix[7] = (uz*vx - ux*vz) / -denom;
    matrix[8] = (uy*vx - ux*vy) / denom;
}


bool cull_object(double transform_matrix[9], double fovx, double fovy, double dist_threshold,
                 double object_radius, point_t observer_pos, point_t object_pos) {

    // Test distance
    if (distance(observer_pos, object_pos) > dist_threshold) return true;

    point_t object_pos_transl = translate(object_pos, observer_pos);
    point_t rel_pos = transform(transform_matrix, object_pos_transl);

    double fovx_half = degToRad(fovx) / 2.0;
    double fovy_half = degToRad(fovy) / 2.0;

    // Basic test (object is behind)
    if (rel_pos.x < 0.0) return true;

    // Test fovx
    double c_pos_z;
    if (rel_pos.z > 0.0) {
        c_pos_z = rel_pos.z - object_radius;
        if (c_pos_z < 0.0) c_pos_z = 0.0;
    } else {
        c_pos_z = rel_pos.z + object_radius;
        if (c_pos_z > 0.0) c_pos_z = 0.0;
    }
    double r1 = sqrt(pow(rel_pos.x, 2) + pow(c_pos_z, 2));
    double alpha = acos(fabs(rel_pos.x) / r1);
    if (alpha > fovx_half) return true;

    // Test fovy
    double c_pos_y;
    if (rel_pos.y > 0.0) {
        c_pos_y = rel_pos.y - object_radius;
        if (c_pos_y < 0.0) c_pos_y = 0.0;
    } else {
        c_pos_y = rel_pos.y + object_radius;
        if (c_pos_y > 0.0) c_pos_y = 0.0;
    }
    double r2 = sqrt(pow(rel_pos.x, 2) + pow(c_pos_y, 2));
    double beta = acos(fabs(rel_pos.x) / r2);
    if (beta > fovy_half) return true;

    return false;

}


void generate_ellipsoid_points(point_t* ellipsoid_points, int ellipsoid_points_sqrt,
                               double a, double b, double c) {

    const double theta_increment = PI / (double) (ellipsoid_points_sqrt-1);
    const double phi_increment = 2*PI / (double) (ellipsoid_points_sqrt-1);
    const int total_points = ellipsoid_points_sqrt*ellipsoid_points_sqrt;
    const double theta_max = PI + SMALL_DELTA_ANGLE;
    const double phi_max = 2*PI + SMALL_DELTA_ANGLE;

    double phi, theta;

    int i = 0;
    for (phi = 0.0; phi < phi_max; phi += phi_increment) {
        for (theta = 0.0; theta < theta_max; theta += theta_increment) {
            double x = a * sin(theta) * cos(phi);
            double y = b * sin(theta) * sin(phi);
            double z = c * cos(theta);
            point_t p = create_point(x, y, z);
            ellipsoid_points[i++] = p;
            if (i == total_points) return;
        }
    }

}

bool object_sphere_collision(double transform_matrix[9],
                            point_t object_pos, point_t sphere_pos,
                            point_t* object_points, int n_object_points,
                            double sphere_radius) {

    // Translate sphere
    const point_t sphere_pos_transl = translate(sphere_pos, object_pos);

    // Apply transformation to sphere position
    const point_t sphere_pos_transf = transform(transform_matrix, sphere_pos_transl);

    for (int i = 0; i < n_object_points; i++) {
        point_t p = object_points[i];
        if (distance(p, sphere_pos_transf) < sphere_radius) {
            return true;
        }
    }

    return false;

}

bool ellipsoid_small_sphere_collision(double transform_matrix[9],
                                      point_t ellipsoid_pos, point_t sphere_pos,
                                      double a, double b, double c,
                                      double sphere_radius) {

    // Translate sphere
    point_t sphere_pos_transl = translate(sphere_pos, ellipsoid_pos);

    // Apply transformation to sphere position
    point_t sphere_pos_transf = transform(transform_matrix, sphere_pos_transl);

    if (distance(sphere_pos_transf, ORIGIN) < BIG_DELTA) return true;

    // Find parameters (ru, rv, rw) of the vector from origin to sphere_pos_transf
    double ru = sphere_pos_transf.x;
    double rv = sphere_pos_transf.y;
    double rw = sphere_pos_transf.z;

    // Find the intersection between the ellipsoid and the line of (ru, rv, rw)
    double lambda = 1.0 / sqrt(pow(ru/a,2) + pow(rv/b,2) + pow(rw/c,2));
    double pu = lambda*ru;
    double pv = lambda*rv;
    double pw = lambda*rw;

    // Found intersection point
    point_t p = create_point(pu, pv, pw);

    // Check distance
    double dist = distance(ORIGIN, sphere_pos_transf) - distance(ORIGIN, p);
    if (dist < sphere_radius) return true;

    return false;

}

bool object_small_sphere_collision(double transform_matrix[9],
        point_t object_pos, point_t sphere_pos,
        bounding_boxes_t* bounding_boxes) {

    // Translate sphere
    point_t sphere_pos_transl = translate(sphere_pos, object_pos);

    // Apply transformation to sphere position
    point_t sphere_pos_transf = transform(transform_matrix, sphere_pos_transl);

    for (int i = 0; i < bounding_boxes->n_boxes; i++) {
        bounding_box_t* box = &bounding_boxes->array[i];
        if (sphere_pos_transf.x > box->x_min && sphere_pos_transf.x < box->x_max &&
            sphere_pos_transf.y > box->y_min && sphere_pos_transf.y < box->y_max &&
            sphere_pos_transf.z > box->z_min && sphere_pos_transf.z < box->z_max)
            return true;
    }

    return false;

}

vector_t calculate_shot_direction(point_t object_pos, vector_t object_direction, double object_speed,
                                  point_t shooter_pos, double shot_speed) {

    double vx = object_direction.x * object_speed;
    double vy = object_direction.y * object_speed;
    double vz = object_direction.z * object_speed;
    double a = object_pos.x-shooter_pos.x;
    double b = object_pos.y-shooter_pos.y;
    double c = object_pos.z-shooter_pos.z;
    double k = shot_speed;

    double radix = sqrt(4*pow(a*vx + b*vy + c*vz, 2) -
                        4*(pow(a,2) + pow(b,2) + pow(c,2)) *
                        (-pow(k,2) + pow(vx,2) + pow(vy,2) + pow(vz,2)));
    double denom = a*vx + b*vy + c*vz + 0.5*radix;

    double sx = (a*pow(k,2) + b*vx*vy - a*pow(vy,2) + c*vx*vz - a*pow(vz,2) + 0.5*vx*radix) / denom;
    double sy = (b*pow(k,2) - b*pow(vx,2) + a*vx*vy + c*vy*vz - b*pow(vz,2) + 0.5*vy*radix) / denom;
    double sz = (c*pow(k,2) - c*pow(vx,2) - c*pow(vy,2) + a*vx*vz + b*vy*vz + 0.5*vz*radix) / denom;

    vector_t s = create_vector(sx, sy, sz);
    vector_t shot_direction = normalize(s);

    return shot_direction;

}
