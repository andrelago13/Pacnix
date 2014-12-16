#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "pacnix.h"
#include "mouse.h"
#include "mouse_header.h"
#include "timer.h"
#include "video_gr.h"
#include "i8254.h"
#include "kbd_header.h"
#include "pixmaps.h"
#include "video.h"
#include "kbd_header.h"
#include "kbd_funct.h"
#include "maze.h"

// Initialize frame rate counters. Frame rate set to 50
double counter;

void rotate_img(char* map, int width, int height)
{
	char * new_map = malloc(sizeof(char)*width*height);

	int old, new; old = 0; new = 0;

	int x, y;
	for(x=0, y=0; y < height; y++)
	{
		char * new_pix = (char *)pixel(new_map, width, height, 0, (-y)+width);
		char * old_pix = pixel(map, width, height, x, y);

		*((char *)new_pix) = *((char *)old_pix);


		x++;
		if(x == width);
			x = 0;
	}

	map = new_map;
}

void pacnix_start()
{
	initialize_map_pieces();
	counter = 0;
	interrupts();
	printf("\n\n\tProgram ended\n\n");
}

void empty_buf()
{
	unsigned long test = 0;
	unsigned long letra = 0;
	sys_inb(0x64, &test);
	while(test & 1 != 0)
	{
		sys_inb(KBD_OUT_BUF, &letra);
		sys_inb(0x64, &test);
	}
}

void interrupts()
{
	int ipc_status;
	message msg;

	// Subscribe mouse interrupts
	int mouse_hook = MOUSE_IRQ;
	int ret = mouse_subscribe(&mouse_hook);
	if(ret < 0)
		return;
	unsigned long irq_set_mouse = BIT(ret);

	// Subscribe timer0 interrupts
	int timer_hook = TIMER0_IRQ;
	ret = timer_subscribe(&timer_hook);
	if(ret < 0)
		return;
	unsigned long irq_set_timer = BIT(ret);

	// Subscribe keyboard interrupts
	ret = kbd_subscribe_int();
	if(ret < 0)
		return;
	unsigned long irq_set_kbd = BIT(ret);

	int terminus = 1;

	// Initialize mouse cursor
	Mouse_coord mouse;
	mouse.x_coord = 450;
	mouse.y_coord = 400;
	mouse.img.x = mouse.x_coord;
	mouse.img.y = mouse.y_coord;
	mouse.img.map = (char *)read_xpm(cursor, &mouse.img.width, &mouse.img.height);

	// Initialize pacman
	Pacman *pacman;
	pacman = malloc(sizeof(Pacman));
	pacman = pacman_init(200, 350, 3);

	// Initialize orange ghost
	Ghost *orange_ghost;
	orange_ghost = malloc(sizeof(Ghost));
	orange_ghost = ghost_init(60, 60, 2, COLOR_GHOST_ORANGE, 0);

	// Initialize pink ghost
	Ghost *pink_ghost;
	pink_ghost = malloc(sizeof(Ghost));
	pink_ghost = ghost_init(60, 90, 2, COLOR_GHOST_PINK, 0);

	// Initialize red ghost
	Ghost *red_ghost;
	red_ghost = malloc(sizeof(Ghost));
	red_ghost = ghost_init(60, 120, 2, COLOR_GHOST_RED, 0);

	// Initialize blue ghost
	Ghost *blue_ghost;
	blue_ghost = malloc(sizeof(Ghost));
	blue_ghost = ghost_init(60, 150, 2, COLOR_GHOST_BLUE, 1);

	// Initialize game map 1
	Pacman_map *map1;
	map1 = (Pacman_map *)malloc(sizeof(Pacman_map));
	map1 = map1_initialize();

	// Initialize packet read
	Mouse_packet tmp_delta;

	// Set mouse stream mode
	set_stream();

	// Initialize VRAM
	vg_init(GRAF_1024x768);

	while(terminus != 0)
	{
		if ( driver_receive(ANY, &msg, &ipc_status) != 0 ) {
			printf("Driver_receive failed\n");
			continue;
		}
		if (is_ipc_notify(ipc_status))
		{
			switch (_ENDPOINT_P(msg.m_source))
			{
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse)		/////////////////////////////// MOUSE INTERRUPT /////////////////////////////
			{
					ret = mouse_read_packet(&tmp_delta);

					if(ret == 1)
						update_mouse(&mouse, &tmp_delta);
				}
				if (msg.NOTIFY_ARG & irq_set_timer)		//////////////////////////////// TIMER 0 INTERRUPT /////////////////////////////
				{
					if(fps_tick() == 1)
					{
						fill_screen(COLOR_BLACK);

						draw_map(30, 30, map1);

						pacman_move_tick(pacman);

						draw_img(orange_ghost->img);
						move_ghost(orange_ghost, pacman);
						draw_img(blue_ghost->img);
						move_ghost(blue_ghost, pacman);
						draw_img(pink_ghost->img);
						move_ghost(pink_ghost, pacman);
						draw_img(red_ghost->img);
						move_ghost(red_ghost, pacman);

						draw_img(pacman->img->sp);
						animate_asprite(pacman->img);

						draw_mouse(&mouse);
						update_buffer();
					}

				}
				if (msg.NOTIFY_ARG & irq_set_kbd)			///////////////////////////// KEYBOARD INTERRUPT /////////////////////////
				{
					unsigned long letra = 0;

					if(OK != sys_inb(KBD_OUT_BUF, &letra))	// Read scancode
						return;

					if(letra == ESC_break)
					{
						terminus = 0;
						dis_stream();
					}

					// TEST CODE FOR RANDOM, CHASE AND ESCAPE MODES //
					if(letra == 0x13) //'R'
						blue_ghost->mode = 0; //random
					if(letra == 0x2E) //'C'
						blue_ghost->mode = 1; //chase
					if(letra == 0x12) //'E'
						blue_ghost->mode = 3; //escape
					/////////////////////////////////////////

					pacman_read_key(pacman, letra);

				}
				break;
			default:
				break;
			}
		}
	}

	// Unsubscribe all interrupts
	mouse_unsubscribe(&mouse_hook);
	timer_unsubscribe(&timer_hook);
	kbd_unsubscribe_int();
	empty_buf();

	vg_exit();
}

