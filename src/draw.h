#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include "opengl.h"
#include "world.h"
#include "ase_importer.h"
#include "bounding_boxes.h"
#include "texture.h"
#include "explosion.h"

#define MINE_DETAIL_0 10
#define MINE_DETAIL_1 20
#define MINE_DETAIL_2 40
#define MINE_DETAIL_3 100

#define TURRET_DETAIL_0 10
#define TURRET_DETAIL_1 40
#define TURRET_DETAIL_2 80
#define TURRET_DETAIL_3 120

#define CHECKPOINT_DETAIL_0 25
#define CHECKPOINT_DETAIL_1 50
#define CHECKPOINT_DETAIL_2 150

#define STARMAP_DETAIL 50
#define STARMAP_RADIUS 1.0
#define PLANET_OFFSET 15.0
#define PLANET_RADIUS 14.0
#define PLANET_DETAIL 400

#define SHOT_DETAIL 15

#define SHIELD_DETAIL 100
#define SHIELD_MAX_ALPHA 0.45


// materials
const static float starmap_color[] = { 0.5, 0.5, 0.5, 1.0 };
const static float planet_color[] = { 1.0, 1.0, 1.0, 1.0 };
const static float checkpoint_color[] = { 1.0, 1.0, 0.0, 1.0 };
const static float mine_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
const static float mine_specular[] = { 1.0, 1.0, 1.0, 1.0 };
const static int mine_shininess = 12;
const static float turret_color[] = { 0.1, 0.1, 0.1, 1.0 };
const static float turret_specular[] = { 1.0, 1.0, 1.0, 1.0 };
const static int turret_shininess = 12;
const static float shot_color[] = { 0.0, 1.0, 0.0, 1.0 };
const static float ship_body_color[] = { 0.7, 0.7, 0.7, 1.0 };
const static float ship_glass_color[] = { 0.1, 0.1, 0.1, 1.0 };
const static float ship_decorations_color[] = { 0.9, 0.1, 0.0, 1.0 };
const static float ship_glow_color[] = { 0.6, 0.85, 1.0, 1.0 };
const static float ship_shield_color[] = { 1.0, 0.0, 1.0, 1.0 };
const static float white[] = { 1.0, 1.0, 1.0, 1.0 };
const static float no_color[] = { 0.0, 0.0, 0.0, 1.0 };


void drawAxes(double x_len, double y_len, double z_len);
void drawWorldBox(double width, double height, double depth);
void drawSegment(point_t p1, point_t p2);
void drawStarmap(texture_t texture_id);
void drawPlanet(texture_t texture_id);
void drawRotatedPlanet(display_list_t display_list, double planet_rot_x, double planet_rot_y);
void drawCheckpointSphere(int detail);
void drawCheckpoint(display_list_t display_list, double alpha);
void drawMine(int detail);
void drawTurret(int detail);
void getShipTransformationMatrix(double transform_matrix[9], vector_t position,
                                 vector_t vector_look, vector_t vector_up);
void drawShip(display_list_t ship_body,
              display_list_t ship_weapons,
              display_list_t ship_glass,
              display_list_t ship_decorations,
              display_list_t ship_glow,
              vector_t position,
              vector_t vector_look,
              vector_t vector_up,
              double inclination,
              double throttle,
              double opacity,
              bool draw_bounding_boxes,
              bounding_boxes_t* boxes);
void drawShipShield(vector_t position, vector_t vector_look, vector_t vector_up,
                    double inclination, double intensity);
void drawShipShieldPoints(point_t* points, int n_points,
                          vector_t position, vector_t vector_look, vector_t vector_up,
                          double inclination);
void drawShot();
void drawExplosion(explosion_t* explosion, vector_t position, vector_t vector_look, vector_t vector_up);
void draw2DWireRectangle(int left, int top, int width, int height, const float color[4], float line_width);
void draw2DSolidRectangle(int left, int top, int width, int height, const float color[4]);
void draw2DTexturedRectangle(int left, int top, int width, int height, const float color[4], texture_t texture_id,
                           double tcoord_u_left, double tcoord_u_right,
                           double tcoord_v_bottom, double tcoord_v_top);
void lookAt(vector_t position, vector_t vector_look, vector_t vector_up);


#endif
