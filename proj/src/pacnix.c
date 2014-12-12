#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include <stdio.h>
#include <minix/sysutil.h>
#include <stdint.h>
#include <math.h>

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

// Initialize frame rate counters. Frame rate set to 50
double period_ints = (double) 50 / 60;
double sum_period = 0;

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
	pacman = pacman_init(200, 200, 3);

	// Initialize orange ghost
	Ghost *orange_ghost;
	orange_ghost = malloc(sizeof(Ghost));
	orange_ghost = ghost_init(100, 100, 2, 0, 0);

	// Initialize packet read
	Mouse_packet tmp_delta;

	// Set mouse stream mode
	set_stream();

	// Initialize VRAM
	vg_init(GRAF_1024x768);
	//vg_init(0x117);

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
					if(fps_tick())
					{
						fill_screen(COLOR_BLACK);

						draw_line(20, 20, 1000, 20, 1);
						draw_line(20, 20, 20, 700, 1);
						draw_line(20, 700, 1000, 700, 1);
						draw_line(1000, 700, 1000, 20, 1);
						draw_line(700, 400, 700, 600, 1);
						draw_line(700, 370, 700, 100, 1);
						draw_line(5, 0, 5, 28, 1);
						draw_line(20, 50, 50, 50, 1);


						pacman_move_tick(pacman);

						draw_img(orange_ghost->img->sp);

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
	sum_period += period_ints;

	if(abs(sum_period-1) < 0.001)
	{
		sum_period = 0;
		printf("YES\n");
		return 1;
	}

	printf("NO\n");
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

	ghost->img = malloc(sizeof(AnimSprite));

	char** maps;
	maps = malloc(sizeof(char *));
	int temp1, temp2;

	switch(color)
	{
	case 0:
		maps[0] = (char *)read_xpm(ghost_orange_right, &temp1, &temp2);
		ghost->img = create_asprite(maps, 1, speed, xi, yi, ghost_orange_right);
		break;
	case 1:
		maps[0] = (char *)read_xpm(ghost_red_right, &temp1, &temp2);
		ghost->img = create_asprite(maps, 1, speed, xi, yi, ghost_red_right);
		break;
	case 2:
		maps[0] = (char *)read_xpm(ghost_blue_right, &temp1, &temp2);
		ghost->img = create_asprite(maps, 1, speed, xi, yi, ghost_blue_right);
		break;
	case 3:
		maps[0] = (char *)read_xpm(ghost_pink_right, &temp1, &temp2);
		ghost->img = create_asprite(maps, 1, speed, xi, yi, ghost_pink_right);
		break;

	}

	return ghost;
}

void ghost_rotate(Ghost * ghost, int direction)
{
	if(ghost->direction == direction)
		return;

	//char ** new_maps = ghost_dir_map(ghost->color, direction);

	char** new_maps;
	ghost->img->sp->map = new_maps[0];
	ghost->img->map = new_maps;


	ghost->direction = direction;
}



char *pixel(char* map, int width, int heigth, int x, int y)
{
	return map + y*width + x;
}