void pacman_read_key(Pacman * pacman, unsigned long scan_code)
{
	if(scan_code == RIGHT_ARROW)
	{
		pacman->desired_direction = RIGHT;
	}
	else if(scan_code == UP_ARROW)
	{
		pacman->desired_direction = UP;
	}
	else if (scan_code == LEFT_ARROW)
	{
		pacman->desired_direction = LEFT;
	}
	else if (scan_code == DOWN_ARROW)
	{
		pacman->desired_direction = DOWN;
	}
}

Pacman * pacman_init(int xi, int yi, int speed)
{
	Pacman * pacman;
	pacman = (Pacman *)malloc(sizeof(Pacman));

	pacman->mode = 0;
	pacman->direction = 1;
	pacman->desired_direction = 1;

	pacman->img = malloc(sizeof(AnimSprite));

	char** maps;
	maps = malloc(6*sizeof(char *));
	int temp1, temp2;

	maps[0] = (char *)read_xpm( pacman_r0_xpm, &temp1, &temp2);
	maps[1] = (char *)read_xpm( pacman_r1_xpm, &temp1, &temp2);
	maps[2] = (char *)read_xpm( pacman_r2_xpm, &temp1, &temp2);
	maps[3] = (char *)read_xpm( pacman_r3_xpm, &temp1, &temp2);
	maps[4] = (char *)read_xpm( pacman_r2_xpm, &temp1, &temp2);
	maps[5] = (char *)read_xpm( pacman_r1_xpm, &temp1, &temp2);

	pacman->img = create_asprite(maps, 6, speed, xi, yi, pacman_r3_xpm);

	return pacman;
}

int fps_tick()
{
	static double period = (double)TIMER0_FREQ/GAME_FPS;

	counter++;
	if((double)counter >= (double)period)
	{
		counter -= period;
		return 1;
	}
	return 0;
}

