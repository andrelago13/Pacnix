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
#include "pac_menu.h"

// Initialize frame rate counters. Frame rate set to 50
double counter;
int tick_counter;
Pacman_map *map;

// Initialize mouse cursor
Mouse_coord mouse;

int pause_state;

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
	mouse.x_coord = 450;
	mouse.y_coord = 400;
	mouse.img.x = mouse.x_coord;
	mouse.img.y = mouse.y_coord;
	mouse.img.map = (char *)read_xpm(cursor, &mouse.img.width, &mouse.img.height);

	tick_counter = 0;
	pause_state = 0;
	counter = 0;

	int prev_score = 0;
	initialize_menu_pieces();
	int ret = start_menu(prev_score, 0);
	clear_menu_pieces();
	int end_prog = 0;

	while(end_prog == 0)
	{
		printf("RET : %d\n", ret);
		reset_mouse_packets();
		switch(ret)
		{
		case 0:
			initialize_map_pieces();
			ret = game_local(0);
			clear_map_pieces();
			prev_score = ret;
			ret = 5;
			break;
		case 1:
			initialize_map_pieces();
			game_local(1);
			clear_map_pieces();
			ret = 5;
			break;
		case 2:
			initialize_menu_pieces();
			ret = start_menu(prev_score, 1);
			clear_menu_pieces();
			break;
		case 3:
			initialize_menu_pieces();
			ret = start_menu(prev_score, 2);
			clear_menu_pieces();
			break;
		case 4:
			end_prog=1;
			break;
		case 5:
			initialize_menu_pieces();
			ret = start_menu(prev_score, 0);
			clear_menu_pieces();
			break;
		}
	}
}

