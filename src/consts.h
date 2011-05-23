#ifndef CONSTS_H
#define CONSTS_H

// Program constants
#define PROGRAM_NAME "MINE CRISIS v0.92 beta"
#define COPYRIGHT_NOTICE "(c)2010 Giacomo Drago. All rights reserved."
#define WINDOW_TITLE "Mine Crisis"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_POS_X 100
#define WINDOW_POS_Y 100
#define GAME_MODE 0 // if enabled excludes fullscreen
#define GAME_MODE_STRING "800x600:16@60"
#define START_FULLSCREEN 1
#define INVERT_Y 1
#define PRINT_STATUS 0
#define PRINT_COLLISION_REPORT 0
#define PRINT_DEBUG_INTERVAL 0.2
#define FRUSTUM_CULLING 1
#define DRAW_DEBUG_AIDS 0
#define ANTIALIASING 1
#define ENABLE_BACKGROUND_MUSIC 1


// Resources

#define WORLD_FILE "./worlds/world.txt"

#define COPYRIGHT_FILE "COPYRIGHT.TXT"
    #define COPYRIGHT_FILE_CHECKSUM 3544333434

#define LICENSE_FILE "LICENSE.TXT"
    #define LICENSE_FILE_CHECKSUM 1403050127

#define README_FILE "README.TXT"
    #define README_FILE_CHECKSUM 3285314056

#define PLANET_TEXTURE_FILE "./data/planet.png"
    #define PLANET_TEXTURE_FILE_CHECKSUM 1907437650

#define STARMAP_TEXTURE_FILE "./data/starmap.png"
    #define STARMAP_TEXTURE_FILE_CHECKSUM 1880947955

#define SHIELD_TEXTURE_FILE "./data/shield.png"
    #define SHIELD_TEXTURE_FILE_CHECKSUM 1867088182

#define AFTERBURNER_TEXTURE_FILE "./data/afterburner.png"
    #define AFTERBURNER_TEXTURE_FILE_CHECKSUM 1924308016

#define INVISIBILITY_TEXTURE_FILE "./data/invisibility.png"
    #define INVISIBILITY_TEXTURE_FILE_CHECKSUM 1783708284

#define NUMBERS_TEXTURE_FILE "./data/numbers.png"
    #define NUMBERS_TEXTURE_FILE_CHECKSUM 3382335371

#define REMAINING_TIME_TEXTURE_FILE "./data/remaining_time.png"
    #define REMAINING_TIME_TEXTURE_FILE_CHECKSUM 2957264435

#define REMAINING_CHECKPOINTS_TEXTURE_FILE "./data/remaining_checkpoints.png"
    #define REMAINING_CHECKPOINTS_TEXTURE_FILE_CHECKSUM 767065940

#define GAMEOVER_LOSE_TEXTURE_FILE "./data/gameover_lose.png"
    #define GAMEOVER_LOSE_TEXTURE_FILE_CHECKSUM 1813625396

#define GAMEOVER_LOSE_TIMEOUT_TEXTURE_FILE "./data/gameover_lose_timeout.png"
    #define GAMEOVER_LOSE_TIMEOUT_TEXTURE_FILE_CHECKSUM 1440625490

#define GAMEOVER_WIN_TEXTURE_FILE "./data/gameover_win.png"
    #define GAMEOVER_WIN_TEXTURE_FILE_CHECKSUM 3403992007

#define PAUSE_TEXTURE_FILE "./data/pause.png"
    #define PAUSE_TEXTURE_FILE_CHECKSUM 2088678305

#define PLAY_AGAIN_TEXTURE_FILE "./data/play_again.png"
    #define PLAY_AGAIN_TEXTURE_FILE_CHECKSUM 746037917

#define SHOT_SOUND_FILE "./data/shot.wav"
    #define SHOT_SOUND_FILE_CHECKSUM 3745068138

#define GAMEOVER_WIN_SOUND_FILE "./data/gameover_win.wav"
    #define GAMEOVER_WIN_SOUND_FILE_CHECKSUM 2288608036

#define GAMEOVER_LOSE_SOUND_FILE "./data/gameover_lose.wav"
    #define GAMEOVER_LOSE_SOUND_FILE_CHECKSUM 1067008452

#define SHIELD_SOUND_FILE "./data/shield.wav"
    #define SHIELD_SOUND_FILE_CHECKSUM 4289915498

#define ALARM_SOUND_FILE "./data/alarm.wav"
    #define ALARM_SOUND_FILE_CHECKSUM 3859811983