void pacman_move_tick(Pacman * pacman)
{
	pacman_try_rotate(pacman);

	if (1 == pacman_check_surroundings(pacman))
		return;

	switch(pacman->direction)
	{
	case 0:
		pacman->img->sp->y += 2;
		if(pacman->img->sp->y >= 768-28)
			pacman->img->sp->y = 768-28;
		break;
	case 1:
		pacman->img->sp->x += 2;
		if(pacman->img->sp->x >= 1024-28)
			pacman->img->sp->x = 1024-28;
		break;
	case 2:
		pacman->img->sp->y -= 2;
		if(pacman->img->sp->y <= 0)
			pacman->img->sp->y = 0;
		break;
	case 3:
		pacman->img->sp->x -= 2;
		if(pacman->img->sp->x <= 0)
			pacman->img->sp->x = 0;
		break;
	}
}

int pacman_check_surroundings(Pacman * pacman)
{
	int x;
	int y;
	int it;

	switch(pacman->direction)
	{
	case (int) DOWN:
		x = pacman->img->sp->x;
		y = pacman->img->sp->y; y += pacman->img->sp->height;
		it = pacman->img->sp->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x, y+1) == 1))
				return 1;
			x++;
		}
		return 0;
	case (int) RIGHT:
		x = pacman->img->sp->x; x += pacman->img->sp->width;
		y = pacman->img->sp->y;
		it = pacman->img->sp->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x+1, y) == 1))
				return 1;
			y++;

		}
		return 0;
	case (int) UP:
		x = pacman->img->sp->x;
		y = pacman->img->sp->y; y--;
		it = pacman->img->sp->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x, y-1) == 1))
				return 1;
			x++;
		}
		return 0;
	case (int) LEFT:
		x = pacman->img->sp->x; x--;
		y = pacman->img->sp->y;
		it = pacman->img->sp->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x-1, y) == 1))
				return 1;
			y++;

		}
		return 0;
	}
}

void pacman_rotate_scan(Pacman * pacman, unsigned long scan_code)
{
	if(scan_code == RIGHT_ARROW)
	{
		pacman_rotate(pacman, RIGHT);
	}
	else if(scan_code == UP_ARROW)
	{
		pacman_rotate(pacman, UP);
	}
	else if (scan_code == LEFT_ARROW)
	{
		pacman_rotate(pacman, LEFT);
	}
	else if (scan_code == DOWN_ARROW)
	{
		pacman_rotate(pacman, DOWN);
	}
}

void pacman_rotate(Pacman * pacman, int direction)
{
	if(pacman->direction == direction)
		return;

	char ** new_maps = pacman_maps(direction);

	pacman->img->sp->map = new_maps[pacman->img->cur_fig];
	pacman->img->map = new_maps;


	pacman->direction = direction;
}

void pacman_try_rotate(Pacman * pacman)
{
	if(pacman->direction == pacman->desired_direction)
		return;

	int prev_dir = pacman->direction;
	pacman->direction = pacman->desired_direction;

	if(pacman_check_surroundings(pacman) == 1)
	{
		pacman->direction = prev_dir;
		return;
	}
	pacman->direction = prev_dir;
	pacman_rotate(pacman, pacman->desired_direction);
}

char ** pacman_maps(int direction)
{
	if(direction == DOWN)
		return pacman_down_maps();
	else if (direction == UP)
		return pacman_up_maps();
	else if (direction == LEFT)
		return pacman_left_maps();
	else
		return pacman_right_maps();
}

char ** pacman_right_maps()
{
	char** maps;
	maps = malloc(6*sizeof(char *));
	int temp1, temp2;

	maps[0] = (char *)read_xpm( pacman_r0_xpm, &temp1, &temp2);
	maps[1] = (char *)read_xpm( pacman_r1_xpm, &temp1, &temp2);
	maps[2] = (char *)read_xpm( pacman_r2_xpm, &temp1, &temp2);
	maps[3] = (char *)read_xpm( pacman_r3_xpm, &temp1, &temp2);
	maps[4] = (char *)read_xpm( pacman_r2_xpm, &temp1, &temp2);
	maps[5] = (char *)read_xpm( pacman_r1_xpm, &temp1, &temp2);

	return maps;
}

