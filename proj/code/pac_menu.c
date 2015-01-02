#include "pac_menu.h"
#include "pacnix.h"
#include "video_gr.h"
#include "kbd_header.h"
#include "pixmaps.h"
#include "maze.h"

static int selection;

static Sprite *singleplayer_img;
static Sprite *multiplayer_img;
static Sprite *instructions_img;
static Sprite *about_img;
static Sprite *exit_img;

static Sprite *letter_p_img;
static Sprite *letter_a_img;
static Sprite *letter_c_img;
static Sprite *letter_n_img;
static Sprite *letter_i_img;
static Sprite *letter_x_img;

static Sprite *pacman_img;
static Sprite *dot_img;
static Sprite *ghost_red_img;
static Sprite *ghost_blue_img;
static Sprite *ghost_pink_img;
static Sprite *ghost_orange_img;

static Sprite *instructions_menu_img;
static Sprite *about_menu_img;

void initialize_menu_pieces()
{
	selection = -1;

	FILE * pixmap_file;
	pixmap_file = (FILE *)fopen(MENU_IMG_FILE, "r");
	if(pixmap_file == NULL)
		return;

	singleplayer_img = (Sprite *)malloc(sizeof(Sprite));
	singleplayer_img->x = 312;
	singleplayer_img->y = 430;
	read_file_xpm(pixmap_file, singleplayer_img);

	multiplayer_img = (Sprite *)malloc(sizeof(Sprite));
	multiplayer_img->x = 312;
	multiplayer_img->y = 500;
	read_file_xpm(pixmap_file, multiplayer_img);

	instructions_img = (Sprite *)malloc(sizeof(Sprite));
	instructions_img->x = 312;
	instructions_img->y = 568;
	read_file_xpm(pixmap_file, instructions_img);

	about_img = (Sprite *)malloc(sizeof(Sprite));
	about_img->x = 312;
	about_img->y = 626;
	read_file_xpm(pixmap_file, about_img);

	exit_img = (Sprite *)malloc(sizeof(Sprite));
	exit_img->x = 312;
	exit_img->y = 685;
	read_file_xpm(pixmap_file, exit_img);

	letter_p_img = (Sprite *)malloc(sizeof(Sprite));
	letter_p_img->x = 84;
	letter_p_img->y = 30;
	read_file_xpm(pixmap_file, letter_p_img);

	letter_a_img = (Sprite *)malloc(sizeof(Sprite));
	letter_a_img->x = 229;
	letter_a_img->y = 30;
	read_file_xpm(pixmap_file, letter_a_img);

	letter_c_img = (Sprite *)malloc(sizeof(Sprite));
	letter_c_img->x = 374;
	letter_c_img->y = 14;
	read_file_xpm(pixmap_file, letter_c_img);

	letter_n_img = (Sprite *)malloc(sizeof(Sprite));
	letter_n_img->x = 519;
	letter_n_img->y = 30;
	read_file_xpm(pixmap_file, letter_n_img);

	letter_i_img = (Sprite *)malloc(sizeof(Sprite));
	letter_i_img->x = 664;
	letter_i_img->y = 15;
	read_file_xpm(pixmap_file, letter_i_img);

	letter_x_img = (Sprite *)malloc(sizeof(Sprite));
	letter_x_img->x = 809;
	letter_x_img->y = 38;
	read_file_xpm(pixmap_file, letter_x_img);

	ghost_red_img = (Sprite *)malloc(sizeof(Sprite));
	ghost_red_img->x = 260;
	ghost_red_img->y = 310;
	ghost_red_img->map = (char *)read_xpm(ghost_red_right, &ghost_red_img->width, &ghost_red_img->height);

	ghost_blue_img = (Sprite *)malloc(sizeof(Sprite));
	ghost_blue_img->x = 300;
	ghost_blue_img->y = 310;
	ghost_blue_img->map = (char *)read_xpm(ghost_blue_right, &ghost_blue_img->width, &ghost_blue_img->height);

	ghost_orange_img = (Sprite *)malloc(sizeof(Sprite));
	ghost_orange_img->x = 380;
	ghost_orange_img->y = 310;
	ghost_orange_img->map = (char *)read_xpm(ghost_orange_right, &ghost_orange_img->width, &ghost_orange_img->height);

	ghost_pink_img = (Sprite *)malloc(sizeof(Sprite));
	ghost_pink_img->x = 340;
	ghost_pink_img->y = 310;
	ghost_pink_img->map = (char *)read_xpm(ghost_pink_right, &ghost_pink_img->width, &ghost_pink_img->height);

	pacman_img = (Sprite *)malloc(sizeof(Sprite));
	pacman_img->x = 580;
	pacman_img->y = 310;
	pacman_img->map = (char *)read_xpm(pacman_r3_xpm, &pacman_img->width, &pacman_img->height);

	dot_img = (Sprite *)malloc(sizeof(Sprite));
	dot_img->x = 730;
	dot_img->y = 310;
	read_file_xpm(pixmap_file, dot_img);

	instructions_menu_img = (Sprite *)malloc(sizeof(Sprite));
	instructions_menu_img->x = 52;
	instructions_menu_img->y = 34;
	read_file_xpm(pixmap_file, instructions_menu_img);

	about_menu_img = (Sprite *)malloc(sizeof(Sprite));
	about_menu_img->x = 15;
	about_menu_img->y = 122;
	read_file_xpm(pixmap_file, about_menu_img);

	fclose(pixmap_file);
}

