#include "pac_menu.h"
#include "pacnix.h"
#include "video_gr.h"

void draw_main_menu(Mouse_coord * mouse)
{
	Sprite * sp;
	sp = (Sprite *)malloc(sizeof(Sprite));

	///////// SINGLEPLAYER /////////
	sp->map = (char *)read_xpm(singleplayer_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 430;
	draw_img(sp);
	if(is_in_img(sp, mouse->x_coord, mouse->y_coord))
		surround_img(sp, 63);


	///////// MULTIPLAYER /////////
	sp->map = (char *)read_xpm(multiplayer_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 500;
	draw_img(sp);
	if(is_in_img(sp, mouse->x_coord, mouse->y_coord))
		surround_img(sp, 63);


	///////// INSTRUCTIONS /////////
	sp->map = (char *)read_xpm(instructions_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 568;
	draw_img(sp);
	if(is_in_img(sp, mouse->x_coord, mouse->y_coord))
		surround_img(sp, 63);


	///////// ABOUT /////////
	sp->map = (char *)read_xpm(about_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 626;
	draw_img(sp);
	if(is_in_img(sp, mouse->x_coord, mouse->y_coord))
		surround_img(sp, 63);


	///////// EXIT /////////
	sp->map = (char *)read_xpm(singleplayer_xpm, &sp->width, &sp->height);
	sp->x = 312;
	sp->y = 685;
	draw_img(sp);
	if(is_in_img(sp, mouse->x_coord, mouse->y_coord))
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