char ** pacman_left_maps()
{
	char** maps;
	maps = malloc(6*sizeof(char *));
	int temp1, temp2;

	maps[0] = (char *)read_xpm( pacman_l0_xpm, &temp1, &temp2);
	maps[1] = (char *)read_xpm( pacman_l1_xpm, &temp1, &temp2);
	maps[2] = (char *)read_xpm( pacman_l2_xpm, &temp1, &temp2);
	maps[3] = (char *)read_xpm( pacman_l3_xpm, &temp1, &temp2);
	maps[4] = (char *)read_xpm( pacman_l2_xpm, &temp1, &temp2);
	maps[5] = (char *)read_xpm( pacman_l1_xpm, &temp1, &temp2);

	return maps;
}

char ** pacman_up_maps()
{
	char** maps;
	maps = malloc(6*sizeof(char *));
	int temp1, temp2;

	maps[0] = (char *)read_xpm( pacman_u0_xpm, &temp1, &temp2);
	maps[1] = (char *)read_xpm( pacman_u1_xpm, &temp1, &temp2);
	maps[2] = (char *)read_xpm( pacman_u2_xpm, &temp1, &temp2);
	maps[3] = (char *)read_xpm( pacman_u3_xpm, &temp1, &temp2);
	maps[4] = (char *)read_xpm( pacman_u2_xpm, &temp1, &temp2);
	maps[5] = (char *)read_xpm( pacman_u1_xpm, &temp1, &temp2);

	return maps;
}

char ** pacman_down_maps()
{
	char** maps;
	maps = malloc(6*sizeof(char *));
	int temp1, temp2;

	maps[0] = (char *)read_xpm( pacman_d0_xpm, &temp1, &temp2);
	maps[1] = (char *)read_xpm( pacman_d1_xpm, &temp1, &temp2);
	maps[2] = (char *)read_xpm( pacman_d2_xpm, &temp1, &temp2);
	maps[3] = (char *)read_xpm( pacman_d3_xpm, &temp1, &temp2);
	maps[4] = (char *)read_xpm( pacman_d2_xpm, &temp1, &temp2);
	maps[5] = (char *)read_xpm( pacman_d1_xpm, &temp1, &temp2);

	return maps;
}



Ghost * ghost_init(int xi, int yi, int speed, int color, int mode)
{
	Ghost * ghost;
	ghost = (Ghost *)malloc(sizeof(Ghost));

	ghost->mode = mode;
	ghost->direction = 1;
	ghost->speed = 2;
	ghost->color = color;

	ghost->img = malloc(sizeof(Sprite));

	switch(color)
	{
	case (int) COLOR_GHOST_ORANGE:
		ghost->img = create_sprite(ghost_orange_right, xi, yi);
		break;
	case (int) COLOR_GHOST_RED:
			ghost->img = create_sprite(ghost_red_right, xi, yi);
		break;
	case (int) COLOR_GHOST_BLUE:
			ghost->img = create_sprite(ghost_blue_right, xi, yi);
		break;
	case (int) COLOR_GHOST_PINK:
			ghost->img = create_sprite(ghost_pink_right, xi, yi);
		break;

	}

	return ghost;
}

void ghost_rotate(Ghost * ghost, int direction)
{
	if(ghost->direction == direction)
		return;

	ghost->img->map = (char *)ghost_dir_map(ghost->color, direction);

	ghost->direction = direction;
}

