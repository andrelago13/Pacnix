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

void pacnix_start()
{
	interrupts();
	printf("\n\n\tProgram ended\n\n");
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
/*
	// Initialize pacman TEST==================================
	Pacman pacman;
	pacman.img.x = 200;
	pacman.img.y = 200;
	pacman.direction = 1;
	pacman.img.map = (char *)read_xpm(pacman_left, &pacman.img.width, &pacman.img.height);
*/

	Pacman *pacman;
	pacman = malloc(sizeof(Pacman));
	pacman = pacman_init(200, 200, 3);

	////// TESTING ANIM ///////
	AnimSprite *anim;
	char** maps;
	maps = malloc(6 * sizeof(char *));
	int temp1, temp2;

	maps[0] = (char *)read_xpm(pacman_r0_xpm, &temp1, &temp2);
	maps[1] = (char *)read_xpm(pacman_r1_xpm, &temp1, &temp2);
	maps[2] = (char *)read_xpm(pacman_r2_xpm, &temp1, &temp2);
	maps[3] = (char *)read_xpm(pacman_r3_xpm, &temp1, &temp2);
	maps[4] = (char *)read_xpm(pacman_r2_xpm, &temp1, &temp2);
	maps[5] = (char *)read_xpm(pacman_r1_xpm, &temp1, &temp2);

	anim = create_asprite(maps, 6, 2, 200, 200, ghost_pink_left);
	//maps[1] = read_xp
	//anim = *create_asprite()

	// Initialize packet_read
	Mouse_packet tmp_delta;

	// Initialize mouse stream mode
	set_stream();

	// Initialize frame rate counters. Frame rate set to 50

	double period_ints = (double) 50 / 60;
	double sum_period = 0;

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
				if (msg.NOTIFY_ARG & irq_set_mouse)		//// MOUSE INTERRUPT ////
				{
					ret = mouse_read_packet(&tmp_delta);

					if(ret == 1)
						update_mouse(&mouse, &tmp_delta);
				}
				else if (msg.NOTIFY_ARG & irq_set_timer)		//// TIMER 0 INTERRUPT ////
				{
					unsigned long letra = 0;
					sys_inb(0x64, &letra);
					printf("=> %u\n", letra & 1);



					sum_period += period_ints;

					if(abs(sum_period-1) < 0.1)
					{
						sum_period = 0;
						fill_screen(COLOR_BLACK);
//*
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
						draw_img(pacman->img->sp);
//*/
						animate_asprite(anim);
						animate_asprite(pacman->img);
						draw_img(anim->sp);


						draw_mouse(&mouse);
						update_buffer();
					}
				}
				else if (msg.NOTIFY_ARG & irq_set_kbd)			//// KEYBOARD INTERRUPT ////
				{
					unsigned long letra = 0;

					if(OK != sys_inb(KBD_OUT_BUF, &letra))	// Read scancode
						return;

					if(letra == ESC_break)
					{
						terminus = 0;
						dis_stream();
					}
//*
					if(letra == RIGHT_ARROW)
					{
						pacman_rotate(pacman, RIGHT);
						pacman->direction = RIGHT;
					}
					else if(letra == UP_ARROW)
					{
						pacman_rotate(pacman, UP);
						pacman->direction = UP;
					}
					else if (letra == LEFT_ARROW)
					{
						pacman_rotate(pacman, LEFT);
						pacman->direction = LEFT;
					}
					else if (letra == DOWN_ARROW)
					{
						pacman_rotate(pacman, DOWN);
						pacman->direction = DOWN;
					}

					sys_inb(KBD_OUT_BUF, &letra);
//*/
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

	vg_exit();
}

Pacman * pacman_init(int xi, int yi, int speed)
{
	Pacman * pacman;
	pacman = (Pacman *)malloc(sizeof(Pacman));

	pacman->mode = 0;
	pacman->direction = 1;

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
	/*
	 * AnimSprite *anim;
	char** maps;
	maps = malloc(4 * sizeof(char *));
	int temp1, temp2;

	Sprite sp;
	sp.map = (char *)read_xpm(ghost_pink_left, &sp.width, &sp.height);

	maps[0] = (char *)read_xpm(ghost_pink_left, &temp1, &temp2);
	maps[1] = (char *)read_xpm(ghost_blue_left, &temp1, &temp2);
	maps[2] = (char *)read_xpm(ghost_red_left, &temp1, &temp2);
	maps[3] = (char *)read_xpm(ghost_orange_left, &temp1, &temp2);

	anim = create_asprite(maps, 4, 2, 200, 200, ghost_pink_left);
	 */

	//pacman->img = create_asprite()
}

void pacman_rotate(Pacman * pacman, int direction)
{
	printf("ROTATE\n");
	if(pacman->direction == direction)
		return;
	printf("====\n");

	char ** new_maps = pacman_maps(direction);

	pacman->img->sp->map = new_maps[pacman->img->cur_fig];
	pacman->img->map = new_maps;
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

char *pixel(char* map, int width, int heigth, int x, int y)
{
	return map + y*width + x;
}
