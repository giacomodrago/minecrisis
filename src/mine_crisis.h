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

#ifndef MINE_CRISIS_H
#define MINE_CRISIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "consts.h"
#include "opengl.h"
#include "openal.h"
#include "geom.h"
#include "world.h"
#include "draw.h"
#include "utils.h"
#include "crc32.h"
#include "random.h"
#include "texture.h"
#include "ase_importer.h"
#include "bounding_boxes.h"
#include "sound.h"
#include "music.h"
#include "explosion.h"
#include "resources.h"

typedef struct {
    mine_t** mines;
    int allocated_size;
    int n_mines;
} mine_group_t;

// world data
world_t world;

#define SECTOR_DIV_X 10
#define SECTOR_DIV_Y 10
#define SECTOR_DIV_Z 10
#define REALLOC_INCREMENT 10
struct {
    double width_step, height_step, depth_step;
    double width_sectors[SECTOR_DIV_X];
    double height_sectors[SECTOR_DIV_Y];
    double depth_sectors[SECTOR_DIV_Z];
    mine_group_t sectors[SECTOR_DIV_X][SECTOR_DIV_Y][SECTOR_DIV_Z];
} mine_groups;


typedef struct {
    double value;
    int direction;
} checkpoint_alpha_t;


// status variables
struct {
    double perspective_near;
    double perspective_far;
    double aspect_ratio;
    double perspective_fovx;
    double perspective_fovy;
    int viewport_width;
    int viewport_height;
    timestamp_t program_clock;
    timestamp_t game_clock;
    timestamp_t prev_timestamp;
    point_t ship_pos;
    point_t ship_prev_pos;
    point_t camera_pos;
    vector_t vector_look;
    vector_t vector_up;
    double ship_inclination;
    double camera_distance;
    double throttle;
    double throttle_x;
    double throttle_y;
    double throttle_rot;
    double planet_rot_x;
    double planet_rot_y;
    double cull_matrix[9];
    double collision_matrix[9];
    point_t shield_ellipsoid_points[SHIELD_ELLIPSOID_POINTS];
    int* sorted_checkpoints;
    checkpoint_alpha_t* checkpoints_alpha;
    explosion_t* explosion;
    timestamp_t countdown_last_timestamp;
    double gameover_opacity;
    timestamp_t gameover_timestamp;
    bool show_play_again;
    double ship_opacity;
    double copy_notice_opacity;
} status;


typedef struct shot_entry {
    shot_t shot;
    struct shot_entry* next;
} shot_entry_t;


typedef struct vector_entry {
    vector_t vector;
    struct vector_entry* next;
} vector_entry_t;


// game variables
struct {
    int phase;
    bool paused;
    int reached_checkpoints_no;
    vector_entry_t* move_vectors_list;
    int n_shots;
    shot_entry_t* shots_list;
    timestamp_t shield_down_timestamp;
    timestamp_t afterburner_down_timestamp;
    timestamp_t invisibility_down_timestamp;
    double shield_health;
    double afterburner_fuel;
    double invisibility_fuel;
    bool invisible_mode;
    double remaining_seconds;
} game;


// display lists ids
struct {
    display_list_t base_id;
    display_list_t mine_low;
    display_list_t mine_med;
    display_list_t mine_high;
    display_list_t mine_highest;
    display_list_t checkpoint_low;
    display_list_t checkpoint_med;
    display_list_t checkpoint_high;
    display_list_t turret_low;
    display_list_t turret_med;
    display_list_t turret_high;
    display_list_t turret_highest;
    display_list_t planet;
    display_list_t shot;
    display_list_t starmap;
    display_list_t ship_body;
    display_list_t ship_weapons;
    display_list_t ship_glass;
    display_list_t ship_decorations;
    display_list_t ship_glow;
} lists;
#define NUM_DISPLAY_LISTS 19

// model variables
struct {
    ASE_Object* ship;
    bounding_boxes_t ship_boxes;
    int n_ship_points;
    point_t* ship_points;
} models;

// texture variables
struct {
    texture_t planet;
    texture_t starmap;
    texture_t shield;
    texture_t afterburner;
    texture_t invisibility;
    texture_t numbers;
    texture_t remaining_time;
    texture_t remaining_checkpoints;
    texture_t gameover_lose;
    texture_t gameover_lose_timeout;
    texture_t gameover_win;
    texture_t pause;
    texture_t play_again;
    texture_t copy_notice;
    double numbers_coords[12][4];
} textures;

// sound buffers
struct {
    sound_buffer_t background_music;
    sound_buffer_t shot;
    sound_buffer_t gameover_win;
    sound_buffer_t gameover_lose;
    sound_buffer_t shield;
    sound_buffer_t alarm;
    sound_buffer_t mine;
    sound_buffer_t checkpoint;
    sound_buffer_t beep;
} sound_buffers;

// sound sources
struct {
    sound_source_t background_music;
    sound_source_t gameover;
    sound_source_t shield;
    sound_source_t alarm;
    sound_source_t mine;
    sound_source_t checkpoint;
    sound_source_t beep;
    sound_source_t* turrets;
} sound_sources;

struct {
    unsigned char* data;
    int data_len;
    music_context_t context;
} music;

// controls variables
struct {
    bool accel;
    bool decel;
    bool afterburner;
    bool rot_left;
    bool rot_right;
    bool invisibility;
    bool left;
    bool right;
    bool up;
    bool down;
    bool fullscreen;
} controls;


// debug variables
struct {
    timestamp_t prev_print_timestamp;
} debug;


// Initialization functions
void checkDistributionIntegrity();
void init(const char* file_path);
void initGraphics();
static inline texture_t loadTexture(const char* filename, crc32_t checksum);
texture_t loadTextureFromBuffer(const unsigned char* buffer, int buf_len, crc32_t checksum);
sound_buffer_t loadSound(const char* filename, crc32_t checksum);
static inline void loadBackgroundMusic(const char* filename, crc32_t checksum);
static inline void generateLists();
static inline void generateMineGroups();
static inline void loadShipModel();
static inline void loadTextureCoords();

// Cleanup functions
void cleanup();

// Game progress functions
void startGame();
void gameProgress();
void endGame();
void pauseGame(bool paused);
void gameOver(bool win);
static inline void addMoveVector(vector_t new_vector);
static inline void addShot(shot_t new_shot);
void findSector(point_t point, int* sector_x, int* sector_y, int* sector_z);

// Display functions
void reshape(int width, int height);
void draw();
void drawCockpit();

// Interaction functions
void fullscreen(bool active);
void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void keySpecialDown(int key, int x, int y);
void keySpecialUp(int key, int x, int y);

// Debug functions
void printDebugInfo();

#endif