char* ghost_dir_map(int color, int direction)
{
	int temp1, temp2;
	switch(color)
	{
	case (int) COLOR_GHOST_ORANGE:
		{
		switch(direction)
		{
		case (int) DOWN:
				return (char *)read_xpm(ghost_orange_down, &temp1, &temp2);
		break;
		case (int) LEFT:
				return (char *)read_xpm(ghost_orange_left, &temp1, &temp2);
		break;
		case (int) RIGHT:
				return (char *)read_xpm(ghost_orange_right, &temp1, &temp2);
		break;
		case (int) UP:
				return (char *)read_xpm(ghost_orange_up, &temp1, &temp2);
		break;

		}
		break;
		}
	case (int) COLOR_GHOST_RED:
		{
		switch(direction)
		{
		case (int) DOWN:
				return (char *)read_xpm(ghost_red_down, &temp1, &temp2);
		break;
		case (int) LEFT:
				return (char *)read_xpm(ghost_red_left, &temp1, &temp2);
		break;
		case (int) RIGHT:
				return (char *)read_xpm(ghost_red_right, &temp1, &temp2);
		break;
		case (int) UP:
				return (char *)read_xpm(ghost_red_up, &temp1, &temp2);
		break;

		}
		break;
		}
	case (int) COLOR_GHOST_BLUE:
		{
		switch(direction)
		{
		case (int) DOWN:
				return (char *)read_xpm(ghost_blue_down, &temp1, &temp2);
		break;
		case (int) LEFT:
				return (char *)read_xpm(ghost_blue_left, &temp1, &temp2);
		break;
		case (int) RIGHT:
				return (char *)read_xpm(ghost_blue_right, &temp1, &temp2);
		break;
		case (int) UP:
				return (char *)read_xpm(ghost_blue_up, &temp1, &temp2);
		break;

		}
		break;
		}
	case (int) COLOR_GHOST_PINK:
		{
		switch(direction)
		{
		case (int) DOWN:
				return (char *)read_xpm(ghost_pink_down, &temp1, &temp2);
		break;
		case (int) LEFT:
				return (char *)read_xpm(ghost_pink_left, &temp1, &temp2);
		break;
		case (int) RIGHT:
				return (char *)read_xpm(ghost_pink_right, &temp1, &temp2);
		break;
		case (int) UP:
				return (char *)read_xpm(ghost_pink_up, &temp1, &temp2);
		break;

		}
		break;
		}
	}
	return NULL;
}

void ghost_read_key(Ghost * ghost, unsigned long scan_code)
{
	if(scan_code == RIGHT_ARROW)
	{
		ghost_rotate(ghost, RIGHT);
	}
	else if(scan_code == UP_ARROW)
	{
		ghost_rotate(ghost, UP);
	}
	else if (scan_code == LEFT_ARROW)
	{
		ghost_rotate(ghost, LEFT);
	}
	else if (scan_code == DOWN_ARROW)
	{
		ghost_rotate(ghost, DOWN);
	}
}

int ghost_check_surroundings(Ghost * ghost)
{
	int x;
	int y;
	int it;

	switch(ghost->direction)
	{
	case (int) DOWN:
		x = ghost->img->x;
		y = ghost->img->y; y += ghost->img->height;
		it = ghost->img->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x, y+1) == 1))
				return 1;
			x++;
		}
		return 0;
	case (int) RIGHT:
		x = ghost->img->x; x += ghost->img->width;
		y = ghost->img->y;
		it = ghost->img->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x+1, y) == 1))
				return 1;
			y++;
		}
		return 0;
	case (int) UP:
		x = ghost->img->x;
		y = ghost->img->y; y--;
		it = ghost->img->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x, y-1) == 1))
				return 1;
			x++;
		}
		return 0;
	case (int) LEFT:
		x = ghost->img->x; x--;
		y = ghost->img->y;
		it = ghost->img->width;

		for(;it > 0; it--)
		{
			if((*pixel_vid(x, y) == 1) || (*pixel_vid(x-1, y) == 1))
				return 1;
			y++;
		}
		return 0;
	}
}

void move_ghost(Ghost * ghost, Pacman * pacman)
{
	switch(ghost->mode)
	{
	case 0:			// random mode
		move_ghost_random(ghost);
		return;
	case 1:			// chase mode
		move_ghost_chase(ghost, pacman);
		return;
	case 2:			// user controlled mode - PENDING
		return;
	case 3:
		move_ghost_escape(ghost, pacman);
		return;
	}
}

