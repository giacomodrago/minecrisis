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

#include "mine_crisis.h"


/**************************************/
/****** INITIALIZATION FUNCTIONS ******/
/**************************************/

/*

// Disabled as of version 0.92

void checkDistributionIntegrity() {
    #ifndef __APPLE__
    char* filename;
    bool success = true;
    if (!checkCRC32(COPYRIGHT_FILE, COPYRIGHT_FILE_CHECKSUM)) {
        filename = COPYRIGHT_FILE;
        success = false;
    }
    if (success && !checkCRC32(LICENSE_FILE, LICENSE_FILE_CHECKSUM)) {
        filename = LICENSE_FILE;
        success = false;
    }
    if (success && !checkCRC32(README_FILE, README_FILE_CHECKSUM)) {
        filename = README_FILE;
        success = false;
    }
    if (!success) {
        fprintf(stderr, "ERROR: invalid or corrupted %s file.\n", filename);
        exit(EXIT_FAILURE);
    }
    #endif
}
*/

void init(const char* file_path) {

    world = loadWorld(file_path);

    loadShipModel();

    generateMineGroups();

    // Load texture coords
    loadTextureCoords();

    // Generate ellipsoid points
    generate_ellipsoid_points(status.shield_ellipsoid_points,
                              SHIELD_ELLIPSOID_POINTS_SQRT,
                              SHIP_RADIUS_X, SHIP_RADIUS_Y, SHIP_RADIUS_Z);

    // Internal buffers
    status.sorted_checkpoints = (int*) malloc(world.n_checkpoints * sizeof(int));
    status.checkpoints_alpha = (checkpoint_alpha_t*) malloc(world.n_checkpoints * sizeof(checkpoint_alpha_t));

    // Explosion
    status.explosion = explosion_create(EXPLOSION_POINTS, EXPLOSION_TRIANGLES);

    // Program clock
    status.program_clock = 0;
    status.copy_notice_opacity = 1.0;

    // Fullscreen mode
    controls.fullscreen = false;

    sound_init();

    // Set sound buffers
    sound_buffers.shot = loadSound(SHOT_SOUND_FILE, SHOT_SOUND_FILE_CHECKSUM);
    sound_buffers.gameover_lose = loadSound(GAMEOVER_LOSE_SOUND_FILE, GAMEOVER_LOSE_SOUND_FILE_CHECKSUM);
    sound_buffers.gameover_win = loadSound(GAMEOVER_WIN_SOUND_FILE, GAMEOVER_WIN_SOUND_FILE_CHECKSUM);
    sound_buffers.shield = loadSound(SHIELD_SOUND_FILE, SHIELD_SOUND_FILE_CHECKSUM);
    sound_buffers.alarm = loadSound(ALARM_SOUND_FILE, ALARM_SOUND_FILE_CHECKSUM);
    sound_buffers.mine = loadSound(MINE_SOUND_FILE, MINE_SOUND_FILE_CHECKSUM);
    sound_buffers.checkpoint = loadSound(CHECKPOINT_SOUND_FILE, CHECKPOINT_SOUND_FILE_CHECKSUM);
    sound_buffers.beep = loadSound(BEEP_SOUND_FILE, BEEP_SOUND_FILE_CHECKSUM);

    // Set sound sources
    sound_sources.gameover = sound_create_source_unpos();
    sound_sources.shield = sound_create_source_unpos();
    sound_sources.alarm = sound_create_source_unpos();
    sound_sources.mine = sound_create_source(ORIGIN, SOUND_MAX_DISTANCE);
    sound_sources.checkpoint = sound_create_source_unpos();
    sound_sources.beep = sound_create_source_unpos();
    sound_sources.turrets = (sound_source_t*)malloc(world.n_turrets * sizeof(sound_source_t));
    for (int i = 0; i < world.n_turrets; i++) {
        sound_sources.turrets[i] = sound_create_source(world.turrets[i].pos, TURRET_ENGAGE_DIST);
    }

    #if ENABLE_BACKGROUND_MUSIC
    loadBackgroundMusic(MUSIC_SOUND_FILE, MUSIC_SOUND_FILE_CHECKSUM);
    #endif

}

void initGraphics() {

	glMatrixMode(GL_MODELVIEW);

    // Set default color and depth
    glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);

    // Planet light
	glLightfv(PLANET_LIGHT, GL_AMBIENT, planet_light_ambient);
	glLightfv(PLANET_LIGHT, GL_DIFFUSE, planet_light_diffuse);
	glLightfv(PLANET_LIGHT, GL_SPECULAR, planet_light_specular);

    // Ship spotlight
    glLightfv(SPACESHIP_SPOTLIGHT, GL_AMBIENT, spaceship_spotlight_ambient);
    glLightfv(SPACESHIP_SPOTLIGHT, GL_DIFFUSE, spaceship_spotlight_diffuse);
	glLightfv(SPACESHIP_SPOTLIGHT, GL_SPECULAR, spaceship_spotlight_specular);
	glLightf(SPACESHIP_SPOTLIGHT, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(SPACESHIP_SPOTLIGHT, GL_QUADRATIC_ATTENUATION, SPOTLIGHT_ATTENUATION);

    // Fake light back the ship
    glLightfv(FAKE_SHIP_LIGHT, GL_POSITION, ship_light_position);
	glLightfv(FAKE_SHIP_LIGHT, GL_AMBIENT, ship_light_ambient);
	glLightfv(FAKE_SHIP_LIGHT, GL_DIFFUSE, ship_light_diffuse);
	glLightfv(FAKE_SHIP_LIGHT, GL_SPECULAR, ship_light_specular);

    // Setup depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

    // Enable automatic normalization
    glEnable(GL_NORMALIZE);

    // Load textures
    glEnable(GL_TEXTURE_2D);

    textures.planet = loadTexture(PLANET_TEXTURE_FILE, PLANET_TEXTURE_FILE_CHECKSUM);
    textures.starmap = loadTexture(STARMAP_TEXTURE_FILE, STARMAP_TEXTURE_FILE_CHECKSUM);
    textures.shield = loadTexture(SHIELD_TEXTURE_FILE, SHIELD_TEXTURE_FILE_CHECKSUM);
    textures.afterburner = loadTexture(AFTERBURNER_TEXTURE_FILE, AFTERBURNER_TEXTURE_FILE_CHECKSUM);
    textures.invisibility = loadTexture(INVISIBILITY_TEXTURE_FILE, INVISIBILITY_TEXTURE_FILE_CHECKSUM);
    textures.numbers = loadTexture(NUMBERS_TEXTURE_FILE, NUMBERS_TEXTURE_FILE_CHECKSUM);
    textures.remaining_time = loadTexture(REMAINING_TIME_TEXTURE_FILE, REMAINING_TIME_TEXTURE_FILE_CHECKSUM);
    textures.remaining_checkpoints = loadTexture(REMAINING_CHECKPOINTS_TEXTURE_FILE, REMAINING_CHECKPOINTS_TEXTURE_FILE_CHECKSUM);
    textures.gameover_lose = loadTexture(GAMEOVER_LOSE_TEXTURE_FILE, GAMEOVER_LOSE_TEXTURE_FILE_CHECKSUM);
    textures.gameover_lose_timeout = loadTexture(GAMEOVER_LOSE_TIMEOUT_TEXTURE_FILE, GAMEOVER_LOSE_TIMEOUT_TEXTURE_FILE_CHECKSUM);
    textures.gameover_win = loadTexture(GAMEOVER_WIN_TEXTURE_FILE, GAMEOVER_WIN_TEXTURE_FILE_CHECKSUM);
    textures.pause = loadTexture(PAUSE_TEXTURE_FILE, PAUSE_TEXTURE_FILE_CHECKSUM);
    textures.play_again = loadTexture(PLAY_AGAIN_TEXTURE_FILE, PLAY_AGAIN_TEXTURE_FILE_CHECKSUM);
    textures.copy_notice = loadTextureFromBuffer(res_copy_notice, RES_COPY_NOTICE_SIZE, RES_COPY_NOTICE_CHECKSUM);

    // Generate display lists
    generateLists();

}

/**************************************/

texture_t loadTexture(const char* filename, crc32_t checksum) {
    bool success = false;
    texture_t result;
    unsigned char* buffer;
    int buf_len = readFile(filename, &buffer);
    if (buf_len != FILE_READ_ERROR && calculate_crc32(buffer, buf_len) == checksum) {
        result = texture_load_PNG_buf(buffer, buf_len, GL_LINEAR, GL_LINEAR);
        if (result != TEXTURE_LOAD_ERROR) success = true;
    }
    free(buffer);
    if (success) {
        printf("Loaded '%s' successfully.\n", filename);
    } else {
        fprintf(stderr, "ERROR: error loading '%s' texture!\n", filename);
        exit(EXIT_FAILURE);
    }
    return result;
}

/**************************************/