int start_menu(int prev_score, int screen)
{
	/*
//	FILE * fp;
//	fp = (FILE *)open("/home/lcom/repos/proj/src/scores.txt", "w+");
//	char *str1 = "hello";
//	fprintf(fp,  "more: %s\n", str1);
//	fclose(fp);
//	return;

	tick_counter = 0;
	pause_state = 0;
	initialize_map_pieces();
	counter = 0;
	int ret = game_local(1);*/

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
	int end_status = 0;

	// Initialize packet read
	Mouse_packet tmp_delta;

	// Set mouse stream mode
	set_stream();

	int menu_option = 0;

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
					{
						update_mouse(&mouse, &tmp_delta);

						if(screen == 0)
						{
							menu_option = check_mainmenu_click(&mouse);
							if(menu_option != -1)
							{
								terminus = 0;
								dis_stream();
								tmp_delta.rb = 0;
								tmp_delta.lb = 0;
								tmp_delta.mb = 0;
								update_mouse(&mouse, &tmp_delta);
							}

						}
						else if(screen == 1)
						{

						}
						else if(screen == 2)
						{

						}

					}
				}

				if (msg.NOTIFY_ARG & irq_set_timer)		//////////////////////////////// TIMER 0 INTERRUPT /////////////////////////////
				{
					if(fps_tick() == 1)
					{
						fill_screen(COLOR_BLACK);

						if(screen == 0)
						{
							draw_main_menu(&mouse);

							if(prev_score > 0)
							{
								draw_score_header(850, 500);
								draw_num(prev_score, 970, 528, 2);
							}
						}
						else if(screen == 1)
						{
							draw_inst_menu();
						}
						else if(screen == 2)
						{

						}



						draw_mouse(&mouse);
						update_buffer();
					}

				}

				if (msg.NOTIFY_ARG & irq_set_kbd)			///////////////////////////// KEYBOARD INTERRUPT /////////////////////////
				{
					unsigned long letra = 0;

					if(OK != sys_inb(KBD_OUT_BUF, &letra))	// Read scancode
						return;

					if(screen == 0)
					{
						if(letra == ESC_break)
						{
							terminus = 0;
							menu_option = 4;
							dis_stream();
						}

						int ret = arrow_click(letra);
						if(ret != -1)
						{
							printf("OPTION : %d\n", ret);
							terminus = 0;
							menu_option = ret;
							dis_stream();
						}
					}
					else if(screen == 1)
					{
						if(letra == ESC_break)
						{
							terminus = 0;
							menu_option = 5;
							dis_stream();
						}
					}
					else if(screen == 2)
					{
						if(letra == ESC_break)
						{
							terminus = 0;
							menu_option = 5;
							dis_stream();
						}
					}

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

	return menu_option;
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

int game_local(int game_mode)
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
	int end_status = 0;

	// Initialize pacman
	Pacman *pacman;
	pacman = malloc(sizeof(Pacman));
	pacman = pacman_init(374, 480, 3, 3);
	pacman->spawn_timer = 2;

	// Initialize orange ghost
	Ghost *orange_ghost;
	orange_ghost = malloc(sizeof(Ghost));
	orange_ghost = ghost_init(374, 270, 2, COLOR_GHOST_ORANGE, 0);
	orange_ghost->spawn_timer = 8;

	// Initialize pink ghost
	Ghost *pink_ghost;
	pink_ghost = malloc(sizeof(Ghost));
	pink_ghost = ghost_init(374, 270, 2, COLOR_GHOST_PINK, 4);
	pink_ghost->chase_time = 10;
	pink_ghost->random_time = 10;
	pink_ghost->temp_mode = 0;
	pink_ghost->spawn_timer = 3;

	// Initialize red ghost
	Ghost *red_ghost;
	red_ghost = malloc(sizeof(Ghost));
	red_ghost = ghost_init(374, 270, 2, COLOR_GHOST_RED, 4);
	red_ghost->chase_time = 5;
	red_ghost->random_time = 5;
	red_ghost->temp_mode = 0;
	red_ghost->spawn_timer = 2;

	// Initialize blue ghost
	Ghost *blue_ghost;
	blue_ghost = malloc(sizeof(Ghost));
	blue_ghost = ghost_init(374, 270, 2, COLOR_GHOST_BLUE, 4);
	blue_ghost->chase_time = 8;
	blue_ghost->random_time = 5;
	blue_ghost->temp_mode = 0;
	blue_ghost->spawn_timer = 6;

	// Store all ghost pointers in an array
	Ghost *all_ghosts[4];
	all_ghosts[0] = orange_ghost;
	all_ghosts[1] = blue_ghost;
	all_ghosts[2] = red_ghost;
	all_ghosts[3] = pink_ghost;

	// Initialize game map 1
	Pacman_map *map1;
	map1 = (Pacman_map *)malloc(sizeof(Pacman_map));
	map1 = map1_initialize(30, 30);
	map = map1;
	int num_dots = map1->num_dots;
	int num_energizers = map1->num_energizers;

	int score = 0;

	// Initialize game ending info
	AnimSprite *pacman_dead;
	pacman_dead = (AnimSprite *)malloc(sizeof(AnimSprite));

	char** maps;
	maps = malloc(7*sizeof(char *));
	int temp1, temp2;

	maps[0] = (char *)read_xpm( pac_dead_1_xpm, &temp1, &temp2);
	maps[1] = (char *)read_xpm( pac_dead_1_xpm, &temp1, &temp2);
	maps[2] = (char *)read_xpm( pac_dead_1_xpm, &temp1, &temp2);
	maps[3] = (char *)read_xpm( pac_dead_1_xpm, &temp1, &temp2);
	maps[4] = (char *)read_xpm( pac_dead_2_xpm, &temp1, &temp2);
	maps[5] = (char *)read_xpm( pac_dead_3_xpm, &temp1, &temp2);
	maps[6] = (char *)read_xpm( pac_dead_4_xpm, &temp1, &temp2);
	maps[7] = (char *)read_xpm( pac_dead_5_xpm, &temp1, &temp2);
	maps[8] = (char *)read_xpm( pac_dead_6_xpm, &temp1, &temp2);
	maps[9] = (char *)read_xpm(blank_xpm, &temp1, &temp2);

	pacman_dead = (AnimSprite *)create_asprite(maps, 10, 11, 0, 0, pac_dead_1_xpm);

	int game_ended = 0;
	int end_counter = 10;

	// Initialize packet read
	Mouse_packet tmp_delta;

	// Initialize game-paused sprite
	Sprite *paused_game;
	paused_game = (Sprite *)malloc(sizeof(Sprite));
	paused_game->map = (char *)read_xpm(pause_xpm, &paused_game->width, &paused_game->height);
	paused_game->x = 740;
	paused_game->y = 400;

	// Set mouse stream mode
	set_stream();

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
					{
						update_mouse(&mouse, &tmp_delta);

						if((pause_state == 0) && (game_mode != 0) && (game_ended == 0))
							check_for_click(all_ghosts, &mouse);
					}
				}
				if (msg.NOTIFY_ARG & irq_set_timer)		//////////////////////////////// TIMER 0 INTERRUPT /////////////////////////////
				{
					if((fps_tick() == 1) && (game_ended == 0))
					{
						fill_screen(COLOR_BLACK);

						draw_map(map1);

						check_pacman_teleport(pacman, map1);

						if(cell_type(pacman->img->sp->x + pacman->img->sp->width/2, pacman->img->sp->y + pacman->img->sp->height/2, map1) == 2)
						{
							fill_cell(pacman->img->sp->x + pacman->img->sp->width/2, pacman->img->sp->y + pacman->img->sp->height/2, map1, 0);
							all_ghosts_escape(all_ghosts, 8);
							num_energizers--;
							score += 50;
						}

						if(cell_type(pacman->img->sp->x + pacman->img->sp->width/2, pacman->img->sp->y + pacman->img->sp->height/2, map1) == 1)
						{
							fill_cell(pacman->img->sp->x + pacman->img->sp->width/2, pacman->img->sp->y + pacman->img->sp->height/2, map1, 0);
							num_dots--;
							score += 10;
							// INCREASE SCORE //
						}
						if((num_dots == 0) && (num_energizers == 0))
						{
							terminus = 0;
							end_status = 3;
						}

						check_ghosts_teleport(all_ghosts, map1);

						draw_img(orange_ghost->img);
						draw_img(blue_ghost->img);
						draw_img(pink_ghost->img);
						draw_img(red_ghost->img);

						draw_img(pacman->img->sp);

						if(pause_state == 0)
						{
							pacman_move_tick(pacman);
							move_ghost(orange_ghost, pacman);
							move_ghost(blue_ghost, pacman);
							move_ghost(pink_ghost, pacman);
							move_ghost(red_ghost, pacman);
							animate_asprite(pacman->img);
						}

						int collision = check_collisions(all_ghosts, pacman);
						if(collision != -1)
						{
							if((all_ghosts[collision]->mode == 3) || (all_ghosts[collision]->mode == 5))
							{
								// GHOST DIES
								ghost_eaten(all_ghosts[collision]);
								score += 300;
							}
							else
							{
								// PACMAN DIES
								pacman->lives--;
								if(pacman->lives <= 0)
								{
									if(game_mode == 0)
									{
										end_status = 1;
										game_ended = 1;
										pacman_dead->sp->x = pacman->img->sp->x;
										pacman_dead->sp->y = pacman->img->sp->y;
									}
									else if(game_mode == 1)
									{
										end_status = 2;
										game_ended = 1;
										pacman_dead->sp->x = pacman->img->sp->x;
										pacman_dead->sp->y = pacman->img->sp->y;
									}
								}
								else
								{
									pacman->img->sp->x = 374;
									pacman->img->sp->y = 480;
									pacman->desired_direction = (int) RIGHT;
								}
								pacman->spawn_timer = 2;
								reset_all_ghosts(all_ghosts);
							}
						}

						draw_mouse(&mouse);
						draw_lives(pacman->lives, 850, 100);

						if(game_mode == 0)
						{
							draw_score_header(850, 150);
							draw_num(score, 970, 178, 2);
						}

						if(pause_state == 1)
						{
							draw_img(paused_game);
						}

						update_buffer();
					}
					else if((fps_tick() == 1) && (game_ended == 1))
					{
						fill_screen(COLOR_BLACK);

						draw_map(map1);

						animate_asprite(pacman_dead);
						draw_img(pacman_dead->sp);
						if(pacman_dead->cur_fig == pacman_dead->num_fig - 1)
							end_counter--;
						if(end_counter == 0)
							terminus = 0;


						draw_mouse(&mouse);

						if(game_mode == 0)
						{
							draw_score_header(850, 150);
							draw_num(score, 970, 178, 2);
						}
						update_buffer();
					}


					if(pause_state == 0)
					{
						all_ghosts_spawn_timer(all_ghosts);
						pacman_spawn_timer(pacman);
						all_ghosts_escape_tick(all_ghosts);
					}

				}
				if (msg.NOTIFY_ARG & irq_set_kbd)			///////////////////////////// KEYBOARD INTERRUPT /////////////////////////
				{
					unsigned long letra = 0;

					if(OK != sys_inb(KBD_OUT_BUF, &letra))	// Read scancode
						return;

					if(game_ended == 0)
					{
						if(letra == ESC_break)
						{
							pause_state = abs(pause_state - 1);
						}
						else if((letra == E_KEY) && (pause_state == 1))
						{
							terminus = 0;
							end_status = 0;
							dis_stream();
						}

						if(pause_state == 0)
						{
							if(game_mode != 0)
								check_user_ghosts(all_ghosts, letra);
							pacman_read_key(pacman, letra);
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}

	// Unsubscribe all interrupts
	dis_stream();
	mouse_unsubscribe(&mouse_hook);
	timer_unsubscribe(&timer_hook);
	kbd_unsubscribe_int();
	empty_buf();

	free(pacman);
	free(pink_ghost);
	free(red_ghost);
	free(blue_ghost);
	free(orange_ghost);
	free(map1);
	free(paused_game);

	pause_state = 0;

	if(game_mode == 0)
		return score;

	return 0;
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

Pacman * pacman_init(int xi, int yi, int speed, int lives)
{
	Pacman * pacman;
	pacman = (Pacman *)malloc(sizeof(Pacman));

	pacman->mode = 0;
	pacman->direction = 1;
	pacman->desired_direction = 1;
	pacman->lives = lives;

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

	tick_counter++; tick_counter = tick_counter % 60;

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
	if(pacman->spawn_timer != 0)
		return;

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

void pacman_spawn_timer(Pacman * pacman)
{
	if(tick_counter == 0)
	{
		if(pacman->spawn_timer > 0)
			pacman->spawn_timer = pacman->spawn_timer - 1;
		else
			pacman->spawn_timer = 0;
	}
	int temp1, temp2;
	if(pacman->spawn_timer > 0)
	{
		pacman->img->sp->map = (char *)read_xpm(pacman_r3_xpm, &temp1, &temp2);
	}
}




Ghost * ghost_init(int xi, int yi, int speed, int color, int mode)
{
	Ghost * ghost;
	ghost = (Ghost *)malloc(sizeof(Ghost));

	ghost->mode = mode;
	ghost->temp_mode = 0;
	ghost->prev_mode = mode;
	if(probability(50))
	{
		ghost->direction = 1;
		ghost->desired_direction = 1;
	}
	else
	{
		ghost->direction = 3;
		ghost->desired_direction = 3;
	}

	ghost->speed = 2;
	ghost->color = color;
	ghost->detouring = 0;

	ghost->chase_time = 0;
	ghost->random_time = 0;
	ghost->curr_counter = 0;

	ghost->escape_counter = 0;

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
	if(ghost->mode == 3)
	{
		if((ghost->escape_counter == 0) || (ghost->escape_counter == 2))
			ghost->img->map = (char *)ghost_dir_map(9, direction);
		else
			ghost->img->map = (char *)ghost_dir_map(10, direction);
	}
	else if(ghost->mode == 5)
	{
		if((ghost->escape_counter == 0) || (ghost->escape_counter == 2))
			ghost->img->map = (char *)ghost_dir_map(9, direction);
		else
			ghost->img->map = (char *)ghost_dir_map(10, direction);
	}
	else
	{
		ghost->img->map = (char *)ghost_dir_map(ghost->color, direction);
	}
	ghost->direction = direction;
}

char* ghost_dir_map(int color, int direction)
{
	int temp1, temp2;


	if(color ==  9)		// Special case, when pacman is energized
		return (char *)read_xpm(ghost_dead_white, &temp1, &temp2);
	if(color == 10)
		return (char *)read_xpm(ghost_dead_blue, &temp1, &temp2);

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
	if(ghost->spawn_timer > 0)
		return;

	switch(ghost->mode)
	{
	case 0:			// random mode
		move_ghost_random(ghost);
		return;
	case 1:			// chase pacman mode
		move_ghost_chase(ghost, pacman);
		return;
	case 2:			// user controlled mode
		move_ghost_user(ghost);
		return;
	case 3:			// escape pacman mode
		if(probability(70))					// to increase easyness of ghost catching
			move_ghost_escape(ghost, pacman);
		else
			move_ghost_random(ghost);
		return;
	case 4:			// timed ghost - switches between chase and evade modes
		move_timed_ghost(ghost, pacman);
		return;
	case 5:
		move_ghost_user_esc(ghost);
		return;
	}
}

void move_timed_ghost(Ghost * ghost, Pacman * pacman)
{
	if(tick_counter == 0)
	{
		ghost->curr_counter++;
		switch(ghost->temp_mode)
		{
		case 0:
			if(ghost->curr_counter == ghost->random_time)
			{
				ghost->curr_counter = 0;
				ghost->temp_mode = 1;
			}
			break;
		case 1:
			if(ghost->curr_counter == ghost->chase_time)
			{
				ghost->curr_counter = 0;
				ghost->temp_mode = 0;
			}
			break;
		}
	}

	switch(ghost->temp_mode)
	{
	case 0:
		move_ghost_random(ghost);
		break;
	case 1:
		move_ghost_chase(ghost, pacman);
		break;
	}

	return;
}

void move_ghost_random(Ghost * ghost)
{
	ghost->detouring = 0;
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
		if(abs(slope) > 100)	// CHANGE IF PATH-FIND NOT WORKING WELL
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

	int sides[] = {0, 0, 0, 0};
	check_all_surroundings(ghost->img->x, ghost->img->y, ghost->img->width, ghost->img->height, sides);

	if((sides[(ghost->direction + 1) % 4] == 1) && (sides[(ghost->direction+3) % 4] == 1) && (ghost_check_surroundings(ghost) == 1))
		new_dir = (old_dir + 2) % 4;
	else if((sides[(ghost->direction + 1) % 4] == 1) && (sides[(ghost->direction+3) % 4] == 1))
		new_dir = old_dir;
	else
	{

		if(pac_dir == (int) PACDIR_S)
		{
			new_dir = (int) DOWN;
			ghost->direction = (int) DOWN;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) DOWN)
				ghost->detouring = 0;
		}
		else if(pac_dir == (int) PACDIR_N)
		{
			new_dir = (int) UP;
			ghost->direction = (int) UP;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) UP)
				ghost->detouring = 0;
		}
		else if(pac_dir == (int) PACDIR_E)
		{
			new_dir = (int) RIGHT;
			ghost->direction = (int) RIGHT;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) RIGHT)
				ghost->detouring = 0;
		}
		else if(pac_dir == (int) PACDIR_W)
		{
			new_dir = (int) LEFT;
			ghost->direction = (int) LEFT;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) LEFT)
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) UP) || (new_dir == (int) RIGHT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) UP) || (new_dir == (int) LEFT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) UP) || (new_dir == (int) RIGHT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) DOWN) || (new_dir == (int) RIGHT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) UP) || (new_dir == (int) LEFT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) LEFT) || (new_dir == (int) DOWN))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) LEFT) || (new_dir == (int) DOWN))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) DOWN) || (new_dir == (int) RIGHT))
				ghost->detouring = 0;
		}

	}


	// KEEP THIS PART, THIS DOES NOT INFLUENCE "CHASE BEHAVIOUR"
	ghost->direction = old_dir;
	ghost_rotate(ghost, new_dir);

	//printf("FINAL : %d\n", ghost->direction);

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