void move_ghost_random(Ghost * ghost)
{
	int old_dir = ghost->direction;
	int new_dir = old_dir;
	if (1 == ghost_check_surroundings(ghost))
	{
		while(ghost_check_surroundings(ghost) != 0)
		{
			new_dir = rand_integer_between(0, 3);
			ghost->direction = new_dir;
		}
	}
	else
	{
		int sides[] = {0, 0, 0, 0};
		check_all_surroundings(ghost->img->x, ghost->img->y, ghost->img->width, ghost->img->height, sides);

		int left = sides[next_revclock_dir(new_dir)];
		int left_dir = next_revclock_dir(new_dir);
		int right = sides[prev_revclock_dir(new_dir)];
		int right_dir = prev_revclock_dir(new_dir);

		if((left == 0) && (right == 1))
		{
			if(probability(50))
			{
				new_dir = left_dir;
			}
		}
		else if ((left == 1) && (right == 0))
		{
			if(probability(50))
			{
				new_dir = right_dir;
			}
		}
		else if ((left == 0) && (right == 0))
		{
			if(probability(50))
			{
				if(probability(50))
					new_dir = right_dir;
				else
					new_dir = left_dir;
			}
		}
	}


	ghost->direction = old_dir;
	ghost_rotate(ghost, new_dir);

	switch(ghost->direction)
	{
	case 0:
		ghost->img->y += ghost->speed;
		if(ghost->img->y >= 768-28)
			ghost->img->y = 768-28;
		break;
	case 1:
		ghost->img->x += ghost->speed;
		if(ghost->img->x >= 1024-28)
			ghost->img->x = 1024-28;
		break;
	case 2:
		ghost->img->y -= ghost->speed;
		if(ghost->img->y <= 0)
			ghost->img->y = 0;
		break;
	case 3:
		ghost->img->x -= ghost->speed;
		if(ghost->img->x <= 0)
			ghost->img->x = 0;
		break;
	}
}

int get_pacman_dir(Ghost * ghost, Pacman * pacman)
{
	int xg, yg, xp, yp;
	xg = ghost->img->x;
	yg = ghost->img->y;
	xp = pacman->img->sp->x;
	yp = pacman->img->sp->y;

	if(xg == xp)		// Vertical line //
	{
		if(yp > yg)
			return (int) PACDIR_S;
		else
			return (int) PACDIR_N;
	}

	double slope = (double)((double)yg-(double)yp)/(double)((double)xg-(double)xp);

	//if((double)abs(slope) < (double)(0.15))		// Horizontal line //
	if(((double)slope < ((double)1/100)) && (double)slope > -1*((double)1/100))
	{
		if(xp > xg)
			return (int) PACDIR_E;
		return (int) PACDIR_W;
	}
	else				// Non horizontal nor vertical line //
	{
		if(abs(slope) > 500)
		{
			if(yg > yp)
				return (int) PACDIR_S;
			else
				return (int) PACDIR_N;
		}
		/*if(slope < -500)
		{
			if(yg > yp)
				return (int) PACDIR_N;
			else
				return (int) PACDIR_S;
		}*/

		if(slope > 1)
		{
			if(yg > yp)
				return (int) PACDIR_NNW;
			return (int) PACDIR_SSE;
		}
		else if((slope <= 1) && (slope > 0))
		{
			if(xg > xp)
				return (int) PACDIR_WNW;
			return (int) PACDIR_ESE;
		}
		else if((slope < 0) && (slope >= -1))
		{
			if(xp > xg)
				return (int) PACDIR_ENE;
			return (int) PACDIR_WSW;
		}
		else if(slope < -1)
		{
			if(yp < yg)
				return (int) PACDIR_NNE;
			return (int) PACDIR_SSW;
		}
	}

	return -1; // "default" in case of failure
}