#define MINE_SOUND_FILE "./data/mine.wav"
    #define MINE_SOUND_FILE_CHECKSUM 1056408803

#define CHECKPOINT_SOUND_FILE "./data/checkpoint.wav"
    #define CHECKPOINT_SOUND_FILE_CHECKSUM 1790473537

#define BEEP_SOUND_FILE "./data/beep.wav"
    #define BEEP_SOUND_FILE_CHECKSUM 274561054

#define MUSIC_SOUND_FILE "./data/music.ogg"
    #define MUSIC_SOUND_FILE_CHECKSUM 2373720264
    #define MUSIC_SOUND_FILE_CHECK 1

#define NUMBERS_COORDS_FILE "./data/numbers.txt"
    #define NUMBERS_COORDS_FILE_CHECKSUM 1257072565

#define SHIP_MODEL_FILE "./data/ship.ase"
    #define SHIP_MODEL_FILE_CHECKSUM 3069233350

#define SHIP_BOUNDS_FILE "./data/ship.bounds"
    #define SHIP_BOUNDS_FILE_CHECKSUM 3007553988

// Throttle constants
#define THROTTLE_DEFAULT 50.0
#define THROTTLE_MIN 30.0
#define THROTTLE_MAX 70.0
#define THROTTLE_INCREMENT 25.0
#define THROTTLE_AFTERBURNER 100.0
#define THROTTLE_AFTERBURNER_FACTOR 4.0
#define THROTTLE_X_MAX PI_HALF
#define THROTTLE_X_INCREMENT 1.8
#define THROTTLE_Y_MAX PI_HALF
#define THROTTLE_Y_INCREMENT 1.8
#define THROTTLE_X_INCLINATION_FACTOR 3.0
#define THROTTLE_ROT_MAX PI
#define THROTTLE_ROT_INCREMENT PI

// Camera constants
#define CAMERA_DEFAULT_DIST 6.0
#define CAMERA_DIST_INCREMENT 1.0
#define CAMERA_BASE_ANGLE 0.05
#define CAMERA_THROTTLE_FACTOR 0.05
#define CAMERA_THROTTLE_Y_FACTOR 0.03

// Controls constants
#define KEY_ACCEL 'w'
#define KEY_DECEL 's'
#define KEY_ROT_LEFT 'a'
#define KEY_ROT_RIGHT 'd'
#define KEY_AFTERBURNER ' '
#define KEY_INVISIBILITY 'x'
#define KEY_CAMERA_NEAR '+'
#define KEY_CAMERA_FAR '-'
#define KEY_PAUSE 'p'
#define KEY_EXIT 27
#define KEY_CAMERA GLUT_KEY_F1
#define KEY_PLAY_AGAIN GLUT_KEY_F2
#define KEY_FULLSCREEN GLUT_KEY_F11
#define KEY_FULLSCREEN2 'f'
#define KEY_SELF_DESTRUCT '|'

// Game constants
#define START_DISTANCE 200.0
#define MINE_EXPLOSION_RANGE 70.0
#define TURRET_ENGAGE_DIST 350.0
#define MINE_EXPLOSION_STRENGTH 100.0
#define MINE_EXPLOSION_SHIELD_DECREMENT 40.0
#define MINE_EXPLODED_BUF_SIZE 16
#define EXPLOSION_VECTOR_DECREMENT 100.0
#define SHOT_SPEED 180.0
#define SHOT_INTERVAL 0.7
#define SHOT_EXPIRE_DISTANCE 3000.0
#define SHOT_STRENGHT 40.0
#define SHOT_SHIELD_DECREMENT 20.0
#define MAX_SHIELD 100.0
#define SHIELD_RESPAWN_DELAY 4.0
#define SHIELD_RESPAWN_SPEED 5.0
#define MAX_AFTERBURNER_FUEL 100.0
#define AFTERBURNER_FUEL_COMSUMPTION 33.33
#define AFTERBURNER_RESPAWN_DELAY 5.0
#define AFTERBURNER_RESPAWN_SPEED 5.0
#define MAX_INVISIBILITY_FUEL 100.0
#define INVISIBILITY_FUEL_COMSUMPTION 15.0
#define INVISIBILITY_RESPAWN_DELAY 8.0
#define INVISIBILITY_RESPAWN_SPEED 2.0
#define STARTING_TIME 30.0
#define TIME_BONUS 15.0
#define DISABLE_MINE_COLLISIONS 0
#define DISABLE_MOVE_VECTORS 0
#define DISABLE_TURRETS 0
#define DISABLE_DAMAGE 0
#define COUNTDOWN_BEGIN 10