void move_ghost_user(Ghost * ghost)
{
	ghost->detouring = 0;

	ghost_try_rotate(ghost);

	if (1 == ghost_check_surroundings(ghost))
		return;

	switch(ghost->direction)
	{
	case 0:
		ghost->img->y += ghost->speed;
		if(ghost->img->y >= 768-28)
			ghost->img->y = 768-28;
		break;
	case 1:
		ghost->img->x += 2;
		if(ghost->img->x >= 1024-28)
			ghost->img->x = 1024-28;
		break;
	case 2:
		ghost->img->y -= 2;
		if(ghost->img->y <= 0)
			ghost->img->y = 0;
		break;
	case 3:
		ghost->img->x -= 2;
		if(ghost->img->x <= 0)
			ghost->img->x = 0;
		break;
	}
}

void move_ghost_user_esc(Ghost * ghost)
{
	ghost->detouring = 0;

	ghost_try_rotate(ghost);

	if (1 == ghost_check_surroundings(ghost))
		return;

	switch(ghost->direction)
	{
	case 0:
		ghost->img->y += ghost->speed;
		if(ghost->img->y >= 768-28)
			ghost->img->y = 768-28;
		break;
	case 1:
		ghost->img->x += 2;
		if(ghost->img->x >= 1024-28)
			ghost->img->x = 1024-28;
		break;
	case 2:
		ghost->img->y -= 2;
		if(ghost->img->y <= 0)
			ghost->img->y = 0;
		break;
	case 3:
		ghost->img->x -= 2;
		if(ghost->img->x <= 0)
			ghost->img->x = 0;
		break;
	}
}

