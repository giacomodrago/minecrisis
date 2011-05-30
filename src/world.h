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

#ifndef WORLD_H
#define WORLD_H

#include "utils.h"
#include "geom.h"

#define MINE_RADIUS 5.0
#define CHECKPOINT_RADIUS 20.0
#define TURRET_RADIUS 5.0
#define SHOT_RADIUS 0.15
#define SHIP_RADIUS_MAX 2.5
#define SHIP_RADIUS_X 1.59
#define SHIP_RADIUS_Y 0.51
#define SHIP_RADIUS_Z 2.5

#define MINE_STATUS_IDLE 0
#define MINE_STATUS_EXPLODED 1
#define CHECKPOINT_STATUS_UNREACHED 0
#define CHECKPOINT_STATUS_REACHED 1
#define TURRET_STATUS_IDLE 0
#define TURRET_STATUS_ENGAGED 1


typedef struct {
    point_t pos;
    int status;
} checkpoint_t;

typedef struct {
    point_t pos;
    int status;
} mine_t;

typedef struct {
    point_t pos;
    int status;
    timestamp_t fire_timestamp;
} turret_t;

typedef struct {
    double width;
    double height;
    double depth;
    int n_checkpoints;
    int n_mines;
    int n_turrets;
    checkpoint_t* checkpoints;
    mine_t* mines;
    turret_t* turrets;
} world_t;

typedef struct {
    point_t starting_point;
    point_t position;
    point_t prev_position;
    vector_t direction;
    line_t line;
    timestamp_t timestamp;
    bool pending_remove;
    bool remove;
} shot_t;



checkpoint_t createCheckpoint(double pos_x, double pos_y, double pos_z);
mine_t createMine(double pos_x, double pos_y, double pos_z);
turret_t createTurret(double pos_x, double pos_y, double pos_z);
shot_t createShot(point_t starting_point, vector_t direction, timestamp_t timestamp);
world_t createWorld(double width, double height, double depth,
    int n_checkpoints, int n_mines, int n_turrets);

void destroyWorld(world_t* world);
int inWorld(point_t p, world_t* world);
world_t loadWorld(const char* file_path);

#endif