// Game phases
#define PHASE_ACTION 1
#define PHASE_GAMEOVER_LOSE 2
#define PHASE_GAMEOVER_WIN 3

// Graphics constants
#define SHIP_MODEL_SCALING_FACTOR 0.3 // Hardcoded ship model scaling factor
#define PERSPECTIVE_FOVY 45.0
#define PERSPECTIVE_NEAR 0.1
#if ANTIALIASING
    #define DISPLAY_FLAGS GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_MULTISAMPLE
#else
    #define DISPLAY_FLAGS GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH
#endif
#define MINE_DRAW_THRESHOLD 550.0
#define MINE_DETAIL_LOW_THRESHOLD 500.0
#define MINE_DETAIL_MED_THRESHOLD 200.0
#define MINE_DETAIL_HIGH_THRESHOLD 100.0
#define CHECKPOINT_DETAIL_LOW_THRESHOLD 1000.0
#define CHECKPOINT_DETAIL_MED_THRESHOLD 100.0
#define CHECKPOINT_MAX_ALPHA 0.6
#define CHECKPOINT_MIN_ALPHA 0.2
#define CHECKPOINT_FLASH_SPEED 0.3
#define TURRET_DETAIL_LOW_THRESHOLD 800.0
#define TURRET_DETAIL_MED_THRESHOLD 300.0
#define TURRET_DETAIL_HIGH_THRESHOLD 100.0
#define SPOTLIGHT_ATTENUATION 0.00012
#define SPOTLIGHT_CUTOFF 15.0
#define SPOTLIGHT_EXPONENT 100
#define SHIELD_ELLIPSOID_POINTS_SQRT 16
#define SHIELD_ELLIPSOID_POINTS 256
#define SHIELD_MAGNITUDE_SCALE_MAX 40.0
#define INTERPOLATION_GRANULARITY 100
#define PLANET_ROT_DEGREES 2.0
#define EXPLOSION_POINTS 5000
#define EXPLOSION_TRIANGLES 3000
#define INVISIBILITY_SHIP_OPACITY 0.2
#define INVISIBILITY_TRANSITION_SPEED 3.0

// Light variables and constants
// Light 0: light coming from the planet
#define PLANET_LIGHT GL_LIGHT0
const float planet_light_position[] = {0.0, 0.0, 1.0, 0.0};
const float planet_light_ambient[] = {0.4, 0.4, 0.4, 1.0};
const float planet_light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
const float planet_light_specular[] = {1.0, 1.0, 1.0, 0.0};

// Light 1: ship spotlight
#define SPACESHIP_SPOTLIGHT GL_LIGHT1
const float spaceship_spotlight_ambient[] = {0.0, 0.0, 0.0, 0.0};
const float spaceship_spotlight_diffuse[] = {0.2, 0.0, 0.2, 0.0};
const float spaceship_spotlight_specular[] = {1.0, 0.0, 1.0, 1.0};

// Light 2: fake light back the ship
#define FAKE_SHIP_LIGHT GL_LIGHT2
const float ship_light_position[] = {0.0, 1.0, 2.0, 0.0};
const float ship_light_ambient[] = {0.4, 0.4, 0.4, 1.0};
const float ship_light_diffuse[] = {0.7, 0.7, 0.7, 1.0};
const float ship_light_specular[] = {1.0, 1.0, 1.0, 0.0};


// Cockpit constants

#define SHIELD_METER_PADDING_LEFT 15
#define SHIELD_METER_PADDING_BOTTOM 15
#define SHIELD_METER_WIDTH 20
#define SHIELD_METER_HEIGHT 200
#define SHIELD_ICON_WIDTH 30
#define SHIELD_ICON_HEIGHT 30
#define SHIELD_ICON_PADDING_LEFT 10
#define SHIELD_ICON_PADDING_BOTTOM 7

#define AFTERBURNER_METER_PADDING_RIGHT 15
#define AFTERBURNER_METER_WIDTH 20
#define AFTERBURNER_METER_HEIGHT 200
#define AFTERBURNER_METER_PADDING_BOTTOM 15
#define AFTERBURNER_ICON_PADDING_RIGHT 10
#define AFTERBURNER_ICON_WIDTH 30
#define AFTERBURNER_ICON_HEIGHT 30
#define AFTERBURNER_ICON_PADDING_BOTTOM 7

