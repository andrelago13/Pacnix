#ifndef __PACNIX_H
#define __PACNIX_H

#include "asprite.h"

#define DOWN 0
#define RIGHT 1
#define UP 2
#define LEFT 3

#define DOWN_ARROW 0x50
#define RIGHT_ARROW 0x4D
#define UP_ARROW 0x48
#define LEFT_ARROW 0x4B

#define COLOR_GHOST_ORANGE 0
#define COLOR_GHOST_RED 1
#define COLOR_GHOST_BLUE 2
#define COLOR_GHOST_PINK 3

// Directions to find pacman for ghosts in chase mode
#define PACDIR_S 0
#define PACDIR_SSE 1
#define PACDIR_ESE 2
#define PACDIR_E 3
#define PACDIR_ENE 4
#define PACDIR_NNE 5
#define PACDIR_N 6
#define PACDIR_NNW 7
#define PACDIR_WNW 8
#define PACDIR_W 9
#define PACDIR_WSW 10
#define PACDIR_SSW 11


#define GAME_FPS 50

typedef struct {
	AnimSprite *img;
	int direction;	// 0 - down, 1 - right, 2 - up, 3 - left
	int desired_direction; // same as previous. Indicates the direction the
						   // user wants pacman to go. It only goes that direction
						   // if there are no obstacles
	int mode;	// 0 - normal, 1 - powered (has eaten Energizer)
} Pacman;

int fps_tick();

Pacman * pacman_init(int xi, int yi, int speed);
void pacman_rotate_scan(Pacman * pacman, unsigned long scan_code);
void pacman_rotate(Pacman * pacman, int direction);
void pacman_read_key(Pacman * pacman, unsigned long scan_code);
void pacman_try_rotate(Pacman * pacman);
void pacman_move_tick(Pacman * pacman);
int pacman_check_surroundings(Pacman * pacman);

char ** pacman_maps(int direction);
char ** pacman_right_maps();
char ** pacman_left_maps();
char ** pacman_up_maps();
char ** pacman_down_maps();




typedef struct {
	Sprite *img;
	int direction;	// 0 - down, 1 - right, 2 - up, 3 - left
	int mode;	// 0 - random, 1 - chase, 2 - user controlled, 3 - evade pacman
	int color;	// 0 - orange, 1 - red, 2 - blue, 3 - pink
	int speed;
} Ghost;

Ghost * ghost_init(int xi, int yi, int speed, int color, int mode);
void ghost_rotate(Ghost * ghost, int direction);
char* ghost_dir_map(int color, int direction);
void ghost_read_key(Ghost * ghost, unsigned long scan_code);
int ghost_check_surroundings(Ghost * ghost);
void move_ghost(Ghost * ghost, Pacman * pacman);
void move_ghost_random(Ghost * ghost);
void move_ghost_chase(Ghost * ghost, Pacman * pacman);
void move_ghost_escape(Ghost * ghost, Pacman * pacman);
int get_pacman_dir(Ghost * ghost, Pacman * pacman);






char *pixel(char* map, int width, int heigth, int x, int y);

// changes array of sides, 0 - down, 1 - right, ... (1 if blocked)
void check_all_surroundings(int xi, int yi, int width, int height, int sides[]);

void pacnix_start();
void interrupts();
int rand_integer_between(int a, int b);
int probability(int percentage);
int prev_revclock_dir(int dir);
int next_revclock_dir(int dir);
int are_opposite_directions(int dir1, int dir2);


#endif /*__PACNIX_H */
