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

typedef struct {
	AnimSprite *img;
	int direction;	// 0 - down, 1 - right, 2 - up, 3 - left
	int mode;	// 0 - normal, 1 - powered (has eaten Energizer)
} Pacman;

Pacman * pacman_init(int xi, int yi, int speed);
void pacman_rotate(Pacman * pacman, int direction);

char ** pacman_maps(int direction);
char ** pacman_right_maps();
char ** pacman_left_maps();
char ** pacman_up_maps();
char ** pacman_down_maps();

char *pixel(char* map, int width, int heigth, int x, int y);


void pacnix_start();
void interrupts();


#endif /*__PACNIX_H */