void ghost_try_rotate(Ghost * ghost)
{
	int prev_dir = ghost->direction;
	ghost->direction = ghost->desired_direction;

	if(ghost_check_surroundings(ghost) == 1)
	{
		ghost->direction = prev_dir;
		ghost_rotate(ghost, prev_dir);
		return;
	}
	ghost->direction = prev_dir;
	ghost_rotate(ghost, ghost->desired_direction);
}

void move_ghost_escape(Ghost * ghost, Pacman * pacman)
{
	//ghost_escape_tick(ghost);

	int pac_dir = get_pacman_dir(ghost, pacman);
	int old_dir = ghost->direction;
	int new_dir = old_dir;

	if(pac_dir == -1)
	{
		printf("PACMAN DIRECTION FAILED!\n");
		return;
	}

	int sides[] = {0, 0, 0, 0};
	check_all_surroundings(ghost->img->x, ghost->img->y, ghost->img->width, ghost->img->height, sides);

	if((sides[(ghost->direction + 1) % 4] == 1) && (sides[(ghost->direction+3) % 4] == 1) && (ghost_check_surroundings(ghost) == 1))
		new_dir = (old_dir + 2) % 4;
	else if((sides[(ghost->direction + 1) % 4] == 1) && (sides[(ghost->direction+3) % 4] == 1) && (ghost_check_surroundings(ghost) == 0))
		new_dir = old_dir;
	else
	{

		if(pac_dir == (int) PACDIR_S)
		{
			new_dir = (int) UP;
			ghost->direction = (int) UP;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) UP)
				ghost->detouring = 0;
		}
		else if(pac_dir == (int) PACDIR_N)
		{
			new_dir = (int) DOWN;
			ghost->direction = (int) DOWN;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) DOWN)
				ghost->detouring = 0;
		}
		else if(pac_dir == (int) PACDIR_E)
		{
			new_dir = (int) LEFT;
			ghost->direction = (int) LEFT;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) LEFT)
				ghost->detouring = 0;
		}
		else if(pac_dir == (int) PACDIR_W)
		{
			new_dir = (int) RIGHT;
			ghost->direction = (int) RIGHT;

			while(ghost_check_surroundings(ghost) == 1)
			{
				ghost->detouring = 1;

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

			if(new_dir == (int) RIGHT)
				ghost->detouring = 0;
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
				ghost->detouring = 1;

				if(probability(50))
				{
					new_dir = (int) UP;
					ghost->direction = (int) UP;
				}
				else
				{
					new_dir = (int) RIGHT;
					ghost->direction = (int) RIGHT;
				}
			}

			if((new_dir == (int) LEFT) || (new_dir == (int) DOWN))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) RIGHT) || (new_dir == (int) DOWN))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) LEFT) || (new_dir == (int) DOWN))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) LEFT) || (new_dir == (int) UP))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) DOWN) || (new_dir == (int) RIGHT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) UP) || (new_dir == (int) RIGHT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) UP) || (new_dir == (int) RIGHT))
				ghost->detouring = 0;
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
				ghost->detouring = 1;

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

			if((new_dir == (int) UP) || (new_dir == (int) LEFT))
				ghost->detouring = 0;
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

