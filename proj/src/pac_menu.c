#include "pac_menu.h"
#include "pacnix.h"
#include "video_gr.h"
#include "kbd_header.h"

static int selection;
static Sprite *singleplayer;
static Sprite *multiplayer;
static Sprite *instructions;
static Sprite *about;
static Sprite *exit;

void initialize_menu_pieces()
{
	singleplayer = (Sprite *)malloc(sizeof(Sprite));
	singleplayer->map = (char *)read_xpm(singleplayer_xpm, &singleplayer->width, &singleplayer->height);
	singleplayer->x = 312;
	singleplayer->y = 430;

	multiplayer = (Sprite *)malloc(sizeof(Sprite));
	multiplayer->map = (char *)read_xpm(multiplayer_xpm, &multiplayer->width, &multiplayer->height);
	multiplayer->x = 312;
	multiplayer->y = 500;

	instructions = (Sprite *)malloc(sizeof(Sprite));
	instructions->map = (char *)read_xpm(instructions_xpm, &instructions->width, &instructions->height);
	instructions->x = 312;
	instructions->y = 568;

	about = (Sprite *)malloc(sizeof(Sprite));
	about->map = (char *)read_xpm(about_xpm, &about->width, &about->height);
	about->x = 312;
	about->y = 626;

	exit = (Sprite *)malloc(sizeof(Sprite));
	exit->map = (char *)read_xpm(singleplayer_xpm, &exit->width, &exit->height);
	exit->x = 312;
	exit->y = 685;
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

	///////// SINGLEPLAYER /////////
	draw_img(singleplayer);
	if(selection == 0)
		surround_img(singleplayer, 63);


	///////// MULTIPLAYER /////////
	draw_img(multiplayer);
	if(selection == 1)
		surround_img(multiplayer, 63);


	///////// INSTRUCTIONS /////////
	draw_img(instructions);
	if(selection == 2)
		surround_img(instructions, 63);


	///////// ABOUT /////////
	draw_img(about);
	if(selection == 3)
		surround_img(about, 63);


	///////// EXIT /////////
	draw_img(exit);
	if(selection == 4)
		surround_img(exit, 63);
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
		}
		else if(selection == -1)
			selection = 4;
		return -1;
	case (int) DOWN_ARROW:
		if((selection >= 0) && (selection < 4))
		{
			selection++;
		}
		else if(selection == -1)
			selection = 0;
		return -1;
	case (int) ENTER_KEY:
		return selection;
	}
	return -1;
}