void clear_menu_pieces()
{
	destroy_sprite(singleplayer_img);
	destroy_sprite(multiplayer_img);
	destroy_sprite(instructions_img);
	destroy_sprite(about_img);
	destroy_sprite(exit_img);

	destroy_sprite(letter_p_img);
	destroy_sprite(letter_a_img);
	destroy_sprite(letter_c_img);
	destroy_sprite(letter_n_img);
	destroy_sprite(letter_i_img);
	destroy_sprite(letter_x_img);

	destroy_sprite(pacman_img);
	destroy_sprite(dot_img);
	destroy_sprite(ghost_red_img);
	destroy_sprite(ghost_blue_img);
	destroy_sprite(ghost_pink_img);
	destroy_sprite(ghost_orange_img);

	destroy_sprite(instructions_menu_img);
	destroy_sprite(about_menu_img);
}

void draw_main_menu(Mouse_coord * mouse)
{
	if((mouse->x_coord >= 312) && (mouse->x_coord < 711))
	{
		if((mouse->y_coord >= 430) && (mouse->y_coord < 487))
		{
			selection = 0;
		}else if((mouse->y_coord >= 500) && (mouse->y_coord < 555))
		{
			selection = 1;
		}else if((mouse->y_coord >= 568) && (mouse->y_coord < 613))
		{
			selection = 2;
		}else if((mouse->y_coord >= 626) && (mouse->y_coord < 672))
		{
			selection = 3;
		}else if((mouse->y_coord >= 685) && (mouse->y_coord < 731))
		{
			selection = 4;
		}
	}

	draw_img(singleplayer_img);
	if(selection == 0)
		surround_img(singleplayer_img, 63);

	draw_img(multiplayer_img);
	if(selection == 1)
		surround_img(multiplayer_img, 63);

	draw_img(instructions_img);
	if(selection == 2)
		surround_img(instructions_img, 63);

	draw_img(about_img);
	if(selection == 3)
		surround_img(about_img, 63);

	draw_img(exit_img);
	if(selection == 4)
		surround_img(exit_img, 63);

	draw_img(letter_p_img);
	draw_img(letter_a_img);
	draw_img(letter_c_img);
	draw_img(letter_n_img);
	draw_img(letter_i_img);
	draw_img(letter_x_img);

	draw_img(pacman_img);
	draw_img(dot_img);
	draw_img(ghost_red_img);
	draw_img(ghost_blue_img);
	draw_img(ghost_pink_img);
	draw_img(ghost_orange_img);
}

void draw_inst_menu()
{
	draw_img(instructions_menu_img);
}

void draw_about_menu()
{
	draw_img(about_menu_img);
}

int check_mainmenu_click(Mouse_coord * mouse)
{
	if(mouse->lb == 1)
	{
		if((mouse->x_coord >= 312) && (mouse->x_coord < 711))
		{
			if((mouse->y_coord >= 430) && (mouse->y_coord < 487))
			{
				return 0;
			}

			if((mouse->y_coord >= 500) && (mouse->y_coord < 555))
			{
				return 1;
			}

			if((mouse->y_coord >= 568) && (mouse->y_coord < 613))
			{
				return 2;
			}

			if((mouse->y_coord >= 626) && (mouse->y_coord < 672))
			{
				return 3;
			}

			if((mouse->y_coord >= 685) && (mouse->y_coord < 731))
			{
				return 4;
			}
		}
	}

	return -1;
}

int arrow_click(unsigned long key)
{
	switch(key)
	{
	case (int) UP_ARROW:
		if(selection > 0)
		{
			selection--;
			return -1;
		}
		else if(selection == -1)
		{
			selection = 4;
			return -1;
		}
		else if(selection == 0)
			selection = 4;
		return -1;
	case (int) DOWN_ARROW:
		if((selection >= 0) && (selection < 4))
		{
			selection++;
			return -1;
		}
		else if(selection == -1)
		{
			selection = 0;
			return -1;
		}
		else if(selection == 4)
			selection = 0;
		return -1;
	case (int) ENTER_KEY:
		return selection;
	}
	return -1;
}