int is_in_ghost(Ghost * ghost, int x_click, int y_click)
{
	int x = ghost->img->x;
	int y = ghost->img->y;
	int delta = ghost->img->width;

	if((x_click >= x) && (x_click < (x+delta)))
	{
		if((y_click >= y) && (y_click < (y+delta)))
		{
			return 1;
		}
	}

	return 0;
}

void switch_ghosts_to_auto(Ghost *ghosts[], int exception)
{
	int i = 0;
	for(;i<4;i++)
	{
		if(i != exception)
		{
			if(ghosts[i]->mode == 2)
			{
				ghosts[i]->mode = ghosts[i]->prev_mode;
			}
			else if (ghosts[i]->mode == 5)
			{
				ghosts[i]->mode = 3;
			}
			else if (ghosts[i]->mode != 3)
			{
				ghosts[i]->mode = ghosts[i]->prev_mode;
			}
		}
	}
}

void switch_ghosts_to_mode(Ghost *ghosts[], int mode)
{
	int i = 0;
	for(;i<4;i++)
		ghosts[i]->mode = mode;
}

void ghost_change_desired_direction(Ghost *ghost, unsigned long scan_code)
{
	switch(scan_code)
	{
	case W_KEY:
		ghost->desired_direction = UP;
		return;
	case A_KEY:
		ghost->desired_direction = LEFT;
		return;
	case S_KEY:
		ghost->desired_direction = DOWN;
		return;
	case D_KEY:
		ghost->desired_direction = RIGHT;
		return;
	}
}

