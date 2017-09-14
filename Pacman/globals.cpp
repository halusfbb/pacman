#include "globals.h"

const char* GRAY_GHOST_IMAGE = "ghost_32.png";
const char* RED_GHOST_IMAGE = "ghost_32_red.png";
const char* PINK_GHOST_IMAGE = "ghost_32_pink.png";
const char* CYAN_GHOST_IMAGE = "ghost_32_cyan.png";
const char* ORANGE_GHOST_IMAGE = "ghost_32_orange.png";
const char* DEAD_GHOST_IMAGE = "Ghost_Dead_32.png";
const char* FRIGHTENED_GHOST_IMAGE = "Ghost_Vulnerable_32.png";

int CHASE_STATE_MIN_DURATION = 17;
int CHASE_STATE_MAX_DURATION = 22;
int SCATTER_STATE_MIN_DURATION = 3;
int SCATTER_STATE_MAX_DURATION = 5;
int CHASE_SCATTER_SWAPS = 8;
GhostState INITIAL_CYCLIC_GHOST_STATE = GHOST_CHASE;

int PACMAN_START_LIVES = 3;
float PACMAN_BASE_SPEED = 95.f;

float GHOST_BASE_SPEED = 95.f;
float GHOST_FRIGHTENED_SPEED_FACTOR = .5f;
float GHOST_DEAD_SPEED_FACTOR = 1.5f;

float GHOST_BLINK_ON_DURATION = .5f;
float GHOST_BLINK_OFF_DURATION = .1f;
float GHOST_BLINK_BEFORE_FRIGHTEN_END = 3.f;

int RED_GHOST_DOT_TOLERANCE = 70;
float RED_GHOSt_SPEED_UPGRADE = 1.05f;
int ORANGE_GHOST_TO_AVATAR_PROXIMITY_TOLERANCE = 8;

int MIN_TIMER_BEFORE_GHOST_EXIT = 4;
int MAX_TIMER_BEFORE_GHOSTS_EXIT = 6;

int FRIGHTEN_DURATION = 10;

int TILE_SIZE = 22;

int ENTITIY_OFFSET_SIZE_X = 220;
int ENTITIY_OFFSET_SIZE_Y = 60;