#include "pac_menu.h"
#include "pacnix.h"
#include "video_gr.h"
#include "kbd_header.h"
#include "pixmaps.h"
#include "maze.h"

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

	Sprite sp;

	sp.x = 312;
	sp.y = 430;
	sp.map = (char *)read_xpm(singleplayer_xpm, &sp.width, &sp.height);
	draw_img(&sp);
	if(selection == 0)
		surround_img(&sp, 63);

	free(sp.map);
	sp.x = 312;
	sp.y = 500;
	sp.map = (char *)read_xpm(multiplayer_xpm, &sp.width, &sp.height);
	draw_img(&sp);
	if(selection == 1)
		surround_img(&sp, 63);

	free(sp.map);
	sp.x = 312;
	sp.y = 568;
	sp.map = (char *)read_xpm(instructions_xpm, &sp.width, &sp.height);
	draw_img(&sp);
	if(selection == 2)
		surround_img(&sp, 63);

	free(sp.map);
	sp.x = 312;
	sp.y = 626;
	sp.map = (char *)read_xpm(about_xpm, &sp.width, &sp.height);
	draw_img(&sp);
	if(selection == 3)
		surround_img(&sp, 63);

	free(sp.map);
	sp.x = 312;
	sp.y = 685;
	sp.map = (char *)read_xpm(exit_xpm, &sp.width, &sp.height);
	draw_img(&sp);
	if(selection == 4)
		surround_img(&sp, 63);

	free(sp.map);
	sp.x = 84;
	sp.y = 30;
	sp.map = (char *)read_xpm(letter_p_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 229;
	sp.y = 30;
	sp.map = (char *)read_xpm(letter_a_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 374;
	sp.y = 14;
	sp.map = (char *)read_xpm(letter_c_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 519;
	sp.y = 30;
	sp.map = (char *)read_xpm(letter_n_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 664;
	sp.y = 15;
	sp.map = (char *)read_xpm(letter_i_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 809;
	sp.y = 38;
	sp.map = (char *)read_xpm(letter_x_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 260;
	sp.y = 310;
	sp.map = (char *)read_xpm(ghost_red_right, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 300;
	sp.y = 310;
	sp.map = (char *)read_xpm(ghost_blue_right, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 340;
	sp.y = 310;
	sp.map = (char *)read_xpm(ghost_pink_right, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 380;
	sp.y = 310;
	sp.map = (char *)read_xpm(ghost_orange_right, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 580;
	sp.y = 310;
	sp.map = (char *)read_xpm(pacman_r3_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 730;
	sp.y = 310;
	sp.map = (char *)read_xpm(menu_dot_xpm, &sp.width, &sp.height);
	draw_img(&sp);
	free(sp.map);
}

void draw_inst_menu()
{
	Sprite sp;

	sp.x = 26;
	sp.y = 50;
	sp.map = (char *)read_xpm(sing_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 56;
	sp.y = 108;
	sp.map = (char *)read_xpm(sing_cont_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	sp.y = 400;
	draw_img(&sp);

	free(sp.map);
	sp.x = 590;
	sp.y = 170;
	sp.map = (char *)read_xpm(sing_obj_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 26;
	sp.y = 322;
	sp.map = (char *)read_xpm(multip_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 590;
	sp.y = 400;
	sp.map = (char *)read_xpm(multip_obj_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 56;
	sp.y = 570;
	sp.map = (char *)read_xpm(multip_cont_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	free(sp.map);
	sp.x = 490;
	sp.y = 120;
	sp.map = (char *)read_xpm(objective_xpm, &sp.width, &sp.height);
	draw_img(&sp);

	sp.y = 354;
	draw_img(&sp);
	free(sp.map);
}

void draw_about_menu()
{
//	Sprite sp;
//	sp.map = (char *)read_xpm(about_menu_xpm, &sp.width, &sp.height);
//	sp.x = 0;
//	sp.y = 0;
//	draw_img(&sp);
//	free(sp.map);
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