void check_user_ghosts(Ghost *ghosts[], unsigned long scan_code)
{
	int i = 0;

	for(; i < 4; i++)
	{
		if((ghosts[i]->mode == 2) || (ghosts[i]->mode == 5))
		{
			ghost_change_desired_direction(ghosts[i], scan_code);
			return;
		}
	}
}

void all_ghosts_escape(Ghost * ghosts[], int time)
{
	int i = 0;
	for(; i < 4; i++)
	{
		if(ghosts[i]->mode != 2)
		{
			ghosts[i]->mode = 3;
			ghosts[i]->escape_counter = time;
		}
		else
		{
			ghosts[i]->mode = 5;
			ghosts[i]->escape_counter = time;
		}
	}
}

void ghost_escape_tick(Ghost * ghost)
{
	if(tick_counter == 0)
	{
		if(ghost->escape_counter > 0)
		{
			ghost->escape_counter = ghost->escape_counter - 1;
			return;
		}
		else
		{
			if(ghost->mode == 3)
				ghost->mode = ghost->prev_mode;
			else if(ghost->mode == 5)
				ghost->mode = 2;
			ghost->escape_counter = 0;
		}
	}
}

void all_ghosts_escape_tick(Ghost * ghosts[])
{
	if(tick_counter == 0)
	{
		int i = 0;
		for(;i<4;i++)
		{
			if(ghosts[i]->escape_counter > 0)
				ghosts[i]->escape_counter = ghosts[i]->escape_counter - 1;
			else
			{
				if(ghosts[i]->mode == 3)
				{
					ghosts[i]->mode = ghosts[i]->prev_mode;
					ghosts[i]->escape_counter = 0;
				}
				if(ghosts[i]->mode == 5)
				{
					ghosts[i]->mode = 2;
					ghosts[i]->escape_counter = 0;
				}
			}
		}
	}
}

