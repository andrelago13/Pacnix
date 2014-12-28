#include "pac_menu.h"
#include "pacnix.h"
#include "video_gr.h"
#include "kbd_header.h"
#include "pixmaps.h"
#include "maze.h"

static int selection;
static Sprite *singleplayer;
static Sprite *multiplayer;
static Sprite *instructions;
static Sprite *about;
static Sprite *exit;

static Sprite *letter_p;
static Sprite *letter_a;
static Sprite *letter_c;
static Sprite *letter_n;
static Sprite *letter_i;
static Sprite *letter_x;

static Sprite *ghost_red;
static Sprite *ghost_blue;
static Sprite *ghost_pink;
static Sprite *ghost_orange;
static Sprite *pacman;
static Sprite *dot;

static Sprite *inst_sing;
static Sprite *inst_sing_cont;
static Sprite *inst_sing_obj;
static Sprite *inst_multip;
static Sprite *inst_multip_cont;
static Sprite *inst_multip_obj;
static Sprite *inst_objective;

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
	exit->map = (char *)read_xpm(exit_xpm, &exit->width, &exit->height);
	exit->x = 312;
	exit->y = 685;

	letter_p = (Sprite *)malloc(sizeof(Sprite));
	letter_p->map = (char *)read_xpm(letter_p_xpm, &letter_p->width, &letter_p->height);
	letter_p->x = 84;
	letter_p->y = 30;

	letter_a = (Sprite *)malloc(sizeof(Sprite));
	letter_a->map = (char *)read_xpm(letter_a_xpm, &letter_a->width, &letter_a->height);
	letter_a->x = 229;
	letter_a->y = 30;

	letter_c = (Sprite *)malloc(sizeof(Sprite));
	letter_c->map = (char *)read_xpm(letter_c_xpm, &letter_c->width, &letter_c->height);
	letter_c->x = 374;
	letter_c->y = 14;

	letter_n = (Sprite *)malloc(sizeof(Sprite));
	letter_n->map = (char *)read_xpm(letter_n_xpm, &letter_n->width, &letter_n->height);
	letter_n->x = 519;
	letter_n->y = 30;

	letter_i = (Sprite *)malloc(sizeof(Sprite));
	letter_i->map = (char *)read_xpm(letter_i_xpm, &letter_i->width, &letter_i->height);
	letter_i->x = 664;
	letter_i->y = 15;

	letter_x = (Sprite *)malloc(sizeof(Sprite));
	letter_x->map = (char *)read_xpm(letter_x_xpm, &letter_x->width, &letter_x->height);
	letter_x->x = 809;
	letter_x->y = 38;

	ghost_red = (Sprite *)malloc(sizeof(Sprite));
	ghost_red->map = (char *)read_xpm(ghost_red_right, &ghost_red->width, &ghost_red->height);
	ghost_red->x = 260;
	ghost_red->y = 310;

	ghost_blue = (Sprite *)malloc(sizeof(Sprite));
	ghost_blue->map = (char *)read_xpm(ghost_blue_right, &ghost_blue->width, &ghost_blue->height);
	ghost_blue->x = 300;
	ghost_blue->y = 310;

	ghost_pink = (Sprite *)malloc(sizeof(Sprite));
	ghost_pink->map = (char *)read_xpm(ghost_pink_right, &ghost_pink->width, &ghost_pink->height);
	ghost_pink->x = 340;
	ghost_pink->y = 310;

	ghost_orange = (Sprite *)malloc(sizeof(Sprite));
	ghost_orange->map = (char *)read_xpm(ghost_orange_right, &ghost_orange->width, &ghost_orange->height);
	ghost_orange->x = 380;
	ghost_orange->y = 310;

	pacman = (Sprite *)malloc(sizeof(Sprite));
	pacman->map = (char *)read_xpm(pacman_r3_xpm, &pacman->width, &pacman->height);
	pacman->x = 580;
	pacman->y = 310;

	dot = (Sprite *)malloc(sizeof(Sprite));
	dot->map = (char *)read_xpm(menu_dot_xpm, &dot->width, &dot->height);
	dot->x = 730;
	dot->y = 310;

	inst_sing = (Sprite *)malloc(sizeof(Sprite));
	inst_sing->map = (char *)read_xpm(sing_xpm, &inst_sing->width, &inst_sing->height);
	inst_sing->x = 26;
	inst_sing->y = 50;

	inst_sing_cont = (Sprite *)malloc(sizeof(Sprite));
	inst_sing_cont->map = (char *)read_xpm(sing_cont_xpm, &inst_sing_cont->width, &inst_sing_cont->height);
	inst_sing_cont->x = 56;
	inst_sing_cont->y = 108;

	inst_sing_obj = (Sprite *)malloc(sizeof(Sprite));
	inst_sing_obj->map = (char *)read_xpm(sing_obj_xpm, &inst_sing_obj->width, &inst_sing_obj->height);
	inst_sing_obj->x = 590;
	inst_sing_obj->y = 170;

	inst_multip = (Sprite *)malloc(sizeof(Sprite));
	inst_multip->map = (char *)read_xpm(multip_xpm, &inst_multip->width, &inst_multip->height);
	inst_multip->x = 26;
	inst_multip->y = 322;

	inst_multip_cont = (Sprite *)malloc(sizeof(Sprite));
	inst_multip_cont->map = (char *)read_xpm(multip_cont_xpm, &inst_multip_cont->width, &inst_multip_cont->height);
	inst_multip_cont->x = 56;
	inst_multip_cont->y = 400;

	inst_multip_obj = (Sprite *)malloc(sizeof(Sprite));
	inst_multip_obj->map = (char *)read_xpm(multip_obj_xpm, &inst_multip_obj->width, &inst_multip_obj->height);
	inst_multip_obj->x = 590;
	inst_multip_obj->y = 400;

	inst_objective = (Sprite *)malloc(sizeof(Sprite));
	inst_objective->map = (char *)read_xpm(objective_xpm, &inst_objective->width, &inst_objective->height);
	inst_objective->x = 490;
	inst_objective->y = 120;
}