void move_ghost_chase(Ghost * ghost, Pacman * pacman)
{
	int pac_dir = get_pacman_dir(ghost, pacman);
	int old_dir = ghost->direction;
	int new_dir = old_dir;

	if(pac_dir == -1)
	{
		printf("PACMAN DIRECTION FAILED!\n");
		return;
	}

	if(pac_dir == (int) PACDIR_S)
	{
		new_dir = (int) DOWN;
		ghost->direction = (int) DOWN;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_N)
	{
		new_dir = (int) UP;
		ghost->direction = (int) UP;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_E)
	{
		new_dir = (int) RIGHT;
		ghost->direction = (int) RIGHT;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_W)
	{
		new_dir = (int) LEFT;
		ghost->direction = (int) LEFT;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_NNE)
	{
		new_dir = (int) UP;
		ghost->direction = (int) UP;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) RIGHT;
			ghost->direction = (int) RIGHT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_NNW)
	{
		new_dir = (int) UP;
		ghost->direction = (int) UP;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) LEFT;
			ghost->direction = (int) LEFT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_ENE)
	{
		new_dir = (int) RIGHT;
		ghost->direction = (int) RIGHT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) UP;
			ghost->direction = (int) UP;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_ESE)
	{
		new_dir = (int) RIGHT;
		ghost->direction = (int) RIGHT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) DOWN;
			ghost->direction = (int) DOWN;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_WNW)
	{
		new_dir = (int) LEFT;
		ghost->direction = (int) LEFT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) UP;
			ghost->direction = (int) UP;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_WSW)
	{
		new_dir = (int) LEFT;
		ghost->direction = (int) LEFT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) DOWN;
			ghost->direction = (int) DOWN;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_SSW)
	{
		new_dir = (int) DOWN;
		ghost->direction = (int) DOWN;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) LEFT;
			ghost->direction = (int) LEFT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_SSE)
	{
		new_dir = (int) DOWN;
		ghost->direction = (int) DOWN;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) RIGHT;
			ghost->direction = (int) RIGHT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}

	// KEEP THIS PART, THIS DOES NOT INFLUENCE "CHASE BEHAVIOUR"
	ghost->direction = old_dir;
	ghost_rotate(ghost, new_dir);

	switch(ghost->direction)
	{
	case 0:
		ghost->img->y += ghost->speed;
		if(ghost->img->y >= 768-28)
			ghost->img->y = 768-28;
		break;
	case 1:
		ghost->img->x += ghost->speed;
		if(ghost->img->x >= 1024-28)
			ghost->img->x = 1024-28;
		break;
	case 2:
		ghost->img->y -= ghost->speed;
		if(ghost->img->y <= 0)
			ghost->img->y = 0;
		break;
	case 3:
		ghost->img->x -= ghost->speed;
		if(ghost->img->x <= 0)
			ghost->img->x = 0;
		break;
	}
}

