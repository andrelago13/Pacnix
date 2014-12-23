#ifndef __PACNIX_H
#define __PACNIX_H

#include "asprite.h"
#include "mouse.h"

#define DOWN 0
#define RIGHT 1
#define UP 2
#define LEFT 3

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
	int lives;
	int spawn_timer;
} Pacman;

int fps_tick();

Pacman * pacman_init(int xi, int yi, int speed, int lives);
void pacman_rotate_scan(Pacman * pacman, unsigned long scan_code);
void pacman_rotate(Pacman * pacman, int direction);
void pacman_read_key(Pacman * pacman, unsigned long scan_code);
void pacman_try_rotate(Pacman * pacman);
void pacman_move_tick(Pacman * pacman);
int pacman_check_surroundings(Pacman * pacman);
void pacman_spawn_timer(Pacman * pacman);

char ** pacman_maps(int direction);
char ** pacman_right_maps();
char ** pacman_left_maps();
char ** pacman_up_maps();
char ** pacman_down_maps();




typedef struct {
	Sprite *img;
	int direction;	// 0 - down, 1 - right, 2 - up, 3 - left
	int mode;	// 0 - random, 1 - chase, 2 - user controlled, 3 - evade pacman, 4 - timed, 5 - user controlled evading
	int desired_direction; // for user_controlled movement
	int prev_mode;
	int color;	// 0 - orange, 1 - red, 2 - blue, 3 - pink
	int speed;
	int detouring;
	int chase_time, random_time, curr_counter, temp_mode;
	int escape_counter, spawn_timer;
} Ghost;

Ghost * ghost_init(int xi, int yi, int speed, int color, int mode);
void ghost_rotate(Ghost * ghost, int direction);
char* ghost_dir_map(int color, int direction);
void ghost_read_key(Ghost * ghost, unsigned long scan_code);
int ghost_check_surroundings(Ghost * ghost);
void move_ghost(Ghost * ghost, Pacman * pacman);
void move_ghost_random(Ghost * ghost);
void move_ghost_chase(Ghost * ghost, Pacman * pacman);
void move_ghost_user(Ghost * ghost);
void move_ghost_escape(Ghost * ghost, Pacman * pacman);
void move_timed_ghost(Ghost * ghost, Pacman * pacman);
void move_ghost_user_esc(Ghost * ghost);
int get_pacman_dir(Ghost * ghost, Pacman * pacman);
int is_in_ghost(Ghost * ghost, int x_click, int y_click);
void switch_ghosts_to_auto(Ghost *ghosts[], int exception);
void switch_ghosts_to_mode(Ghost *ghosts[], int mode);
void check_user_ghosts(Ghost *ghosts[], unsigned long scan_code);
void ghost_change_desired_direction(Ghost *ghost, unsigned long scan_code);
void ghost_try_rotate(Ghost * ghost);
void all_ghosts_escape(Ghost * ghosts[], int time);
void ghost_escape_tick(Ghost * ghost);
void all_ghosts_escape_tick(Ghost * ghosts[]);
void all_ghosts_spawn_timer(Ghost * ghosts[]);
void reset_all_ghosts(Ghost * ghosts[]);
void reset_ghost(Ghost * ghost);
void ghost_eaten(Ghost * ghost);





char *pixel(char* map, int width, int heigth, int x, int y);

// changes array of sides, 0 - down, 1 - right, ... (1 if blocked)
void check_all_surroundings(int xi, int yi, int width, int height, int sides[]);

int game_local(int game_mode);	// GAME-MODE : 0-SP, 1-MP			// return : 0-terminated, 1-pacman lost sp, 2-pacman lost mp, 3-pacman won

void start_menu();
void pacnix_start();
void empty_buf();
void draw_lives(int lives, int xi, int yi);
int rand_integer_between(int a, int b);
int probability(int percentage);
int prev_revclock_dir(int dir);
int next_revclock_dir(int dir);
int are_opposite_directions(int dir1, int dir2);
void check_for_click(Ghost *ghosts[], Mouse_coord *mouse);
int check_collisions(Ghost *ghosts[], Pacman * pacman);
double get_dist(Sprite *sp1, Sprite *sp2);		// return -1 if no collision, ghost number otherwise


#endif /*__PACNIX_H */