texture_t loadTextureFromBuffer(const unsigned char* buffer, int buf_len, crc32_t checksum) {
    bool success = false;
    texture_t result;
    if (calculate_crc32(buffer, buf_len) == checksum) {
        result = texture_load_PNG_buf(buffer, buf_len, GL_LINEAR, GL_LINEAR);
        if (result != TEXTURE_LOAD_ERROR) success = true;
    }
    if (!success) {
        fprintf(stderr, "ERROR: error loading texture!\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

/**************************************/

sound_buffer_t loadSound(const char* filename, crc32_t checksum) {
    bool success = false;
    sound_buffer_t result;
    unsigned char* buffer;
    int buf_len = readFile(filename, &buffer);
    if (buf_len != FILE_READ_ERROR && calculate_crc32(buffer, buf_len) == checksum) {
        result = sound_create_buffer_wav(buffer, buf_len);
        if (result != BUFFER_LOAD_ERROR) success = true;
    }
    free(buffer);
    if (success) {
        printf("Loaded '%s' successfully.\n", filename);
    } else {
        fprintf(stderr, "ERROR: error loading '%s' sound!\n", filename);
        exit(EXIT_FAILURE);
    }
    return result;
}

/**************************************/

void loadBackgroundMusic(const char* filename, crc32_t checksum) {
    bool success = false;
    music.data_len = readFile(filename, &music.data);
    #if MUSIC_SOUND_FILE_CHECK
    if (calculate_crc32(music.data, music.data_len) == checksum) {
        success = true;
    }
    #else
    success = true;
    #endif

    if (success) {
        printf("Loaded '%s' successfully.\n", filename);
    } else {
        fprintf(stderr, "ERROR: error loading '%s' background music!\n", filename);
        exit(EXIT_FAILURE);
    }
}

/**************************************/

void generateLists() {

    GLuint base_id = glGenLists(NUM_DISPLAY_LISTS);
    lists.base_id = base_id;

    lists.mine_low = base_id;
    glNewList(lists.mine_low, GL_COMPILE);
        drawMine(0); // low detail
    glEndList();

    lists.mine_med = base_id+1;
    glNewList(lists.mine_med, GL_COMPILE);
        drawMine(1); // med detail
    glEndList();

    lists.mine_high = base_id+2;
    glNewList(lists.mine_high, GL_COMPILE);
        drawMine(2); // high detail
    glEndList();

    lists.mine_highest = base_id+3;
    glNewList(lists.mine_highest, GL_COMPILE);
        drawMine(3); // highest detail
    glEndList();

    lists.checkpoint_low = base_id+4;
    glNewList(lists.checkpoint_low, GL_COMPILE);
        drawCheckpointSphere(0); // low detail
    glEndList();

    lists.checkpoint_med = base_id+5;
    glNewList(lists.checkpoint_med, GL_COMPILE);
        drawCheckpointSphere(1); // med detail
    glEndList();

    lists.checkpoint_high = base_id+6;
    glNewList(lists.checkpoint_high, GL_COMPILE);
        drawCheckpointSphere(2); // high detail
    glEndList();

    lists.turret_low = base_id+7;
    glNewList(lists.turret_low, GL_COMPILE);
        drawTurret(0); // low detail
    glEndList();

    lists.turret_med = base_id+8;
    glNewList(lists.turret_med, GL_COMPILE);
        drawTurret(1); // med detail
    glEndList();

    lists.turret_high = base_id+9;
    glNewList(lists.turret_high, GL_COMPILE);
        drawTurret(2); // high detail
    glEndList();

    lists.turret_highest = base_id+10;
    glNewList(lists.turret_highest, GL_COMPILE);
        drawTurret(3); // highest detail
    glEndList();

    lists.shot = base_id+11;
    glNewList(lists.shot, GL_COMPILE);
        drawShot();
    glEndList();

    lists.planet = base_id+12;
    glNewList(lists.planet, GL_COMPILE);
        drawPlanet(textures.planet);
    glEndList();

    lists.starmap = base_id+13;
    glNewList(lists.starmap, GL_COMPILE);
        drawStarmap(textures.starmap);
    glEndList();

    lists.ship_body = base_id+14;
    glNewList(lists.ship_body, GL_COMPILE);
        ASE_drawMesh(models.ship, "Body", false);
    glEndList();

    lists.ship_weapons = base_id+15;
    glNewList(lists.ship_weapons, GL_COMPILE);
        ASE_drawMesh(models.ship, "Weapons", false);
    glEndList();

    lists.ship_glass = base_id+16;
    glNewList(lists.ship_glass, GL_COMPILE);
        ASE_drawMesh(models.ship, "Glass", false);
    glEndList();

    lists.ship_decorations = base_id+17;
    glNewList(lists.ship_decorations, GL_COMPILE);
        ASE_drawMesh(models.ship, "Decorations", false);
    glEndList();

    lists.ship_glow = base_id+18;
    glNewList(lists.ship_glow, GL_COMPILE);
        ASE_drawMesh(models.ship, "Glow", false);
    glEndList();


}

/**************************************/

void generateMineGroups() {

    mine_groups.width_step = world.width / SECTOR_DIV_X;
    mine_groups.height_step = world.height / SECTOR_DIV_Y;
    mine_groups.depth_step = world.depth / SECTOR_DIV_Z;

    double x_min = -world.width / 2.0;
    double y_min = -world.height / 2.0;
    double z_min = -world.depth / 2.0;

    for (int i = 0; i < SECTOR_DIV_X; i++) {
        if (i == 0) mine_groups.width_sectors[i] = x_min + mine_groups.width_step;
        else mine_groups.width_sectors[i] = mine_groups.width_sectors[i-1] + mine_groups.width_step;
    }
    for (int i = 0; i < SECTOR_DIV_Y; i++) {
        if (i == 0) mine_groups.height_sectors[i] = y_min + mine_groups.height_step;
        else mine_groups.height_sectors[i] = mine_groups.height_sectors[i-1] + mine_groups.height_step;
    }
    for (int i = 0; i < SECTOR_DIV_Z; i++) {
        if (i == 0) mine_groups.depth_sectors[i] = z_min + mine_groups.depth_step;
        else mine_groups.depth_sectors[i] = mine_groups.depth_sectors[i-1] + mine_groups.depth_step;
    }

    int default_allocated_size = world.n_mines / (SECTOR_DIV_X * SECTOR_DIV_Y * SECTOR_DIV_Z);
    if (default_allocated_size == 0) default_allocated_size = 1;

    for (int i = 0; i < SECTOR_DIV_X; i++) {
        for (int j = 0; j < SECTOR_DIV_Y; j++) {
            for (int k = 0; k < SECTOR_DIV_Y; k++) {
                mine_group_t* group = &mine_groups.sectors[i][j][k];
                group->n_mines = 0;
                group->allocated_size = default_allocated_size;
                group->mines = (mine_t**) malloc(default_allocated_size * sizeof(mine_t*));
            }
        }
    }

    for (int m = 0; m < world.n_mines; m++) {

        mine_t* mine = &world.mines[m];
        point_t pos = mine->pos;

        int sector_x = -1, sector_y = -1, sector_z = -1;
        findSector(pos, &sector_x, &sector_y, &sector_z);
        mine_group_t* group = &mine_groups.sectors[sector_x][sector_y][sector_z];

        int index = group->n_mines++;
        if (group->n_mines > group->allocated_size) {
            group->allocated_size += REALLOC_INCREMENT;
            group->mines = (mine_t**) realloc(group->mines, group->allocated_size * sizeof(mine_t*));
        }
        group->mines[index] = mine;

    }

}

/**************************************/

void loadShipModel() {

    if (checkCRC32(SHIP_MODEL_FILE, SHIP_MODEL_FILE_CHECKSUM)) {
        models.ship = ASE_create(SHIP_MODEL_FILE);
    } else {
        fprintf(stderr, "ERROR: Error loading '%s' file!\n", SHIP_MODEL_FILE);
        exit(EXIT_FAILURE);
    }
    ASE_swapYZ(models.ship);
    double translate_x, translate_y, translate_z;
    ASE_centerModel(models.ship, &translate_x, &translate_y, &translate_z);
    ASE_scaleModel(models.ship,
                   SHIP_MODEL_SCALING_FACTOR, SHIP_MODEL_SCALING_FACTOR, SHIP_MODEL_SCALING_FACTOR);

    if (checkCRC32(SHIP_BOUNDS_FILE, SHIP_BOUNDS_FILE_CHECKSUM)) {
        models.ship_boxes = boundingboxes_load(SHIP_BOUNDS_FILE, true);
    } else {
        fprintf(stderr, "ERROR: Error loading '%s' file!\n", SHIP_BOUNDS_FILE);
        exit(EXIT_FAILURE);
    }
    // Scale and translate boxes
    for (int i = 0; i < models.ship_boxes.n_boxes; i++) {
        bounding_box_t* curr_box = &models.ship_boxes.array[i];
        curr_box->x_min = (curr_box->x_min + translate_x) * SHIP_MODEL_SCALING_FACTOR;
        curr_box->x_max = (curr_box->x_max + translate_x) * SHIP_MODEL_SCALING_FACTOR;
        curr_box->y_min = (curr_box->y_min + translate_y) * SHIP_MODEL_SCALING_FACTOR;
        curr_box->y_max = (curr_box->y_max + translate_y) * SHIP_MODEL_SCALING_FACTOR;
        curr_box->z_min = (curr_box->z_min + translate_z) * SHIP_MODEL_SCALING_FACTOR;
        curr_box->z_max = (curr_box->z_max + translate_z) * SHIP_MODEL_SCALING_FACTOR;
    }

    // Get ship points for collisions
    int n_vertexes = ASE_numVertexes(models.ship);
    vertex_t* all_vertexes = (vertex_t*) malloc(n_vertexes * sizeof(vertex_t));
    ASE_getAllVertexes(models.ship, all_vertexes);
    models.n_ship_points = n_vertexes / 4 + 1;
    models.ship_points = (point_t*) malloc(models.n_ship_points * sizeof(point_t));
    printf("Model vertexes: %d; Collision points: %d\n\n", n_vertexes, models.n_ship_points);

    int k = 0;
    for (int i = 0; i < n_vertexes; i+=4) {
        vertex_t* curr_vertex = &all_vertexes[i];
        models.ship_points[k++] = create_point(curr_vertex->x, curr_vertex->y, curr_vertex->z);
    }

    free(all_vertexes);

}

/**************************************/

void loadTextureCoords() {
    bool success = true;
    if (checkCRC32(NUMBERS_COORDS_FILE, NUMBERS_COORDS_FILE_CHECKSUM)) {
        FILE* coords_fp = fopen(NUMBERS_COORDS_FILE, "r");
        if (!coords_fp) success = false;
        if (success) {
            char line[256];
            int c = 0;
            while (fgets(line, sizeof(line), coords_fp) != NULL) {
                sscanf(line, "%lf %lf %lf %lf",
                       &textures.numbers_coords[c][0],
                       &textures.numbers_coords[c][1],
                       &textures.numbers_coords[c][2],
                       &textures.numbers_coords[c][3]);
                c++;
            }
        }
    } else {
        success = false;
    }
    if (!success) {
        fprintf(stderr, "ERROR: Error loading '%s' file!\n", NUMBERS_COORDS_FILE);
        exit(EXIT_FAILURE);
    }
}

/**************************************/

/**************************************/
/******    CLEANUP FUNCTIONS     ******/
/**************************************/

void cleanup() {

    printf("\nCleaning up...\n");

    destroyWorld(&world);

    free(status.sorted_checkpoints);
    free(status.checkpoints_alpha);

    shot_entry_t* shot_e = game.shots_list;
    while (shot_e != NULL) {
        shot_entry_t* next = shot_e->next;
        free(shot_e);
        shot_e = next;
    }

    vector_entry_t* vector_e = game.move_vectors_list;
    while (vector_e != NULL) {
        vector_entry_t* next = vector_e->next;
        free(vector_e);
        vector_e = next;
    }

    for (int i = 0; i < SECTOR_DIV_X; i++) {
        for (int j = 0; j < SECTOR_DIV_Y; j++) {
            for (int k = 0; k < SECTOR_DIV_Y; k++) {
                mine_group_t* group = &mine_groups.sectors[i][j][k];
                free(group->mines);
            }
        }
    }

    ASE_destroy(models.ship);
    boundingboxes_destroy(&models.ship_boxes);
    free(models.ship_points);

    glDeleteTextures(1, &textures.planet);
    glDeleteTextures(1, &textures.starmap);
    glDeleteTextures(1, &textures.shield);
    glDeleteTextures(1, &textures.afterburner);
    glDeleteTextures(1, &textures.invisibility);
    glDeleteTextures(1, &textures.numbers);
    glDeleteTextures(1, &textures.remaining_time);
    glDeleteTextures(1, &textures.remaining_checkpoints);
    glDeleteTextures(1, &textures.gameover_lose);
    glDeleteTextures(1, &textures.gameover_lose_timeout);
    glDeleteTextures(1, &textures.gameover_win);
    glDeleteTextures(1, &textures.pause);
    glDeleteTextures(1, &textures.play_again);

    glDeleteLists(lists.base_id, NUM_DISPLAY_LISTS);

    explosion_destroy(status.explosion);

    sound_destroy_source(sound_sources.gameover);
    sound_destroy_source(sound_sources.shield);
    sound_destroy_source(sound_sources.alarm);
    sound_destroy_source(sound_sources.mine);
    sound_destroy_source(sound_sources.checkpoint);
    sound_destroy_source(sound_sources.beep);
    for (int i = 0; i < world.n_turrets; i++) {
        sound_destroy_source(sound_sources.turrets[i]);
    }
    free(sound_sources.turrets);

    sound_destroy_buffer(sound_buffers.shot);
    sound_destroy_buffer(sound_buffers.gameover_lose);
    sound_destroy_buffer(sound_buffers.gameover_win);
    sound_destroy_buffer(sound_buffers.shield);
    sound_destroy_buffer(sound_buffers.alarm);
    sound_destroy_buffer(sound_buffers.mine);
    sound_destroy_buffer(sound_buffers.checkpoint);
    sound_destroy_buffer(sound_buffers.beep);

    music_stop(&music.context);
    free(music.data);

    sound_cleanup();

    printf("Goodbye!\n");

}

/**************************************/

/**************************************/
/****   GAME PROGRESS FUNCTIONS    ****/
/**************************************/

void startGame() {

    // Seed random number generators
    rand_seed();

    // Initialize status variables
	status.ship_pos = create_point(world.width / 2.0 + START_DISTANCE, 0.0, 0.0);
	status.camera_pos = status.ship_pos;
	status.vector_look = create_vector(-1.0, 0.0, 0.0);
	status.vector_up = Y_AXIS;
	status.ship_inclination = 0.0;
	status.camera_distance = CAMERA_DEFAULT_DIST;
    status.throttle = THROTTLE_DEFAULT;
    status.throttle_x = 0.0;
    status.throttle_y = 0.0;
    status.throttle_rot = 0.0;
    status.prev_timestamp = microtime();
    status.game_clock = 0;
    status.countdown_last_timestamp = 0;
    status.gameover_timestamp = 0;
    status.gameover_opacity = 0.0;
    status.show_play_again = false;
    status.ship_opacity = 1.0;

    for (int i = 0; i < world.n_checkpoints; i++) {
        double value = rand_gen_range(CHECKPOINT_MIN_ALPHA, CHECKPOINT_MAX_ALPHA);
        int direction = rand_gen_int(0, 1);
        checkpoint_alpha_t curr_checkpoint_alpha = {value, direction};
        status.checkpoints_alpha[i] = curr_checkpoint_alpha;
        world.checkpoints[i].status = CHECKPOINT_STATUS_UNREACHED;
    }

    for (int i = 0; i < world.n_mines; i++) {
        world.mines[i].status = MINE_STATUS_IDLE;
    }

    for (int i = 0; i < world.n_turrets; i++) {
        world.turrets[i].status = TURRET_STATUS_IDLE;
    }

    status.planet_rot_x = 0.0;
    status.planet_rot_y = 0.0;

    // Initialize debug variables
    debug.prev_print_timestamp = status.prev_timestamp;

    // Initialize explosion
    explosion_reset(status.explosion);

    // Initialize game variables
    game.phase = PHASE_ACTION;
    game.paused = false;
    game.reached_checkpoints_no = 0;
    game.n_shots = 0;

    shot_entry_t* shot_e = game.shots_list;
    while (shot_e != NULL) {
        shot_entry_t* next = shot_e->next;
        free(shot_e);
        shot_e = next;
    }
    vector_entry_t* vector_e = game.move_vectors_list;
    while (vector_e != NULL) {
        vector_entry_t* next = vector_e->next;
        free(vector_e);
        vector_e = next;
    }
    game.shots_list = NULL;
    game.move_vectors_list = NULL;

    game.shield_health = MAX_SHIELD;
    game.afterburner_fuel = MAX_AFTERBURNER_FUEL;
    game.invisibility_fuel = MAX_INVISIBILITY_FUEL;
    game.invisible_mode = false;
    game.remaining_seconds = STARTING_TIME;
    game.shield_down_timestamp = 0;
    game.afterburner_down_timestamp = 0;
    game.invisibility_down_timestamp = 0;

    // Initialize control variables
    controls.accel = false;
    controls.decel = false;
    controls.afterburner = false;
    controls.rot_left = false;
    controls.rot_right = false;
    controls.invisibility = false;
    controls.up = false;
    controls.down = false;
    controls.left = false;
    controls.right = false;

    #if ENABLE_BACKGROUND_MUSIC
    // Start background music
    if (music_create_context_ogg(&music.context, music.data, music.data_len, MUSIC_SOUND_GAIN)) {
        music_start(&music.context);
    } else {
        fprintf(stderr, "ERROR: could not play background music!");
        exit(EXIT_FAILURE);
    }
    #endif

    // Update animation variables
    gameProgress();

}

/**************************************/

void gameProgress() {

    timestamp_t timestamp = microtime();
    timestamp_t delta_t_us = timestamp - status.prev_timestamp;
    seconds_t delta_t = usToSecs(delta_t_us);
    status.prev_timestamp = timestamp;

    status.program_clock += delta_t_us;

    if (status.copy_notice_opacity > 0.0 && usToSecs(status.program_clock) > COPYRIGHT_NOTICE_FADE_DELAY)
        status.copy_notice_opacity -= delta_t * COPYRIGHT_NOTICE_FADE_SPEED;

    if (game.paused) return;

    status.game_clock += delta_t_us;

    if (game.phase == PHASE_ACTION) {

        // Update remaining seconds
        game.remaining_seconds -= delta_t;
        if (game.remaining_seconds < 0.0) {
            gameOver(false);
            return;
        } else if (game.remaining_seconds < (double)COUNTDOWN_BEGIN) {
            if (usToSecs(status.game_clock - status.countdown_last_timestamp) > 1.0) {
                sound_play(sound_buffers.beep, sound_sources.beep, BEEP_SOUND_GAIN, false);
                status.countdown_last_timestamp = status.game_clock;
            }
        }

        static bool playing_alarm = false;

        // Respawn shield
        if (usToSecs(status.game_clock - game.shield_down_timestamp) > SHIELD_RESPAWN_DELAY) {
            if (game.shield_health < 0.0) game.shield_health = 0.0;
            game.shield_health += delta_t * SHIELD_RESPAWN_SPEED;
            if (game.shield_health > MAX_SHIELD) game.shield_health = MAX_SHIELD;
            sound_stop(sound_sources.alarm);
            playing_alarm = false;
        } else if (game.shield_health < 0.0) {
            if (!playing_alarm) {
                sound_play(sound_buffers.alarm, sound_sources.alarm, ALARM_SOUND_GAIN, true);
                playing_alarm = true;
            }
        } else {
            sound_stop(sound_sources.alarm);
            playing_alarm = false;
        }

        // Respawn afterburner fuel
        if (usToSecs(status.game_clock - game.afterburner_down_timestamp) > AFTERBURNER_RESPAWN_DELAY) {
            if (game.afterburner_fuel < 0.0) game.afterburner_fuel = 0.0;
            game.afterburner_fuel += delta_t * AFTERBURNER_RESPAWN_SPEED;
            if (game.afterburner_fuel > MAX_AFTERBURNER_FUEL) game.afterburner_fuel = MAX_AFTERBURNER_FUEL;
        }

        // Respawn invisibility fuel
        if (usToSecs(status.game_clock - game.invisibility_down_timestamp) > INVISIBILITY_RESPAWN_DELAY) {
            if (game.invisibility_fuel < 0.0) game.invisibility_fuel = 0.0;
            game.invisibility_fuel += delta_t * INVISIBILITY_RESPAWN_SPEED;
            if (game.invisibility_fuel > MAX_INVISIBILITY_FUEL) game.invisibility_fuel = MAX_INVISIBILITY_FUEL;
        }

        if (game.invisible_mode) {
            status.ship_opacity -= delta_t * INVISIBILITY_TRANSITION_SPEED;
            if (status.ship_opacity < INVISIBILITY_SHIP_OPACITY)
                status.ship_opacity = INVISIBILITY_SHIP_OPACITY;
        } else {
            status.ship_opacity += delta_t * INVISIBILITY_TRANSITION_SPEED;
            if (status.ship_opacity > 1.0) status.ship_opacity = 1.0;
        }


        /* CONTROLS */

        if (game.afterburner_fuel > 0.0 && controls.afterburner) {
            status.throttle += delta_t * THROTTLE_INCREMENT * THROTTLE_AFTERBURNER_FACTOR;
            if (status.throttle > THROTTLE_AFTERBURNER) {
                status.throttle = THROTTLE_AFTERBURNER;
            }
            game.afterburner_fuel -= delta_t * AFTERBURNER_FUEL_COMSUMPTION;
            game.afterburner_down_timestamp = status.game_clock;
        } else if (controls.accel) {
            if (status.throttle > THROTTLE_MAX + SMALL_DELTA) {
                status.throttle -= delta_t * THROTTLE_INCREMENT;
            } else {
                status.throttle += delta_t * THROTTLE_INCREMENT;
                if (status.throttle > THROTTLE_MAX) {
                    status.throttle = THROTTLE_MAX;
                }
            }
        } else if (controls.decel) {
            status.throttle -= delta_t * THROTTLE_INCREMENT;
            if (status.throttle < THROTTLE_MIN) {
                status.throttle = THROTTLE_MIN;
            }
        } else {
            if (status.throttle > THROTTLE_DEFAULT + SMALL_DELTA) {
                status.throttle -= delta_t * THROTTLE_INCREMENT;
                if (status.throttle < THROTTLE_DEFAULT) {
                    status.throttle = THROTTLE_DEFAULT;
                }
            } else if (status.throttle < THROTTLE_DEFAULT - SMALL_DELTA) {
                status.throttle += delta_t * THROTTLE_INCREMENT;
                if (status.throttle > THROTTLE_DEFAULT) {
                    status.throttle = THROTTLE_DEFAULT;
                }
            } else {
                status.throttle = THROTTLE_DEFAULT;
            }
        }

        if (controls.left) {
            if (status.throttle_x > 0.0) {
                status.throttle_x += delta_t * THROTTLE_X_INCREMENT;
                if (status.throttle_x > THROTTLE_X_MAX) {
                    status.throttle_x = THROTTLE_X_MAX;
                }
            } else {
                status.throttle_x += delta_t * THROTTLE_X_INCREMENT * 3.0;
            }
        } else if (controls.right) {
            if (status.throttle_x < 0.0) {
                status.throttle_x -= delta_t * THROTTLE_X_INCREMENT;
                if (status.throttle_x < -THROTTLE_X_MAX) {
                    status.throttle_x = -THROTTLE_X_MAX;
                }
            } else {
                status.throttle_x -= delta_t * THROTTLE_X_INCREMENT * 3.0;
            }
        } else {
            if (status.throttle_x > 0.0 + SMALL_DELTA) {
                status.throttle_x -= delta_t * THROTTLE_X_INCREMENT * 2.0;
                if (status.throttle_x < 0.0) {
                    status.throttle_x = 0.0;
                }
            } else if (status.throttle_x < 0.0 - SMALL_DELTA) {
                status.throttle_x += delta_t * THROTTLE_X_INCREMENT * 2.0;
                if (status.throttle_x > 0.0) {
                    status.throttle_x = 0.0;
                }
            } else {
                status.throttle_x = 0.0;
            }
        }

        if (controls.up) {
            if (status.throttle_y > 0.0) {
                status.throttle_y += delta_t * THROTTLE_Y_INCREMENT;
                if (status.throttle_y > THROTTLE_Y_MAX) {
                    status.throttle_y = THROTTLE_Y_MAX;
                }
            } else {
                status.throttle_y += delta_t * THROTTLE_Y_INCREMENT * 3.0;
            }
        } else if (controls.down) {
            if (status.throttle_y < 0.0) {
                status.throttle_y -= delta_t * THROTTLE_Y_INCREMENT;
                if (status.throttle_y < -THROTTLE_Y_MAX) {
                    status.throttle_y = -THROTTLE_Y_MAX;
                }
            } else {
                status.throttle_y -= delta_t * THROTTLE_Y_INCREMENT * 3.0;
            }
        } else {
            if (status.throttle_y > 0.0 + SMALL_DELTA) {
                status.throttle_y -= delta_t * THROTTLE_Y_INCREMENT * 2.0;
                if (status.throttle_y < 0.0) {
                    status.throttle_y = 0.0;
                }
            } else if (status.throttle_y < 0.0 - SMALL_DELTA) {
                status.throttle_y += delta_t * THROTTLE_Y_INCREMENT * 2.0;
                if (status.throttle_y > 0.0) {
                    status.throttle_y = 0.0;
                }
            } else {
                status.throttle_y = 0.0;
            }
        }

        if (controls.rot_left) {
            if (status.throttle_rot < 0.0) {
                status.throttle_rot -= delta_t * THROTTLE_ROT_INCREMENT;
                if (status.throttle_rot < -THROTTLE_ROT_MAX) {
                    status.throttle_rot = -THROTTLE_ROT_MAX;
                }
            } else {
                status.throttle_rot -= delta_t * THROTTLE_ROT_INCREMENT * 3.0;
            }
        } else if (controls.rot_right) {
            if (status.throttle_rot > 0.0) {
                status.throttle_rot += delta_t * THROTTLE_ROT_INCREMENT;
                if (status.throttle_rot > THROTTLE_ROT_MAX) {
                    status.throttle_rot = THROTTLE_ROT_MAX;
                }
            } else {
                status.throttle_rot += delta_t * THROTTLE_ROT_INCREMENT * 3.0;
            }
        } else {
            if (status.throttle_rot > 0.0 + SMALL_DELTA) {
                status.throttle_rot -= delta_t * THROTTLE_ROT_INCREMENT * 4.0;
                if (status.throttle_rot < 0.0) {
                    status.throttle_rot = 0.0;
                }
            } else if (status.throttle_rot < 0.0 - SMALL_DELTA) {
                status.throttle_rot += delta_t * THROTTLE_ROT_INCREMENT * 4.0;
                if (status.throttle_rot > 0.0) {
                    status.throttle_rot = 0.0;
                }
            } else {
                status.throttle_rot = 0.0;
            }
        }

        if (game.invisibility_fuel > 0.0 && controls.invisibility) {
            game.invisible_mode = true;
            game.invisibility_fuel -= delta_t * INVISIBILITY_FUEL_COMSUMPTION;
            game.invisibility_down_timestamp = status.game_clock;
        } else {
            controls.invisibility = false;
            game.invisible_mode = false;
        }

        // Calculate collision matrix
        vector_t vector_up_incl = rotate(status.vector_up, status.vector_look, -status.ship_inclination);
        generate_transform_matrix(status.collision_matrix, status.vector_look, vector_up_incl);

        // Move the ship according to previous vector look
        status.ship_prev_pos = status.ship_pos;
        double delta_r = status.throttle * delta_t;
        vector_t move_vector = scalar_vector_product(delta_r, status.vector_look);
        status.ship_pos = vectors_sum(status.ship_pos, move_vector);

        // Update listener position
        sound_set_listener(status.ship_pos, status.vector_look, status.vector_up);

        // Calculate new vector look
        double horiz_angle = status.throttle_x * delta_t;
        status.vector_look = rotate(status.vector_look, status.vector_up, horiz_angle);

        // Calculate new vector up and vector look accordingly
        double vert_angle = status.throttle_y * delta_t;
        // Calculate vector right
        vector_t vector_right = cross_product(status.vector_look, status.vector_up);
        status.vector_up = rotate(status.vector_up, vector_right, vert_angle);
        status.vector_look = rotate(status.vector_look, vector_right, vert_angle);

        // Apply rotation
        double rot_angle = status.throttle_rot * delta_t;
        status.vector_up = rotate(status.vector_up, status.vector_look, rot_angle);

        // Calculate ship inclination
        status.ship_inclination = status.throttle_x / THROTTLE_X_INCLINATION_FACTOR;


        /* MINES */

        #if DISABLE_MINE_COLLISIONS == 0

        mine_t* exploded_mines[MINE_EXPLODED_BUF_SIZE];
        int n_exploded_mines = 0;

        int sector_x, sector_y, sector_z;
        findSector(status.ship_pos, &sector_x, &sector_y, &sector_z);

        for (int i = sector_x-1; i <= sector_x+1; i++) {
            if (i < 0 || i > SECTOR_DIV_X-1) continue;
            for (int j = sector_y-1; j <= sector_y+1; j++) {
                if (j < 0 || j > SECTOR_DIV_Y-1) continue;
                for (int k = sector_z-1; k <= sector_z+1; k++) {
                    if (k < 0 || k > SECTOR_DIV_Z-1) continue;
                    mine_group_t* sector = &mine_groups.sectors[i][j][k];
                    mine_t** mines = sector->mines;
                    for (int m = 0; m < sector->n_mines; m++) {
                        mine_t* mine = mines[m];
                        if (mine->status != MINE_STATUS_EXPLODED) {
                            point_t pos = mine->pos;
                            if (distance(pos, status.ship_pos) < SHIP_RADIUS_MAX+MINE_RADIUS) {
                                bool collision_detected;
                                if (game.shield_health > 0.0) {
                                    collision_detected =
                                        object_sphere_collision(status.collision_matrix, status.ship_pos, pos,
                                            status.shield_ellipsoid_points, SHIELD_ELLIPSOID_POINTS,
                                            MINE_RADIUS);
                                } else {
                                    collision_detected =
                                        object_sphere_collision(status.collision_matrix, status.ship_pos, pos,
                                            models.ship_points, models.n_ship_points,
                                            MINE_RADIUS);
                                }
                                if (collision_detected) {
                                    mine->status = MINE_STATUS_EXPLODED; // TODO: implement explosion
                                    if (n_exploded_mines < MINE_EXPLODED_BUF_SIZE) {
                                        exploded_mines[n_exploded_mines] = mine;
                                        n_exploded_mines++;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        for (int i = 0; i < n_exploded_mines; i++) {
            mine_t* mine = exploded_mines[i];
            point_t pos = mine->pos;
            double dist = distance(pos, status.ship_pos);
            if (dist < MINE_EXPLOSION_RANGE) {
                sound_set_source_position(sound_sources.mine, pos);
                sound_play(sound_buffers.mine, sound_sources.mine, MINE_SOUND_GAIN, false);
                if (game.shield_health > 0.0) {
                    #if !DISABLE_DAMAGE
                    game.shield_health -= MINE_EXPLOSION_SHIELD_DECREMENT;
                    game.shield_down_timestamp = status.game_clock;
                    #endif
                    vector_t mine_rel_pos = translate(pos, status.ship_pos);
                    mine_rel_pos = normalize(mine_rel_pos);
                    vector_t move_vector = scalar_vector_product(-MINE_EXPLOSION_STRENGTH, mine_rel_pos);
                    addMoveVector(move_vector);
                } else {
                    gameOver(false);
                    break;
                }
            }
        }

        #endif


        /* TURRETS */

        for (int i = 0; i < world.n_turrets; i++) {
            turret_t* turret = &world.turrets[i];
            point_t pos = turret->pos;
            double dist = distance(pos, status.ship_pos);
            if (dist < TURRET_RADIUS+SHIP_RADIUS_MAX) {
                bool collision_detected;
                if (game.shield_health > 0.0) {
                    collision_detected =
                        object_sphere_collision(status.collision_matrix, status.ship_pos, pos,
                            status.shield_ellipsoid_points, SHIELD_ELLIPSOID_POINTS,
                            TURRET_RADIUS);
                } else {
                    collision_detected =
                        object_sphere_collision(status.collision_matrix, status.ship_pos, pos,
                            models.ship_points, models.n_ship_points,
                            TURRET_RADIUS);
                }
                if (collision_detected) {
                    #if !DISABLE_DAMAGE
                    game.shield_health = -1.0;
                    gameOver(false);
                    #endif
                }
            }
        }

        #if DISABLE_TURRETS == 0
        if (game.phase == PHASE_ACTION) {
            for (int i = 0; i < world.n_turrets; i++) {
                turret_t* turret = &world.turrets[i];
                point_t pos = turret->pos;
                double dist = distance(pos, status.ship_pos);
                if (turret->status == TURRET_STATUS_IDLE) {
                    if (dist < TURRET_ENGAGE_DIST) {
                        turret->status = TURRET_STATUS_ENGAGED;
                        turret->fire_timestamp = status.game_clock;
                    }
                } else if (turret->status == TURRET_STATUS_ENGAGED) {
                    if (dist > TURRET_ENGAGE_DIST) {
                        turret->status = TURRET_STATUS_IDLE;
                    } else {
                        timestamp_t last_fire_timestamp = turret->fire_timestamp;
                        seconds_t time_diff = usToSecs(status.game_clock - last_fire_timestamp);
                        if (time_diff > SHOT_INTERVAL && !game.invisible_mode) {
                            vector_t shot_direction =
                                calculate_shot_direction(status.ship_pos, status.vector_look, status.throttle,
                                                         pos, SHOT_SPEED);
                            shot_t shot = createShot(pos, shot_direction, status.game_clock);
                            addShot(shot);
                            turret->fire_timestamp = status.game_clock;
                            sound_play(sound_buffers.shot, sound_sources.turrets[i], 1.0, false);
                        }
                    }
                }
            }
        }
        #endif

    }

    /* SHOTS */

    if (game.phase != PHASE_GAMEOVER_WIN) {

        shot_entry_t* shot_e = game.shots_list;
        shot_entry_t* shot_prev_e = NULL;

        while (shot_e != NULL) {

            shot_t* curr_shot = &shot_e->shot;

            // update position
            seconds_t shot_delta_t = usToSecs(status.game_clock - curr_shot->timestamp);
            vector_t move_vector = scalar_vector_product(SHOT_SPEED * shot_delta_t, curr_shot->direction);
            curr_shot->prev_position = curr_shot->position;
            curr_shot->position = vectors_sum(curr_shot->starting_point, move_vector);

            bool remove_shot = curr_shot->remove;

            if (!remove_shot) {

                /* START COLLISION TEST */
                double error = distance(curr_shot->prev_position, curr_shot->position);
                double dist = min(
                    distance(curr_shot->prev_position, status.ship_prev_pos),
                    distance(curr_shot->position, status.ship_pos));

                if (dist < max(SHIP_RADIUS_MAX+SHOT_RADIUS, error)) {

                    bool collision_detected = false;

                    bool shield_mode;
                    if (game.shield_health > 0.0) shield_mode = true;
                    else shield_mode = false;

                    #if PRINT_COLLISION_REPORT
                    printf("\nPOSSIBLE COLLISION, PERFORMING INTERPOLATION...\n");
                    printf("(shield_mode: %d; error: %f)\n", shield_mode, error);
                    #endif

                    vector_t ship_pos_delta = create_vector(
                            (status.ship_pos.x - status.ship_prev_pos.x) / INTERPOLATION_GRANULARITY,
                            (status.ship_pos.y - status.ship_prev_pos.y) / INTERPOLATION_GRANULARITY,
                            (status.ship_pos.z - status.ship_prev_pos.z) / INTERPOLATION_GRANULARITY);
                    vector_t shot_pos_delta = create_vector(
                            (curr_shot->position.x - curr_shot->prev_position.x) / INTERPOLATION_GRANULARITY,
                            (curr_shot->position.y - curr_shot->prev_position.y) / INTERPOLATION_GRANULARITY,
                            (curr_shot->position.z - curr_shot->prev_position.z) / INTERPOLATION_GRANULARITY);

                    point_t ship_pos_interpolated = status.ship_prev_pos;
                    point_t shot_pos_interpolated = curr_shot->prev_position;

                    for (int i = 0; i < INTERPOLATION_GRANULARITY; i++) {
                        if (shield_mode) {
                            collision_detected = ellipsoid_small_sphere_collision(
                                status.collision_matrix, ship_pos_interpolated, shot_pos_interpolated,
                                SHIP_RADIUS_X, SHIP_RADIUS_Y, SHIP_RADIUS_Z, SHOT_RADIUS);
                        } else {
                            collision_detected = object_small_sphere_collision(
                                status.collision_matrix, ship_pos_interpolated, shot_pos_interpolated,
                                &models.ship_boxes);
                        }
                        if (collision_detected) break;
                        ship_pos_interpolated = vectors_sum(ship_pos_interpolated, ship_pos_delta);
                        shot_pos_interpolated = vectors_sum(shot_pos_interpolated, shot_pos_delta);
                    }

                    if (collision_detected) {
                        #if PRINT_COLLISION_REPORT
                        printf("\tCOLLISION DETECTED!\n\n");
                        #endif
                        if (shield_mode) {
                            #if !DISABLE_DAMAGE
                            game.shield_health -= SHOT_STRENGHT;
                            sound_play(sound_buffers.shield, sound_sources.shield, 1.0, false);
                            game.shield_down_timestamp = status.game_clock;
                            #endif
                            vector_t shot_move_vector = scalar_vector_product(SHOT_STRENGHT, curr_shot->direction);
                            addMoveVector(shot_move_vector);
                            curr_shot->position = shot_pos_interpolated;
                            curr_shot->pending_remove = true;
                        } else {
                            remove_shot = true;
                            gameOver(false);
                        }
                    }
                }
                /* END COLLISION TEST */

            }

            if (remove_shot || distance(curr_shot->position, curr_shot->starting_point) > SHOT_EXPIRE_DISTANCE) {
                shot_entry_t* next = shot_e->next;
                if (shot_e == game.shots_list) { // is the first entry
                    free(shot_e);
                    game.shots_list = shot_e = next;
                } else {
                    shot_prev_e->next = next;
                    free(shot_e);
                    shot_e = next;
                }
                game.n_shots--;
            } else {
                shot_prev_e = shot_e;
                shot_e = shot_e->next; // update pointer
            }

        }


        /* UPDATE MOVE VECTORS AND SHIP POSITION ACCORDINGLY */

        if (game.phase == PHASE_ACTION) {

            vector_entry_t* vector_e = game.move_vectors_list;
            vector_entry_t* vector_prev_e = NULL;

            while (vector_e != NULL) {

                vector_t* expl_vector = &vector_e->vector;
                double magnitude = vector_magnitude(*expl_vector);

                if (magnitude > SMALL_DELTA) {

                    #if DISABLE_MOVE_VECTORS == 0
                    vector_t move_vector = scalar_vector_product(delta_t, *expl_vector);
                    status.ship_pos = vectors_sum(status.ship_pos, move_vector);
                    #endif

                    double new_magnitude = magnitude - EXPLOSION_VECTOR_DECREMENT * delta_t;
                    if (new_magnitude < SMALL_DELTA) new_magnitude = 0.0;
                    *expl_vector = resize(*expl_vector, new_magnitude);

                    vector_prev_e = vector_e;
                    vector_e = vector_e->next; // update pointer

                } else { // delete the entry

                    vector_entry_t* next = vector_e->next;
                    if (vector_e == game.move_vectors_list) { // is the first entry
                        free(vector_e);
                        game.move_vectors_list = vector_e = next;
                    } else {
                        vector_prev_e->next = next;
                        free(vector_e);
                        vector_e = next;
                    }

                }

            }


            /* CHECKPOINTS */
            for (int i = 0; i < world.n_checkpoints; i++) {
                checkpoint_t* checkpoint = &world.checkpoints[i];
                if (checkpoint->status != CHECKPOINT_STATUS_REACHED) {
                    point_t pos = checkpoint->pos;
                    if (distance(pos, status.ship_pos) < SHIP_RADIUS_MAX+CHECKPOINT_RADIUS) {
                        bool collision_detected =
                            object_sphere_collision(status.collision_matrix, status.ship_pos, pos,
                                                    status.shield_ellipsoid_points, SHIELD_ELLIPSOID_POINTS,
                                                    CHECKPOINT_RADIUS);
                        if (collision_detected) { // checkpoint reached?
                            checkpoint->status = CHECKPOINT_STATUS_REACHED;
                            game.remaining_seconds += TIME_BONUS;
                            game.reached_checkpoints_no++;
                            if (game.reached_checkpoints_no == world.n_checkpoints)
                                gameOver(true);
                            else
                                sound_play(sound_buffers.checkpoint, sound_sources.checkpoint, 1.0, false);
                            break;
                        }
                    }
                }
            }

        }

        for (int i = 0; i < world.n_checkpoints; i++) {
            checkpoint_alpha_t* checkpoint_alpha = &status.checkpoints_alpha[i];
            if (checkpoint_alpha->direction > 0) {
                checkpoint_alpha->value += CHECKPOINT_FLASH_SPEED * delta_t;
                if (checkpoint_alpha->value > CHECKPOINT_MAX_ALPHA) {
                    checkpoint_alpha->value = CHECKPOINT_MAX_ALPHA;
                    checkpoint_alpha->direction = -1;
                }
            } else {
                checkpoint_alpha->value -= CHECKPOINT_FLASH_SPEED * delta_t;
                if (checkpoint_alpha->value < CHECKPOINT_MIN_ALPHA) {
                    checkpoint_alpha->value = CHECKPOINT_MIN_ALPHA;
                    checkpoint_alpha->direction = 1;
                }
            }
        }

    }

    if (game.phase == PHASE_GAMEOVER_LOSE) {

        explosion_update(status.explosion, delta_t_us);

    } else if (game.phase == PHASE_ACTION || game.phase == PHASE_GAMEOVER_WIN) {

        // Calculate camera position
        double camera_dist = status.camera_distance + status.throttle * CAMERA_THROTTLE_FACTOR;
        vector_t ship_reverse = scalar_vector_product(-1.0, status.vector_look);
        vector_t vector_right = cross_product(status.vector_look, status.vector_up);
        double camera_angle = CAMERA_BASE_ANGLE + status.throttle_y * CAMERA_THROTTLE_Y_FACTOR;
        ship_reverse = rotate(ship_reverse, vector_right, -camera_angle);
        ship_reverse = scalar_vector_product(camera_dist, ship_reverse);
        status.camera_pos = vectors_sum(status.ship_pos, ship_reverse);

        // Calculate planet rotation status
        status.planet_rot_x = PLANET_ROT_DEGREES * (status.ship_pos.x / (world.width / 2.0));
        status.planet_rot_y = PLANET_ROT_DEGREES * (status.ship_pos.y / (world.height / 2.0));

    }

    if (game.phase == PHASE_GAMEOVER_LOSE || game.phase == PHASE_GAMEOVER_WIN) {
        double gameover_delta_t = usToSecs(status.game_clock - status.gameover_timestamp);
        if (gameover_delta_t > GAMEOVER_LABEL_DELAY) {
            status.gameover_opacity += delta_t * GAMEOVER_LABEL_APPEAR_SPEED;
        }
        if (gameover_delta_t > PLAY_AGAIN_LABEL_DELAY) status.show_play_again = true;
    }

    #if FRUSTUM_CULLING
    generate_transform_matrix(status.cull_matrix, status.vector_look, status.vector_up);
    #endif

    #if PRINT_STATUS
    if (usToSecs(timestamp - debug.prev_print_timestamp) > PRINT_DEBUG_INTERVAL) {
        debug.prev_print_timestamp = timestamp;
        printDebugInfo();
    }
    #endif

    glutPostRedisplay();

}

/**************************************/

void endGame() {
    music_stop(&music.context);
    sound_stop(sound_sources.alarm);
}

/**************************************/

void pauseGame(bool paused) {
    if (paused) {
        music_pause(&music.context);
        sound_pause(sound_sources.alarm);
    } else {
        music_resume(&music.context);
        sound_resume(sound_sources.alarm);
    }
}

/**************************************/

void gameOver(bool win) {
    endGame();
    if (win) {
        game.phase = PHASE_GAMEOVER_WIN;
        sound_play(sound_buffers.gameover_win, sound_sources.gameover, 1.0, false);
    } else {
        game.phase = PHASE_GAMEOVER_LOSE;
        sound_play(sound_buffers.gameover_lose, sound_sources.gameover, 1.0, false);
    }
    status.gameover_timestamp = status.game_clock;
}

/**************************************/

void addMoveVector(vector_t new_vector) {
    vector_entry_t* vector_e = (vector_entry_t*) malloc(sizeof(vector_entry_t));
    vector_e->vector = new_vector;
    vector_e->next = game.move_vectors_list;
    game.move_vectors_list = vector_e;
}

/**************************************/

void addShot(shot_t new_shot) {
    shot_entry_t* shot_e = (shot_entry_t*) malloc(sizeof(shot_entry_t));
    shot_e->shot = new_shot;
    shot_e->next = game.shots_list;
    game.shots_list = shot_e;
    game.n_shots++;
}

/**************************************/

void findSector(point_t point, int* sector_x, int* sector_y, int* sector_z) {
    *sector_x = SECTOR_DIV_X-1;
    *sector_y = SECTOR_DIV_Y-1;
    *sector_z = SECTOR_DIV_Z-1;
    for (int i = 0; i < SECTOR_DIV_X; i++) {
        if (point.x < mine_groups.width_sectors[i]+SMALL_DELTA) {
            *sector_x = i;
            break;
        }
    }
    for (int j = 0; j < SECTOR_DIV_Y; j++) {
        if (point.y < mine_groups.height_sectors[j]+SMALL_DELTA) {
            *sector_y = j;
            break;
        }
    }
    for (int k = 0; k < SECTOR_DIV_Z; k++) {
        if (point.z < mine_groups.depth_sectors[k]+SMALL_DELTA) {
            *sector_z = k;
            break;
        }
    }
}

/**************************************/

/**************************************/
/****       DISPLAY FUNCTIONS      ****/
/**************************************/

void reshape(int width, int height) {

    status.viewport_width = width;
    status.viewport_height = height;

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    status.perspective_near = PERSPECTIVE_NEAR;
    status.perspective_far = 1.1 * max_v(3, world.width, world.height, world.depth);
    status.aspect_ratio = (double) width / (double) height;
    status.perspective_fovy = PERSPECTIVE_FOVY;
    status.perspective_fovx = status.aspect_ratio * status.perspective_fovy;
    gluPerspective(PERSPECTIVE_FOVY, status.aspect_ratio, status.perspective_near, status.perspective_far);

    glMatrixMode(GL_MODELVIEW);

}

/**************************************/

void draw() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* STARMAP AND PLANET */

    glLoadIdentity();
    lookAt(ORIGIN, status.vector_look, status.vector_up);
    glCallList(lists.starmap);
    drawRotatedPlanet(lists.planet, status.planet_rot_x, status.planet_rot_y);

    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT); // reset depth buffer

    lookAt(status.camera_pos, status.vector_look, status.vector_up);

	glEnable(GL_LIGHTING);
    glEnable(PLANET_LIGHT);
    glLightfv(PLANET_LIGHT, GL_POSITION, planet_light_position);


    /* TURRETS */
    for (int i = 0; i < world.n_turrets; i++) {
        turret_t* turret = &world.turrets[i];
        point_t pos = turret->pos;
        #if FRUSTUM_CULLING
        if (cull_object(status.cull_matrix,
                        status.perspective_fovx, status.perspective_fovy, status.perspective_far,
                        TURRET_RADIUS, status.camera_pos, pos)) continue;
        #endif
        double dist = distance(pos, status.camera_pos);
        glPushMatrix();
            glTranslated(pos.x, pos.y, pos.z);
            if (dist > TURRET_DETAIL_LOW_THRESHOLD) glCallList(lists.turret_low);
            else if (dist > TURRET_DETAIL_MED_THRESHOLD) glCallList(lists.turret_med);
            else if (dist > TURRET_DETAIL_HIGH_THRESHOLD) glCallList(lists.turret_high);
            else glCallList(lists.turret_highest);
        glPopMatrix();
    }


    /* SHOTS */

    glDisable(GL_LIGHTING);

    shot_entry_t* shot_e = game.shots_list;
    while (shot_e != NULL) {
        shot_t* shot = &shot_e->shot;
        if (shot->pending_remove) shot->remove = true;
        point_t shot_pos = shot->position;
        #if FRUSTUM_CULLING
        if (cull_object(status.cull_matrix,
                        status.perspective_fovx, status.perspective_fovy, status.perspective_far,
                        SHOT_RADIUS, status.camera_pos, shot_pos)) {
            shot_e = shot_e->next; // update pointer
            continue;
        }
        #endif
        glPushMatrix();
            glTranslated(shot_pos.x, shot_pos.y, shot_pos.z);
            glCallList(lists.shot);
        glPopMatrix();
        shot_e = shot_e->next; // update pointer
    }

    glEnable(GL_LIGHTING);

    // insertion sort
    int sorted_checkpoints_n = 0;
    int* sorted_checkpoints = status.sorted_checkpoints;
    for (int checkpoint_id = 0; checkpoint_id < world.n_checkpoints; checkpoint_id++) {
        checkpoint_t* checkpoint_curr = &world.checkpoints[checkpoint_id];
        if (checkpoint_curr->status != CHECKPOINT_STATUS_REACHED) {
            point_t pos = checkpoint_curr->pos;
            #if FRUSTUM_CULLING
            if (cull_object(status.cull_matrix,
                            status.perspective_fovx, status.perspective_fovy, status.perspective_far,
                            CHECKPOINT_RADIUS, status.camera_pos, pos)) continue;
            #endif
            double checkpoint_dist = distance(pos, status.camera_pos);
            int j = sorted_checkpoints_n-1;
            while (j >= 0) {
                double curr_dist = distance(world.checkpoints[sorted_checkpoints[j]].pos, status.camera_pos);
                if (checkpoint_dist < curr_dist) break;
                sorted_checkpoints[j+1] = sorted_checkpoints[j];
                j--;
            }
            sorted_checkpoints[j+1] = checkpoint_id;
            sorted_checkpoints_n++;
        }
    }

    // Set blending function
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // painter's algorithm
    for (int i = 0; i < sorted_checkpoints_n; i++) {
        int checkpoint_id = sorted_checkpoints[i];
        checkpoint_t* checkpoint = &world.checkpoints[checkpoint_id];
        point_t pos = checkpoint->pos;
        double dist = distance(pos, status.camera_pos);
        double alpha = status.checkpoints_alpha[checkpoint_id].value;
        glPushMatrix();
            glTranslated(pos.x, pos.y, pos.z);
            if (dist > CHECKPOINT_DETAIL_LOW_THRESHOLD)
                drawCheckpoint(lists.checkpoint_low, alpha);
            else if (dist > CHECKPOINT_DETAIL_MED_THRESHOLD)
                drawCheckpoint(lists.checkpoint_med, alpha);
            else
                drawCheckpoint(lists.checkpoint_high, alpha);
        glPopMatrix();
    }

    glDisable(GL_BLEND);


    /* SHIP AND SHIP SHIELD */

    if (game.phase != PHASE_GAMEOVER_LOSE) {

        bool draw_shield = false;
        double magnitude_sum = 0.0;

        vector_entry_t* vector_e = game.move_vectors_list;
        glDisable(GL_LIGHTING);
        while (vector_e != NULL) {
            vector_t move_vector = vector_e->vector;
            double magnitude = vector_magnitude(move_vector);
            if (magnitude > SMALL_DELTA) {
                magnitude_sum += magnitude;
                draw_shield = true;
                #if DRAW_DEBUG_AIDS
                vector_t move_vector_abs = vectors_sum(status.ship_pos, move_vector);
                drawSegment(status.ship_pos, move_vector_abs);
                #endif
            }
            vector_e = vector_e->next;
        }
        glEnable(GL_LIGHTING);

        /* SHIP */
        bool draw_ship = (game.phase != PHASE_GAMEOVER_LOSE || status.explosion->phase < 1);

        if (draw_ship) {
            glDisable(PLANET_LIGHT);
            glEnable(FAKE_SHIP_LIGHT);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawShip(lists.ship_body,
                 lists.ship_weapons,
                 lists.ship_glass,
                 lists.ship_decorations,
                 lists.ship_glow,
                 status.ship_pos,
                 status.vector_look,
                 status.vector_up,
                 status.ship_inclination,
                 status.throttle / THROTTLE_AFTERBURNER,
                 status.ship_opacity,
                 DRAW_DEBUG_AIDS,
                 &models.ship_boxes);
            glDisable(GL_BLEND);
            glDisable(FAKE_SHIP_LIGHT);
            glEnable(PLANET_LIGHT);
        }

        /* SHIP SHIELD */
        if (draw_shield) {
            double intensity = min(1.0, magnitude_sum / SHIELD_MAGNITUDE_SCALE_MAX);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            drawShipShield(status.ship_pos, status.vector_look, status.vector_up,
                       status.ship_inclination, intensity);
            glDisable(GL_BLEND);
        }

    } else if (game.phase == PHASE_GAMEOVER_LOSE) {

        glDisable(GL_LIGHTING);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        vector_t vector_up_incl = rotate(status.vector_up, status.vector_look, -status.ship_inclination);
        drawExplosion(status.explosion, status.ship_pos, status.vector_look, vector_up_incl);
        glDisable(GL_BLEND);
        glEnable(GL_LIGHTING);

    }


    /* MINES */

    if (game.phase != PHASE_GAMEOVER_LOSE && !game.invisible_mode) {

        glDisable(PLANET_LIGHT);

        float spotlight_position[] = {status.ship_pos.x, status.ship_pos.y, status.ship_pos.z, 1.0};
        glLightfv(SPACESHIP_SPOTLIGHT, GL_POSITION, spotlight_position);
        float spotlight_direction[] = {status.vector_look.x, status.vector_look.y, status.vector_look.z};
        glLightf(SPACESHIP_SPOTLIGHT, GL_SPOT_CUTOFF, SPOTLIGHT_CUTOFF);
        glLighti(SPACESHIP_SPOTLIGHT, GL_SPOT_EXPONENT, SPOTLIGHT_EXPONENT);
        glLightfv(SPACESHIP_SPOTLIGHT, GL_SPOT_DIRECTION, spotlight_direction);
        glEnable(SPACESHIP_SPOTLIGHT);

        // Set additive blending
        #if DRAW_DEBUG_AIDS == 0
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        #endif

        int sector_x, sector_y, sector_z;
        findSector(status.ship_pos, &sector_x, &sector_y, &sector_z);
        int num_sectors_x = ceil(MINE_DRAW_THRESHOLD / mine_groups.width_step);
        int num_sectors_y = ceil(MINE_DRAW_THRESHOLD / mine_groups.height_step);
        int num_sectors_z = ceil(MINE_DRAW_THRESHOLD / mine_groups.depth_step);

        static const double cull_angle = SPOTLIGHT_CUTOFF * 2.0;

        for (int i = sector_x-num_sectors_x; i <= sector_x+num_sectors_x; i++) {
            if (i < 0 || i > SECTOR_DIV_X-1) continue;
            for (int j = sector_y-num_sectors_y; j <= sector_y+num_sectors_y; j++) {
                if (j < 0 || j > SECTOR_DIV_Y-1) continue;
                for (int k = sector_z-num_sectors_z; k <= sector_z+num_sectors_z; k++) {
                    if (k < 0 || k > SECTOR_DIV_Z-1) continue;
                    mine_group_t* sector = &mine_groups.sectors[i][j][k];
                    mine_t** mines = sector->mines;
                    for (int m = 0; m < sector->n_mines; m++) {
                        mine_t* mine = mines[m];
                        if (mine->status != MINE_STATUS_EXPLODED) {
                            point_t pos = mine->pos;
                            #if FRUSTUM_CULLING
                            if (cull_object(status.cull_matrix, cull_angle, cull_angle, MINE_DRAW_THRESHOLD,
                                            MINE_RADIUS, status.camera_pos, pos)) continue;
                            #endif
                            double dist = distance(pos, status.camera_pos);
                            glPushMatrix();
                                glTranslated(pos.x, pos.y, pos.z);
                                if (dist > MINE_DETAIL_LOW_THRESHOLD) glCallList(lists.mine_low);
                                else if (dist > MINE_DETAIL_MED_THRESHOLD) glCallList(lists.mine_med);
                                else if (dist > MINE_DETAIL_HIGH_THRESHOLD) glCallList(lists.mine_high);
                                else glCallList(lists.mine_highest);
                            glPopMatrix();
                        }
                    }
                }
            }
        }

        glDisable(GL_BLEND);

        glDisable(SPACESHIP_SPOTLIGHT);

    }

    glDisable(GL_LIGHTING);

    #if DRAW_DEBUG_AIDS
    drawAxes(world.width / 2.0, world.height / 2.0, world.depth / 2.0);
    drawWorldBox(world.width, world.height, world.depth);
    drawShipShieldPoints(status.shield_ellipsoid_points, SHIELD_ELLIPSOID_POINTS,
                         status.ship_pos, status.vector_look, status.vector_up,
                         status.ship_inclination);
    #endif

    drawCockpit();


	glutSwapBuffers();
	glutPostRedisplay();

}

/**************************************/

void drawCockpit() {

    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); // push projection
    glLoadIdentity();
    gluOrtho2D(0, status.viewport_width, status.viewport_height, 0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix(); // push modelview
    glLoadIdentity();


    /********************/
    /****** SHIELD ******/
    /********************/

    int shield_fill_amount = (int) ((game.shield_health / MAX_SHIELD) * SHIELD_METER_HEIGHT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float* shield_meter_border_color;
    if (shield_fill_amount >= 0) shield_meter_border_color = (float*)shield_meter_border_color1;
    else shield_meter_border_color = (float*)shield_meter_border_color2;

    draw2DWireRectangle(SHIELD_METER_PADDING_LEFT,
                      status.viewport_height-SHIELD_METER_HEIGHT-SHIELD_METER_PADDING_BOTTOM,
                      SHIELD_METER_WIDTH,
                      SHIELD_METER_HEIGHT,
                      shield_meter_border_color,
                      2.0);

    draw2DTexturedRectangle(SHIELD_ICON_PADDING_LEFT,
                          status.viewport_height-SHIELD_METER_HEIGHT-SHIELD_METER_PADDING_BOTTOM-SHIELD_ICON_HEIGHT-SHIELD_ICON_PADDING_BOTTOM,
                          SHIELD_ICON_WIDTH,
                          SHIELD_ICON_HEIGHT,
                          shield_meter_border_color,
                          textures.shield,
                          0.0, 1.0, 0.0, 1.0);

    if (shield_fill_amount > 0) {
        draw2DSolidRectangle(SHIELD_METER_PADDING_LEFT,
                           status.viewport_height-shield_fill_amount-SHIELD_METER_PADDING_BOTTOM,
                           SHIELD_METER_WIDTH,
                           shield_fill_amount,
                           shield_meter_color);
    }


    /********************/
    /**** AFTERBURNER ***/
    /********************/

    int afterburner_fill_amount = (int) ((game.afterburner_fuel / MAX_AFTERBURNER_FUEL) * AFTERBURNER_METER_HEIGHT);

    draw2DWireRectangle(status.viewport_width-AFTERBURNER_METER_PADDING_RIGHT-AFTERBURNER_METER_WIDTH,
                      status.viewport_height-AFTERBURNER_METER_HEIGHT-AFTERBURNER_METER_PADDING_BOTTOM,
                      AFTERBURNER_METER_WIDTH,
                      AFTERBURNER_METER_HEIGHT,
                      afterburner_meter_border_color,
                      2.0);

    draw2DTexturedRectangle(status.viewport_width-AFTERBURNER_ICON_WIDTH-AFTERBURNER_ICON_PADDING_RIGHT,
                          status.viewport_height-AFTERBURNER_METER_HEIGHT-AFTERBURNER_ICON_HEIGHT-AFTERBURNER_ICON_PADDING_BOTTOM-AFTERBURNER_METER_PADDING_BOTTOM,
                          AFTERBURNER_ICON_WIDTH,
                          AFTERBURNER_ICON_HEIGHT,
                          afterburner_meter_border_color,
                          textures.afterburner,
                          0.0, 1.0, 0.0, 1.0);

    if (afterburner_fill_amount > 0) {
        draw2DSolidRectangle(status.viewport_width-AFTERBURNER_METER_PADDING_RIGHT-AFTERBURNER_METER_WIDTH,
                           status.viewport_height-afterburner_fill_amount-AFTERBURNER_METER_PADDING_BOTTOM,
                           AFTERBURNER_METER_WIDTH,
                           afterburner_fill_amount,
                           afterburner_meter_color);
    }


    /********************/
    /*** INVISIBILITY ***/
    /********************/

    int invisibility_fill_amount = (int) ((game.invisibility_fuel / MAX_INVISIBILITY_FUEL) * INVISIBILITY_METER_HEIGHT);

    draw2DWireRectangle(status.viewport_width-AFTERBURNER_METER_PADDING_RIGHT-AFTERBURNER_METER_WIDTH-INVISIBILITY_METER_PADDING_RIGHT-INVISIBILITY_METER_WIDTH,
                      status.viewport_height-INVISIBILITY_METER_HEIGHT-INVISIBILITY_METER_PADDING_BOTTOM,
                      INVISIBILITY_METER_WIDTH,
                      INVISIBILITY_METER_HEIGHT,
                      invisibility_meter_border_color,
                      2.0);

    draw2DTexturedRectangle(status.viewport_width-INVISIBILITY_ICON_WIDTH-INVISIBILITY_ICON_PADDING_RIGHT-AFTERBURNER_ICON_WIDTH-AFTERBURNER_ICON_PADDING_RIGHT,
                          status.viewport_height-INVISIBILITY_METER_HEIGHT-INVISIBILITY_ICON_HEIGHT-INVISIBILITY_ICON_PADDING_BOTTOM-INVISIBILITY_METER_PADDING_BOTTOM,
                          AFTERBURNER_ICON_WIDTH,
                          AFTERBURNER_ICON_HEIGHT,
                          invisibility_meter_border_color,
                          textures.invisibility,
                          0.0, 1.0, 0.0, 1.0);

    if (invisibility_fill_amount > 0) {
        draw2DSolidRectangle(status.viewport_width-AFTERBURNER_METER_PADDING_RIGHT-AFTERBURNER_METER_WIDTH-INVISIBILITY_METER_PADDING_RIGHT-INVISIBILITY_METER_WIDTH,
                           status.viewport_height-invisibility_fill_amount-INVISIBILITY_METER_PADDING_BOTTOM,
                           INVISIBILITY_METER_WIDTH,
                           invisibility_fill_amount,
                           invisibility_meter_color);
    }



    /********************************/
    /***** REMAINING TIME LABEL *****/
    /********************************/

    draw2DTexturedRectangle(status.viewport_width-REMAINING_TIME_LABEL_WIDTH-REMAINING_TIME_LABEL_PADDING_RIGHT,
                          REMAINING_TIME_LABEL_PADDING_TOP,
                          REMAINING_TIME_LABEL_WIDTH,
                          REMAINING_TIME_LABEL_HEIGHT,
                          remaining_time_label_color,
                          textures.remaining_time,
                          0.0, REMAINING_TIME_LABEL_TEXTURE_COORD, 0.0, 1.0);


    /*************************/
    /***** REMAINING TIME ****/
    /*************************/

    static const double scale_ratio_big = (double)REMAINING_TIME_HEIGHT_BIG / (double)COUNTDOWN_TEXTURE_HEIGHT;
    static const double scale_ratio_small = (double)REMAINING_TIME_HEIGHT_SMALL / (double)COUNTDOWN_TEXTURE_HEIGHT;

    char countdown_text[256];
    double remaining_seconds = max(0.0, game.remaining_seconds);
    sprintf(countdown_text, "%.1f", remaining_seconds);
    int c_length = strlen(countdown_text);

    int right_offset = REMAINING_TIME_PADDING_RIGHT;
    bool small = true;

    for (int i = c_length-1; i >= 0; i--) {
        char curr_char = countdown_text[i];
        int index = 0;
        switch (curr_char) {
            case '1': index = 0; break;
            case '2': index = 1; break;
            case '3': index = 2; break;
            case '4': index = 3; break;
            case '5': index = 4; break;
            case '6': index = 5; break;
            case '7': index = 6; break;
            case '8': index = 7; break;
            case '9': index = 8; break;
            case '0': index = 9; break;
            case '.': index = 10; break;
            case ':': index = 11; break;
        }
        int char_width, char_height, extra_padding;
        if (index == 10) small = false;
        if (small) {
            char_width = (int) (scale_ratio_small * (double)COUNTDOWN_TEXTURE_WIDTH * textures.numbers_coords[index][2]);
            char_height = REMAINING_TIME_HEIGHT_SMALL;
            extra_padding = REMAINING_TIME_HEIGHT_BIG-REMAINING_TIME_HEIGHT_SMALL-3;
        } else {
            char_width = (int) (scale_ratio_big * (double)COUNTDOWN_TEXTURE_WIDTH * textures.numbers_coords[index][2]);
            char_height = REMAINING_TIME_HEIGHT_BIG;
            extra_padding = 0;
        }
        right_offset += char_width;

        draw2DTexturedRectangle(status.viewport_width-right_offset,
                              REMAINING_TIME_PADDING_TOP+REMAINING_TIME_LABEL_HEIGHT+extra_padding,
                              char_width,
                              char_height,
                              remaining_time_color,
                              textures.numbers,
                              textures.numbers_coords[index][0],
                              textures.numbers_coords[index][0]+textures.numbers_coords[index][2],
                              0.0,
                              1.0);

        if (index != 10) {
            if (small) right_offset += (REMAINING_TIME_SPACING_SMALL - char_width);
            else right_offset += (REMAINING_TIME_SPACING_BIG - char_width);
        }
    }


    /**************************************/
    /***** REMAINING CHECKPOINTS LABEL ****/
    /**************************************/

    draw2DTexturedRectangle(REMAINING_CHECKPOINTS_LABEL_PADDING_LEFT,
                          REMAINING_CHECKPOINTS_LABEL_PADDING_TOP,
                          REMAINING_CHECKPOINTS_LABEL_WIDTH,
                          REMAINING_CHECKPOINTS_LABEL_HEIGHT,
                          remaining_checkpoints_label_color,
                          textures.remaining_checkpoints,
                          0.0, REMAINING_CHECKPOINTS_LABEL_TEXTURE_COORD, 0.0, 1.0);



    /********************************/
    /***** REMAINING CHECKPOINTS ****/
    /********************************/

    static const double scale_ratio = (double)REMAINING_CHECKPOINTS_HEIGHT / (double)COUNTDOWN_TEXTURE_HEIGHT;

    char checkpoints_text[256];
    int remaining_checkpoints = world.n_checkpoints - game.reached_checkpoints_no;
    sprintf(checkpoints_text, "%d", remaining_checkpoints);
    int rc_length = strlen(checkpoints_text);
    int left_offset = REMAINING_CHECKPOINTS_PADDING_LEFT;

    for (int i = 0; i < rc_length; i++) {
        char curr_char = checkpoints_text[i];
        int index = 0;
        switch (curr_char) {
            case '1': index = 0; break;
            case '2': index = 1; break;
            case '3': index = 2; break;
            case '4': index = 3; break;
            case '5': index = 4; break;
            case '6': index = 5; break;
            case '7': index = 6; break;
            case '8': index = 7; break;
            case '9': index = 8; break;
            case '0': index = 9; break;
        }

        int char_width = (int) (scale_ratio * (double)COUNTDOWN_TEXTURE_WIDTH * textures.numbers_coords[index][2]);

        draw2DTexturedRectangle(left_offset,
                              REMAINING_CHECKPOINTS_PADDING_TOP+REMAINING_CHECKPOINTS_LABEL_HEIGHT,
                              char_width,
                              REMAINING_CHECKPOINTS_HEIGHT,
                              remaining_checkpoints_color,
                              textures.numbers,
                              textures.numbers_coords[index][0],
                              textures.numbers_coords[index][0]+textures.numbers_coords[index][2],
                              0.0,
                              1.0);

        left_offset += char_width;
    }


    /********************/
    /******* PAUSE ******/
    /********************/

    if (game.paused) {
        draw2DTexturedRectangle((status.viewport_width-PAUSE_LABEL_WIDTH) / 2.0,
                              (status.viewport_height-PAUSE_LABEL_HEIGHT) / 2.0,
                              PAUSE_LABEL_WIDTH,
                              PAUSE_LABEL_HEIGHT,
                              pause_label_color,
                              textures.pause,
                              0.0, 1.0, 0.0, 1.0);
    }



    /***********************/
    /******* GAMEOVER ******/
    /***********************/

    bool gameover = false;

    if (game.phase == PHASE_GAMEOVER_LOSE) {

        float gameover_win_label_color_blend[] = {
            gameover_lose_label_color[0],
            gameover_lose_label_color[1],
            gameover_lose_label_color[2],
            status.gameover_opacity
        };

        texture_t texture_id;

        if (game.remaining_seconds < 0.0) texture_id = textures.gameover_lose_timeout;
        else texture_id = textures.gameover_lose;

        draw2DTexturedRectangle((status.viewport_width-GAMEOVER_LABEL_WIDTH) / 2.0,
                      (status.viewport_height-GAMEOVER_LABEL_HEIGHT) / 2.0,
                      GAMEOVER_LABEL_WIDTH,
                      GAMEOVER_LABEL_HEIGHT,
                      gameover_win_label_color_blend,
                      texture_id,
                      0.0, 1.0, 0.0, 1.0);

        gameover = true;

    } else if (game.phase == PHASE_GAMEOVER_WIN) {

        float gameover_win_label_color_blend[] = {
            gameover_win_label_color[0],
            gameover_win_label_color[1],
            gameover_win_label_color[2],
            status.gameover_opacity
        };

        draw2DTexturedRectangle((status.viewport_width-GAMEOVER_LABEL_WIDTH) / 2.0,
                      (status.viewport_height-GAMEOVER_LABEL_HEIGHT) / 2.0,
                      GAMEOVER_LABEL_WIDTH,
                      GAMEOVER_LABEL_HEIGHT,
                      gameover_win_label_color_blend,
                      textures.gameover_win,
                      0.0, 1.0, 0.0, 1.0);

        gameover = true;

    }

    if (gameover && status.show_play_again) {
        draw2DTexturedRectangle((status.viewport_width-PLAY_AGAIN_LABEL_WIDTH) / 2.0,
              status.viewport_height-PLAY_AGAIN_LABEL_HEIGHT,
              PLAY_AGAIN_LABEL_WIDTH,
              PLAY_AGAIN_LABEL_HEIGHT,
              play_again_label_color,
              textures.play_again,
              0.0, 1.0, 0.0, 1.0);
    }


    /*******************************/
    /******* COPYRIGHT NOTICE ******/
    /*******************************/

    if (status.copy_notice_opacity > 0.0) {
        float copyright_notice_color_blend[] = {
            copyright_notice_color[0],
            copyright_notice_color[1],
            copyright_notice_color[2],
            copyright_notice_color[3] * status.copy_notice_opacity
        };
        draw2DTexturedRectangle((status.viewport_width-COPYRIGHT_NOTICE_WIDTH) / 2.0,
                              0.0,
                              COPYRIGHT_NOTICE_WIDTH,
                              COPYRIGHT_NOTICE_HEIGHT,
                              copyright_notice_color_blend,
                              textures.copy_notice,
                              0.0, 1.0, 0.0, 1.0);
    }

    glDisable(GL_BLEND);

    glPopMatrix(); // pop modelview

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // pop projection

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

}

/**************************************/

/**************************************/
/******   INTERACTION FUNCTIONS  ******/
/**************************************/

void fullscreen(bool active) {
    if (active) {
        glutSetCursor(GLUT_CURSOR_NONE);
        glutFullScreen();
    } else {
        glutSetCursor(GLUT_CURSOR_INHERIT);
        glutPositionWindow(WINDOW_POS_X, WINDOW_POS_Y);
        glutReshapeWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    }
}

/**************************************/

void keyboardDown(unsigned char key, int x, int y) {
    switch (key) {
        case KEY_ACCEL:
            controls.accel = true;
            controls.decel = false;
            break;
        case KEY_DECEL:
            controls.accel = false;
            controls.decel = true;
            controls.afterburner = false;
            break;
        case KEY_AFTERBURNER:
            controls.afterburner = true;
            controls.decel = false;
            break;
        case KEY_ROT_LEFT:
            controls.rot_left = true;
            controls.rot_right = false;
            break;
        case KEY_ROT_RIGHT:
            controls.rot_left = false;
            controls.rot_right = true;
            break;
        case KEY_INVISIBILITY:
            controls.invisibility = !controls.invisibility;
            break;
        case KEY_CAMERA_NEAR:
            status.camera_distance -= CAMERA_DIST_INCREMENT;
            if (status.camera_distance < SHIP_RADIUS_X)
                status.camera_distance = SHIP_RADIUS_X;
            break;
        case KEY_CAMERA_FAR:
            status.camera_distance += CAMERA_DIST_INCREMENT;
            break;
        case KEY_PAUSE:
            if (game.phase == PHASE_ACTION) {
                game.paused = !game.paused;
                pauseGame(game.paused);
            }
            break;
        case KEY_SELF_DESTRUCT:
            if (game.phase == PHASE_ACTION) {
                game.shield_health = -1.0;
                gameOver(false);
            }
            break;
        case KEY_FULLSCREEN2:
            if (controls.fullscreen) {
                controls.fullscreen = false;
                fullscreen(false);
            } else {
                controls.fullscreen = true;
                fullscreen(true);
            }
            break;
        case KEY_EXIT:
            exit(EXIT_SUCCESS);
            break;
    }
}

/**************************************/

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case KEY_ACCEL:
            controls.accel = false;
            break;
        case KEY_DECEL:
            controls.decel = false;
            break;
        case KEY_AFTERBURNER:
            controls.afterburner = false;
            break;
        case KEY_ROT_LEFT:
            controls.rot_left = false;
            break;
        case KEY_ROT_RIGHT:
            controls.rot_right = false;
            break;
    }
}

/**************************************/

void keySpecialDown(int key, int x, int y) {
    switch(key) {
		case GLUT_KEY_UP:
            if (INVERT_Y) {
                controls.down = true;
                controls.up = false;
            } else {
                controls.up = true;
                controls.down = false;
            }
            break;
		case GLUT_KEY_DOWN:
            if (INVERT_Y) {
                controls.up = true;
                controls.down = false;
            } else {
                controls.down = true;
                controls.up = false;
            }
            break;
		case GLUT_KEY_LEFT:
            controls.left = true;
            controls.right = false;
            break;
        case GLUT_KEY_RIGHT:
            controls.right = true;
            controls.left = false;
            break;
        case KEY_PLAY_AGAIN:
            endGame();
            startGame();
            break;
        case KEY_FULLSCREEN:
            if (controls.fullscreen) {
                controls.fullscreen = false;
                fullscreen(false);
            } else {
                controls.fullscreen = true;
                fullscreen(true);
            }
            break;
	}
}

/**************************************/

void keySpecialUp(int key, int x, int y) {
    switch(key) {
		case GLUT_KEY_UP:
            if (INVERT_Y) controls.down = false;
            else controls.up = false;
            break;
		case GLUT_KEY_DOWN:
            if (INVERT_Y) controls.up = false;
            else controls.down = false;
            break;
		case GLUT_KEY_LEFT:
            controls.left = false;
            break;
        case GLUT_KEY_RIGHT:
            controls.right = false;
            break;
	}
}

/**************************************/

/**************************************/
/******      DEBUG FUNCTIONS     ******/
/**************************************/

void printDebugInfo() {
    printf("\npos:  %f %f %f\n"
           "look: %f %f %f\n"
           "vup:  %f %f %f\n"
           "throttle: %f; n_shots: %d\n"
           "remaining_seconds: %f; shield_health: %f\n"
           "afterburner_fuel: %f; invisibility_fuel: %f\n"
           "--------------------\n",
        status.ship_pos.x, status.ship_pos.y, status.ship_pos.z,
        status.vector_look.x, status.vector_look.y, status.vector_look.z,
        status.vector_up.x, status.vector_up.y, status.vector_up.z,
        status.throttle, game.n_shots, game.remaining_seconds, game.shield_health,
        game.afterburner_fuel, game.invisibility_fuel);
}

/**************************************/

/**************************************/
/******           MAIN           ******/
/**************************************/

int main(int argc, char* argv[]) {

    printf("\n\n");
    printf("*******************************\n");
    printf(PROGRAM_NAME"\n");
    printf("*******************************\n");
    printf(COPYRIGHT_NOTICE"\n\n");

    // Disabled as of version 0.91
    // checkDistributionIntegrity();

	glutInit(&argc, argv);

    init(WORLD_FILE);

    #if !GAME_MODE
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(WINDOW_POS_X, WINDOW_POS_Y);
    glutInitDisplayMode(DISPLAY_FLAGS);
    glutCreateWindow(WINDOW_TITLE);
    glutSetCursor(GLUT_CURSOR_WAIT);
    initGraphics();
    glutSetCursor(GLUT_CURSOR_INHERIT);
    if (START_FULLSCREEN) {
        controls.fullscreen = true;
        fullscreen(true);
    }
    #else
    glutInitDisplayMode(DISPLAY_FLAGS);
    glutGameModeString(GAME_MODE_STRING);
	if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
		glutEnterGameMode();
	} else {
		fprintf(stderr,"The selected mode is not available.\n");
		exit(EXIT_FAILURE);
	}
	glutSetCursor(GLUT_CURSOR_NONE);
	initGraphics();
    #endif

    startGame();

	// registra le callback
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keySpecialDown);
	glutSpecialUpFunc(keySpecialUp);
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutIdleFunc(gameProgress);
	glutIgnoreKeyRepeat(true);

    atexit(cleanup);
	glutMainLoop();

	return EXIT_SUCCESS;
}