#define INVISIBILITY_METER_PADDING_RIGHT 20
#define INVISIBILITY_METER_WIDTH 20
#define INVISIBILITY_METER_HEIGHT 200
#define INVISIBILITY_METER_PADDING_BOTTOM 15
#define INVISIBILITY_ICON_PADDING_RIGHT 11
#define INVISIBILITY_ICON_WIDTH 30
#define INVISIBILITY_ICON_HEIGHT 30
#define INVISIBILITY_ICON_PADDING_BOTTOM 7

#define COUNTDOWN_TEXTURE_WIDTH 2048
#define COUNTDOWN_TEXTURE_HEIGHT 256

#define REMAINING_TIME_HEIGHT_BIG 80
#define REMAINING_TIME_HEIGHT_SMALL 40
#define REMAINING_TIME_PADDING_TOP 5
#define REMAINING_TIME_PADDING_RIGHT 15
#define REMAINING_TIME_SPACING_BIG 45
#define REMAINING_TIME_SPACING_SMALL 20

#define REMAINING_TIME_LABEL_PADDING_RIGHT 15
#define REMAINING_TIME_LABEL_PADDING_TOP 5
#define REMAINING_TIME_LABEL_WIDTH 138
#define REMAINING_TIME_LABEL_HEIGHT 20
#define REMAINING_TIME_LABEL_TEXTURE_COORD 0.863

#define PAUSE_LABEL_WIDTH 512
#define PAUSE_LABEL_HEIGHT 256
#define GAMEOVER_LABEL_WIDTH 512
#define GAMEOVER_LABEL_HEIGHT 256

#define GAMEOVER_LABEL_DELAY 2.0
#define GAMEOVER_LABEL_APPEAR_SPEED 0.5

#define REMAINING_CHECKPOINTS_HEIGHT 60
#define REMAINING_CHECKPOINTS_PADDING_LEFT 15
#define REMAINING_CHECKPOINTS_PADDING_TOP 5

#define REMAINING_CHECKPOINTS_LABEL_PADDING_LEFT 15
#define REMAINING_CHECKPOINTS_LABEL_PADDING_TOP 5
#define REMAINING_CHECKPOINTS_LABEL_WIDTH 204
#define REMAINING_CHECKPOINTS_LABEL_HEIGHT 20
#define REMAINING_CHECKPOINTS_LABEL_TEXTURE_COORD 0.64

#define PLAY_AGAIN_LABEL_WIDTH 256
#define PLAY_AGAIN_LABEL_HEIGHT 64
#define PLAY_AGAIN_LABEL_DELAY 3.5

#define COPYRIGHT_NOTICE_WIDTH 512
#define COPYRIGHT_NOTICE_HEIGHT 64
#define COPYRIGHT_NOTICE_FADE_DELAY 4.0
#define COPYRIGHT_NOTICE_FADE_SPEED 0.2


static const float shield_meter_border_color1[] = {1.0, 1.0, 1.0, 0.8};
static const float shield_meter_border_color2[] = {1.0, 0.0, 0.0, 1.0};
static const float shield_meter_color[] = {0.0, 1.0, 1.0, 0.8};
static const float afterburner_meter_border_color[] = {1.0, 1.0, 1.0, 0.8};
static const float afterburner_meter_color[] = {1.0, 1.0, 0.0, 0.8};
static const float invisibility_meter_border_color[] = {1.0, 1.0, 1.0, 0.8};
static const float invisibility_meter_color[] = {0.8, 0.8, 0.8, 0.8};
static const float remaining_time_color[] = {1.0, 1.0, 0.0, 0.8};
static const float remaining_time_label_color[] = {1.0, 1.0, 1.0, 0.8};
static const float remaining_checkpoints_color[] = {0.0, 1.0, 1.0, 0.8};
static const float remaining_checkpoints_label_color[] = {1.0, 1.0, 1.0, 0.8};
static const float pause_label_color[] = {1.0, 1.0, 1.0, 0.8};
static const float gameover_lose_label_color[] = {1.0, 0.0, 0.0, 0.8};
static const float gameover_win_label_color[] = {0.0, 1.0, 0.0, 1.0};
static const float play_again_label_color[] = {1.0, 1.0, 1.0, 1.0};
static const float copyright_notice_color[] = {1.0, 1.0, 1.0, 1.0};
// Sound constants

#define MUSIC_SOUND_GAIN 0.5
#define ALARM_SOUND_GAIN 0.25
#define MINE_SOUND_GAIN 0.2
#define BEEP_SOUND_GAIN 0.3

#endif
