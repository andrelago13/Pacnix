#include "pac_menu.h"
#include "pacnix.h"
#include "video_gr.h"
#include "kbd_header.h"

static int selection;

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

	printf("SELECTION : %d\n", selection);

	Sprite * sp;
	sp = (Sprite *)malloc(sizeof(Sprite));

	///////// SINGLEPLAYER /////////
	sp->map = (char *)read_xpm(singleplayer_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 430;
	draw_img(sp);
	if(selection == 0)
		surround_img(sp, 63);


	///////// MULTIPLAYER /////////
	sp->map = (char *)read_xpm(multiplayer_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 500;
	draw_img(sp);
	if(selection == 1)
		surround_img(sp, 63);


	///////// INSTRUCTIONS /////////
	sp->map = (char *)read_xpm(instructions_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 568;
	draw_img(sp);
	if(selection == 2)
		surround_img(sp, 63);


	///////// ABOUT /////////
	sp->map = (char *)read_xpm(about_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 626;
	draw_img(sp);
	if(selection == 3)
		surround_img(sp, 63);


	///////// EXIT /////////
	sp->map = (char *)read_xpm(singleplayer_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 685;
	draw_img(sp);
	if(selection == 4)
		surround_img(sp, 63);

	free(sp);
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