void clear_menu_pieces()
{
	free(singleplayer->map);
	free(singleplayer);
	free(multiplayer->map);
	free(multiplayer);
	free(instructions->map);
	free(instructions);
	free(about->map);
	free(about);
	free(exit->map);
	free(exit);
	free(ghost_red->map);
	free(ghost_red);
	free(ghost_pink->map);
	free(ghost_pink);
	free(ghost_blue->map);
	free(ghost_blue);
	free(ghost_orange->map);
	free(ghost_orange);
	free(pacman->map);
	free(pacman);
	free(dot->map);
	free(dot);
	free(letter_p->map);
	free(letter_p);
	free(letter_a->map);
	free(letter_a);
	free(letter_c->map);
	free(letter_c);
	free(letter_n->map);
	free(letter_n);
	free(letter_i->map);
	free(letter_i);
	free(letter_x->map);
	free(letter_x);

	free(inst_sing);
	free(inst_sing_cont);
	free(inst_sing_obj);
	free(inst_multip);
	free(inst_multip_cont);
	free(inst_multip_obj);
	free(inst_objective);
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

	///////// PACNIX ///////////////
	draw_img(letter_p);
	draw_img(letter_a);
	draw_img(letter_c);
	draw_img(letter_n);
	draw_img(letter_i);
	draw_img(letter_x);

	///////// GHOSTS AND PACMAN ////
	draw_img(ghost_red);
	draw_img(ghost_orange);
	draw_img(ghost_blue);
	draw_img(ghost_pink);
	draw_img(pacman);
	draw_img(dot);

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

void draw_inst_menu()
{
	draw_img(inst_sing);
	draw_img(inst_sing_cont);
	draw_img(inst_sing_obj);
	draw_img(inst_multip);
	draw_img(inst_multip_cont);
	draw_img(inst_multip_obj);
	draw_img(inst_objective);

	int tempy;
	tempy = inst_objective->y;
	inst_objective->y = 354;
	draw_img(inst_objective);
	inst_objective->y = tempy;

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
