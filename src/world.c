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
#include <string.h>
#include "world.h"

inline checkpoint_t createCheckpoint(double pos_x, double pos_y, double pos_z) {
    point_t point = create_point(pos_x, pos_y, pos_z);
    checkpoint_t checkpoint = {point, CHECKPOINT_STATUS_UNREACHED};
    return checkpoint;
}

inline mine_t createMine(double pos_x, double pos_y, double pos_z) {
    point_t point = create_point(pos_x, pos_y, pos_z);
    mine_t mine = {point, MINE_STATUS_IDLE};
    return mine;
}

inline turret_t createTurret(double pos_x, double pos_y, double pos_z) {
    point_t point = create_point(pos_x, pos_y, pos_z);
    turret_t turret = {point, TURRET_STATUS_IDLE, 0};
    return turret;
}

shot_t createShot(point_t starting_point, vector_t direction, timestamp_t timestamp) {
    point_t position, prev_position;
    position = prev_position = starting_point;
    line_t line = create_line(direction.x, direction.y, direction.z,
                              starting_point.x, starting_point.y, starting_point.z);
    shot_t shot = {starting_point, position, prev_position, direction, line, timestamp, false, false};
    return shot;
}

world_t createWorld(double width, double height, double depth,
    int n_checkpoints, int n_mines, int n_turrets) {
    world_t world;
    world.width = width;
    world.height = height;
    world.depth = depth;
    world.n_checkpoints = n_checkpoints;
    world.n_mines = n_mines;
    world.n_turrets = n_turrets;
    world.checkpoints = (checkpoint_t*) malloc(n_checkpoints * sizeof(checkpoint_t));
    world.mines = (mine_t*) malloc(n_mines * sizeof(mine_t));
    world.turrets = (turret_t*) malloc(n_turrets * sizeof(turret_t));
    return world;
}

void destroyWorld(world_t* world) {
    free(world->checkpoints);
    free(world->mines);
    free(world->turrets);
}

int inWorld(point_t p, world_t* world) {
    double x_min = -world->width / 2.0;
    double x_max = world->width / 2.0;
    double y_min = -world->height / 2.0;
    double y_max = world->height / 2.0;
    double z_min = -world->depth / 2.0;
    double z_max = world->depth / 2.0;
    int in_width = (p.x > x_min && p.x < x_max);
    int in_height = (p.y > y_min && p.y < y_max);
    int in_depth = (p.z > z_min && p.z < z_max);
    int result = in_width && in_height && in_depth;
    return result;
}


#define HEADER 0
#define BEFORE_CHECKPOINTS 1
#define CHECKPOINTS 2
#define BEFORE_MINES 3
#define MINES 4
#define BEFORE_TURRETS 5
#define TURRETS 6
#define END 7

static inline void _badFile(int err_no) {
    fprintf(stderr, "Bad input file (err_no: %d)", err_no);
    exit(1);
}

world_t loadWorld(const char* file_path) {

    FILE* file = fopen(file_path, "r");

    if (file == NULL) _badFile(1);

    int n_mines, n_checkpoints, n_turrets;

    char line[256];
    int status = HEADER;
    int i = 0;

    world_t world = {0}; // to skip the compiler warning
    double pos_x, pos_y, pos_z;

    while (fgets(line, sizeof(line), file) != NULL) {

        stripnl(line);
        if (strlen(line) == 0) { // empty line
            continue;
        }

        switch (status) {
            case HEADER: {
                double width, height, depth;
                if (sscanf(line, "%lf %lf %lf %d %d %d",
                    &width, &height, &depth, &n_checkpoints, &n_mines, &n_turrets) > 0) {
                    world = createWorld(width, height, depth, n_checkpoints, n_mines, n_turrets);
                    status = BEFORE_CHECKPOINTS;
                } else {
                    _badFile(2);
                }
                break;
            }
            case BEFORE_CHECKPOINTS: {
                if (strcmp(line, "checkpoints") == 0) {
                    status = CHECKPOINTS;
                    i = 0;
                } else {
                    _badFile(3);
                }
                break;
            }
            case CHECKPOINTS: {
                if (sscanf(line, "%lf %lf %lf", &pos_x, &pos_y, &pos_z) > 0) {
                    world.checkpoints[i] = createCheckpoint(pos_x, pos_y, pos_z);
                    i++;
                    if (i == n_checkpoints) status = BEFORE_MINES;
                } else {
                    _badFile(4);
                }
                break;
            }
            case BEFORE_MINES: {
                if (strcmp(line, "mines") == 0) {
                    status = MINES;
                    i = 0;
                } else {
                    _badFile(5);
                }
                break;
            }
            case MINES: {
                if (sscanf(line, "%lf %lf %lf", &pos_x, &pos_y, &pos_z) > 0) {
                    world.mines[i] = createMine(pos_x, pos_y, pos_z);
                    i++;
                    if (i == n_mines) status = BEFORE_TURRETS;
                } else {
                    _badFile(6);
                }
                break;
            }
            case BEFORE_TURRETS: {
                if (strcmp(line, "turrets") == 0) {
                    status = TURRETS;
                    i = 0;
                } else {
                    _badFile(7);
                }
                break;
            }
            case TURRETS: {
                if (sscanf(line, "%lf %lf %lf", &pos_x, &pos_y, &pos_z) > 0) {
                    world.turrets[i] = createTurret(pos_x, pos_y, pos_z);
                    i++;
                    if (i == n_turrets) status = END;
                } else {
                    _badFile(8);
                }
                break;
            }
            default:
                break;
        }

    }
    if (status != END) _badFile(9);

    return world;
}

#undef HEADER
#undef BEFORE_CHECKPOINTS
#undef CHECKPOINTS
#undef BEFORE_MINES
#undef MINES
#undef BEFORE_TURRETS
#undef TURRETS
#undef END
