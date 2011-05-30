/*
 * MineCrisis World Generator
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

#include "geom.h"
#include "random.h"
#include "utils.h"
#include "world.h"

#define NUM_PARAMETERS 7
#define MIN_CHECKPOINT_DIST 50.0
#define TURRET_CHECKPOINT_RANGE 400.0

void generateMines(world_t* world) {

    double x_min = -world->width / 2.0;
    double x_max = world->width / 2.0;
    double y_min = -world->height / 2.0;
    double y_max = world->height / 2.0;
    double z_min = -world->depth / 2.0;
    double z_max = world->depth / 2.0;

    rand_seed();

    line_t safe_line = create_line(1.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    printf("\n\nGenerating mines...\n");

    for (int i = 0; i < world->n_mines; i++) {

        printf("\t%d of %d... ", i+1, world->n_mines);

        bool success = false;
        int attempts = 0;

        double pos_x, pos_y, pos_z;

        while (!success) {
            pos_x = rand_gen_range(x_min, x_max);
            pos_y = rand_gen_range(y_min, y_max);
            pos_z = rand_gen_range(z_min, z_max);
            point_t pos_gen = create_point(pos_x, pos_y, pos_z);
            double dist_safe = point_line_dist(pos_gen, safe_line);
            success = true;
            if (dist_safe < MINE_RADIUS*2) success = false;
            if (success) {
                for (int j = 0; j < i; j++) {
                    mine_t* mine_curr = &world->mines[j];
                    if (distance(pos_gen, mine_curr->pos) < MINE_RADIUS*2) {
                        success = false;
                        break;
                    }
                }
            }
            attempts++;
        }

        world->mines[i] = createMine(pos_x, pos_y, pos_z);
        printf("done with %d attempts.\n", attempts);

    }

}

void generateCheckpoints(world_t* world) {

    double x_min = -world->width / 2.0;
    double x_max = world->width / 2.0;
    double y_min = -world->height / 2.0;
    double y_max = world->height / 2.0;
    double z_min = -world->depth / 2.0;
    double z_max = world->depth / 2.0;

    rand_seed();

    printf("\n\nGenerating checkpoints...\n");

    for (int i = 0; i < world->n_checkpoints; i++) {
        printf("\t%d of %d... ", i+1, world->n_checkpoints);
        bool success = false;

        int attempts = 0;
        double pos_x, pos_y, pos_z;
        while (!success) {
            pos_x = rand_gen_range(x_min, x_max);
            pos_y = rand_gen_range(y_min, y_max);
            pos_z = rand_gen_range(z_min, z_max);
            point_t pos_gen = create_point(pos_x, pos_y, pos_z);
            success = true;
            for (int j = 0; j < world->n_mines; j++) {
                mine_t* mine_curr = &world->mines[j];
                if (distance(pos_gen, mine_curr->pos) < CHECKPOINT_RADIUS+MINE_RADIUS) {
                    success = false;
                    break;
                }
            }
            if (success) {
                for (int j = 0; j < i; j++) {
                    checkpoint_t* checkpoint_curr = &world->checkpoints[j];
                    if (distance(pos_gen, checkpoint_curr->pos) < CHECKPOINT_RADIUS*2) {
                        success = false;
                        break;
                    }
                }
            }
            attempts++;
        }
        world->checkpoints[i] = createCheckpoint(pos_x, pos_y, pos_z);
        printf("done with %d attempts.\n", attempts);
    }

}

void generateTurrets(world_t* world) {
    double w_x_min = -world->width / 2.0;
    double w_x_max = world->width / 2.0;
    double w_y_min = -world->height / 2.0;
    double w_y_max = world->height / 2.0;
    double w_z_min = -world->depth / 2.0;
    double w_z_max = world->depth / 2.0;

    rand_seed();

    printf("\n\nGenerating turrets...\n");

    for (int i = 0; i < world->n_turrets; i++) {

        printf("\t%d of %d... ", i+1, world->n_turrets);

        int near_checkpoint_no = rand_gen_int(0, world->n_checkpoints-1);
        point_t near_checkpoint = world->checkpoints[near_checkpoint_no].pos;
        double half_range = TURRET_CHECKPOINT_RANGE / 2.0;
        double x_min = max(w_x_min, near_checkpoint.x-half_range);
        double x_max = min(w_x_max, near_checkpoint.x+half_range);
        double y_min = max(w_y_min, near_checkpoint.y-half_range);
        double y_max = min(w_y_max, near_checkpoint.y+half_range);
        double z_min = max(w_z_min, near_checkpoint.z-half_range);
        double z_max = min(w_z_max, near_checkpoint.z+half_range);

        bool success = false;

        int attempts = 0;
        double pos_x, pos_y, pos_z;
        while (!success) {
            pos_x = rand_gen_range(x_min, x_max);
            pos_y = rand_gen_range(y_min, y_max);
            pos_z = rand_gen_range(z_min, z_max);
            point_t pos_gen = create_point(pos_x, pos_y, pos_z);
            success = true;
            for (int j = 0; j < world->n_mines; j++) {
                mine_t* mine_curr = &world->mines[j];
                if (distance(pos_gen, mine_curr->pos) < TURRET_RADIUS+MINE_RADIUS) {
                    success = false;
                    break;
                }
            }
            if (success) {
                for (int j = 0; j < i; j++) {
                    checkpoint_t* checkpoint_curr = &world->checkpoints[j];
                    if (distance(pos_gen, checkpoint_curr->pos) < TURRET_RADIUS+CHECKPOINT_RADIUS) {
                        success = false;
                        break;
                    }
                }
            }
            if (success) {
                for (int j = 0; j < i; j++) {
                    turret_t* turret_curr = &world->turrets[j];
                    if (distance(pos_gen, turret_curr->pos) < TURRET_RADIUS*2) {
                        success = false;
                        break;
                    }
                }
            }
            attempts++;
        }
        world->turrets[i] = createTurret(pos_x, pos_y, pos_z);
        printf("done with %d attempts.\n", attempts);

    }

}

void writeData(char* file_name, world_t* world) {

    printf("\n\nWriting world data to file... ");

    FILE* file = fopen(file_name, "w");

    // print header
    fprintf(file, "%lf %lf %lf %d %d %d\n",
        world->width, world->height, world->depth, world->n_checkpoints, world->n_mines, world->n_turrets);

    fprintf(file, "\n");

    // print checkpoint positions
    fprintf(file, "checkpoints\n");
    for (int i = 0; i < world->n_checkpoints; i++) {
        checkpoint_t* checkpoint_curr = &world->checkpoints[i];
        fprintf(file, "%lf %lf %lf\n",
            checkpoint_curr->pos.x, checkpoint_curr->pos.y, checkpoint_curr->pos.z);
    }

    fprintf(file, "\n");

    // print mines
    fprintf(file, "mines\n");
    for (int i = 0; i < world->n_mines; i++) {
        mine_t* mine_curr = &world->mines[i];
        fprintf(file, "%lf %lf %lf\n",
            mine_curr->pos.x, mine_curr->pos.y, mine_curr->pos.z);
    }

    fprintf(file, "\n");

    // print turrets
    fprintf(file, "turrets\n");
    for (int i = 0; i < world->n_turrets; i++) {
        turret_t* turret_cur = &world->turrets[i];
        fprintf(file, "%lf %lf %lf\n",
            turret_cur->pos.x, turret_cur->pos.y, turret_cur->pos.z);
    }

    fclose(file);

    printf("done.\n");
}

int main(int argc, char* argv[]) {

    double world_width, world_height, world_depth;
    int n_checkpoints, n_mines, n_turrets;
    char* file_path;

    if (argc == NUM_PARAMETERS+1) {
        file_path = argv[1];
        world_width = atof(argv[2]);
        world_height = atof(argv[3]);
        world_depth = atof(argv[4]);
        n_checkpoints = atoi(argv[5]);
        n_mines = atoi(argv[6]);
        n_turrets = atoi(argv[7]);
        printf("Parameters:\n");
        printf("\tfile_path: \"%s\"\n",file_path);
        printf("\tworld_width: %lf\n",world_width);
        printf("\tworld_height: %lf\n",world_height);
        printf("\tworld_depth: %lf\n",world_depth);
        printf("\tn_checkpoints: %d\n",n_checkpoints);
        printf("\tn_mines: %d\n",n_mines);
        printf("\tn_turrets: %d\n",n_turrets);
    } else {
        fprintf(stderr, "Bad parameters.\n");
        return -1;
    }

    world_t world = createWorld(world_width, world_height, world_depth,
        n_checkpoints, n_mines, n_turrets);

    generateMines(&world);
    generateCheckpoints(&world);
    generateTurrets(&world);
    writeData(file_path, &world);

    destroyWorld(&world);

    return 0;
}
