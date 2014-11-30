#ifndef __PACNIX_H
#define __PACNIX_H

#include "asprite.h"

typedef struct {
	AnimSprite img;
	int direction;	// 0 - down, 1 - right, 2 - up, 3 - left
	int mode;	// 0 - normal, 1 - powered (has eaten Energizer)
} Pacman;

void pacman_init(Pacman * pac);


void pacnix_start();
void interrupts();

#endif /*__PACNIX_H */