void move_ghost_escape(Ghost * ghost, Pacman * pacman)
{
	int pac_dir = get_pacman_dir(ghost, pacman);
	int old_dir = ghost->direction;
	int new_dir = old_dir;

	if(pac_dir == -1)
	{
		printf("PACMAN DIRECTION FAILED!\n");
		return;
	}

	if(pac_dir == (int) PACDIR_S)
	{
		new_dir = (int) UP;
		ghost->direction = (int) UP;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_N)
	{
		new_dir = (int) DOWN;
		ghost->direction = (int) DOWN;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_E)
	{
		new_dir = (int) LEFT;
		ghost->direction = (int) LEFT;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_W)
	{
		new_dir = (int) RIGHT;
		ghost->direction = (int) RIGHT;

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_NNE)
	{
		new_dir = (int) DOWN;
		ghost->direction = (int) DOWN;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) LEFT;
			ghost->direction = (int) LEFT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_NNW)
	{
		new_dir = (int) DOWN;
		ghost->direction = (int) DOWN;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) RIGHT;
			ghost->direction = (int) RIGHT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_ENE)
	{
		new_dir = (int) LEFT;
		ghost->direction = (int) LEFT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) DOWN;
			ghost->direction = (int) DOWN;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_ESE)
	{
		new_dir = (int) LEFT;
		ghost->direction = (int) LEFT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) UP;
			ghost->direction = (int) UP;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_WNW)
	{
		new_dir = (int) RIGHT;
		ghost->direction = (int) RIGHT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) DOWN;
			ghost->direction = (int) DOWN;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) UP;
				ghost->direction = (int) UP;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_WSW)
	{
		new_dir = (int) RIGHT;
		ghost->direction = (int) RIGHT;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) UP;
			ghost->direction = (int) UP;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_SSW)
	{
		new_dir = (int) UP;
		ghost->direction = (int) UP;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) RIGHT;
			ghost->direction = (int) RIGHT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) LEFT;
				ghost->direction = (int) LEFT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}
	else if(pac_dir == (int) PACDIR_SSE)
	{
		new_dir = (int) UP;
		ghost->direction = (int) UP;

		if(ghost_check_surroundings(ghost) == 1)
		{
			new_dir = (int) LEFT;
			ghost->direction = (int) LEFT;
		}

		while(ghost_check_surroundings(ghost) == 1)
		{
			if(probability(50))
			{
				new_dir = (int) RIGHT;
				ghost->direction = (int) RIGHT;
			}
			else
			{
				new_dir = (int) DOWN;
				ghost->direction = (int) DOWN;
			}
		}
	}

	// KEEP THIS PART, THIS DOES NOT INFLUENCE "EVADE BEHAVIOUR"
	ghost->direction = old_dir;
	ghost_rotate(ghost, new_dir);

	switch(ghost->direction)
	{
	case 0:
		ghost->img->y += ghost->speed;
		if(ghost->img->y >= 768-28)
			ghost->img->y = 768-28;
		break;
	case 1:
		ghost->img->x += ghost->speed;
		if(ghost->img->x >= 1024-28)
			ghost->img->x = 1024-28;
		break;
	case 2:
		ghost->img->y -= ghost->speed;
		if(ghost->img->y <= 0)
			ghost->img->y = 0;
		break;
	case 3:
		ghost->img->x -= ghost->speed;
		if(ghost->img->x <= 0)
			ghost->img->x = 0;
		break;
	}
}









int probability(int percentage)
{
	int result = rand_integer_between(0, 100);
	if(result < percentage)
		return 1;
	return 0;
}

void check_all_surroundings(int xi, int yi, int width, int height, int sides[])
{
	int x, y, it;

	sides[0] = 0;
	sides[1] = 0;
	sides[2] = 0;
	sides[3] = 0;

	// CHECK DOWN //
	x = xi;
	y = yi; y += height;
	it = width;

	for(;it > 0; it--)
	{
		if((*pixel_vid(x, y) == 1) || (*pixel_vid(x, y+1) == 1))
			sides[0] = 1;

		if(sides[0] == 1)
			break;
		x++;
	}


	// CHECK RIGHT //
	x = xi; x += width;
	y = yi;
	it = width;

	for(;it > 0; it--)
	{
		if((*pixel_vid(x, y) == 1) || (*pixel_vid(x+1, y) == 1))
			sides[1] = 1;

		if(sides[1] == 1)
			break;
		y++;
	}

	// CHECK UP //
	x = xi;
	y = yi; y--;
	it = width;

	for(;it > 0; it--)
	{
		if((*pixel_vid(x, y) == 1) || (*pixel_vid(x, y-1) == 1))
			sides[2] = 1;

		if(sides[2] == 1)
			break;
		x++;
	}

	// CHECK LEFT //
	x = xi; x--;
	y = yi;
	it = width;

	for(;it > 0; it--)
	{
		if((*pixel_vid(x, y) == 1) || (*pixel_vid(x-1, y) == 1))
			sides[3] = 1;

		if(sides[3] == 1)
			break;
		y++;
	}
}

char *pixel(char* map, int width, int heigth, int x, int y)
{
	return map + y*width + x;
}

int rand_integer_between(int a, int b)
{
	return rand()%(b-a+1) + a;
}

int next_revclock_dir(int dir)
{
	return (dir + 1)%4;
}

int prev_revclock_dir(int dir)
{
	return (4+dir-1)%4;
}

int are_opposite_directions(int dir1, int dir2)
{
	return (abs(dir2-dir1) == 2);
}
