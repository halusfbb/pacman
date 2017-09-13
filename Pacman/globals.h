#ifndef GLOBALS_H
#define GLOBALS_H

#include "Ghost.h"

extern const char* GRAY_GHOST_IMAGE;
extern const char* RED_GHOST_IMAGE;
extern const char* PINK_GHOST_IMAGE;
extern const char* CYAN_GHOST_IMAGE;
extern const char* ORANGE_GHOST_IMAGE;
extern const char* DEAD_GHOST_IMAGE;
extern const char* FRIGHTENED_GHOST_IMAGE;

//in seconds
extern int CHASE_STATE_MIN_DURATION;
extern int CHASE_STATE_MAX_DURATION;
extern int SCATTER_STATE_MIN_DURATION;
extern int SCATTER_STATE_MAX_DURATION;
extern int CHASE_SCATTER_SWAPS; //no of chase and scatter state swaps before it gets permanently locked
extern GhostState INITIAL_CYCLIC_GHOST_STATE;

extern int PACMAN_START_LIVES;
extern float PACMAN_BASE_SPEED;

extern float GHOST_BASE_SPEED;
extern float GHOST_FRIGHTENED_SPEED_FACTOR;
extern float GHOST_DEAD_SPEED_FACTOR;

extern int FRIGHTEN_DURATION;

extern int MIN_TIMER_BEFORE_GHOST_EXIT;
extern int MAX_TIMER_BEFORE_GHOSTS_EXIT;

extern int TILE_SIZE;

extern int ENTITIY_OFFSET_SIZE_X;
extern int ENTITIY_OFFSET_SIZE_Y;
#endif