void all_ghosts_spawn_timer(Ghost * ghosts[])
{
	if(tick_counter == 0)
	{
		int i = 0;
		for(;i<4;i++)
		{
			if(ghosts[i]->spawn_timer > 0)
			{
				ghosts[i]->spawn_timer = ghosts[i]->spawn_timer - 1;
			}
			else
				ghosts[i]->spawn_timer = 0;
		}
	}
	int i = 0, temp1, temp2;
	for(;i<4;i++)
	{
		if(ghosts[i]->spawn_timer > 0)
		{
			ghosts[i]->img->map = (char *)read_xpm(empty_xpm, &temp1, &temp2);
		}
	}
}

void reset_ghost(Ghost * ghost)
{
	ghost->img->x = 374;
	ghost->img->y = 270;
	switch(ghost->color)
	{
	case 0:
		ghost->spawn_timer = 8;
		break;
	case 1:
		ghost->spawn_timer = 2;
		break;
	case 2:
		ghost->spawn_timer = 6;
		break;
	case 3:
		ghost->spawn_timer = 3;
		break;
	}
}

void reset_all_ghosts(Ghost * ghosts[])
{
	int i = 0;
	for(;i<4;i++)
	{
		reset_ghost(ghosts[i]);
	}
}

void ghost_eaten(Ghost * ghost)
{
	ghost->img->x = 374;
	ghost->img->y = 270;
	ghost->spawn_timer = 2;
	if(ghost->mode == 3)
		ghost->mode = ghost->prev_mode;
	else if (ghost->mode == 5)
		ghost->mode = 2;
	ghost->escape_counter = 0;
}



int probability(int percentage)
{
	int result = rand_integer_between(0, 100);
	if(result < percentage)
		return 1;
	return 0;
}

void draw_lives(int lives, int xi, int yi)
{
	Sprite *logo;
	logo = (Sprite *)malloc(sizeof(Sprite));
	logo->map = (char *)read_xpm(pacman_r3_xpm, &logo->width, &logo->height);

	logo->x = xi+80;
	logo->y = yi;

	int i = 0;
	for(;i<lives;i++)
	{
		draw_img(logo);
		logo->x-=40;
	}

	free(logo);
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

void check_for_click(Ghost *ghosts[], Mouse_coord *mouse)
{
	if(mouse->lb == 0)
		return;

	int i = 0;

	for(; i < 4; i++)
	{
		if(is_in_ghost(ghosts[i], mouse->x_coord, mouse->y_coord))
		{
			if(ghosts[i]->mode == 3)
				ghosts[i]->mode = 5;
			else
				ghosts[i]->mode = 2;

			switch_ghosts_to_auto(ghosts, i);
			return;
		}
	}

	switch_ghosts_to_auto(ghosts, 5);
}

double get_dist(Sprite *sp1, Sprite *sp2)
{
	double dist = 0;
	dist = sqrt(pow((sp1->x + sp1->width/2) - (sp2->x + sp2->width/2), 2) + pow((sp1->y + sp1->height/2) - (sp2->y + sp2->height/2), 2));
	return dist;
}

int check_collisions(Ghost *ghosts[], Pacman * pacman)
{
	int i = 0;
	for(; i < 4; i++)
	{
		double dist = get_dist(ghosts[i]->img, pacman->img->sp);
		if((dist < (ghosts[i]->img->width - COLLISION_TOLERANCE)) && (ghosts[i]->spawn_timer == 0))
			return i;
	}
	return -1;
}





