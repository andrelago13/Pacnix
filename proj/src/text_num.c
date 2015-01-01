#include "text_num.h"

void draw_num(int number, int x, int y, unsigned long color)
{
	Sprite *temp;
	temp = (Sprite *)malloc(sizeof(Sprite));
	temp->y = y;
	temp->x = x;

	if(number == 0)
	{
		temp->map = (char *)read_xpm(zero_xpm, &temp->width, &temp->height);
		temp->x -= (temp->width + 3);
		print_num(temp, color);
		return;
	}
	else
	{
		while(number > 0)
		{
			int digit = number % 10;
			number = number / 10;
			digit_map(digit, temp);

			temp->x -= (temp->width + 3);
			print_num(temp, color);
		}
	}
}

void digit_map(int digit, Sprite * sp)
{
	switch(digit)
	{
	case 0:
		sp->map = (char *)read_xpm(zero_xpm, &sp->width, &sp->height);
		return;
	case 1:
		sp->map = (char *)read_xpm(um_xpm, &sp->width, &sp->height);
		return;
	case 2:
		sp->map = (char *)read_xpm(dois_xpm, &sp->width, &sp->height);
		return;
	case 3:
		sp->map = (char *)read_xpm(tres_xpm, &sp->width, &sp->height);
		return;
	case 4:
		sp->map = (char *)read_xpm(quatro_xpm, &sp->width, &sp->height);
		return;
	case 5:
		sp->map = (char *)read_xpm(cinco_xpm, &sp->width, &sp->height);
		return;
	case 6:
		sp->map = (char *)read_xpm(seis_xpm, &sp->width, &sp->height);
		return;
	case 7:
		sp->map = (char *)read_xpm(sete_xpm, &sp->width, &sp->height);
		return;
	case 8:
		sp->map = (char *)read_xpm(oito_xpm, &sp->width, &sp->height);
		return;
	case 9:
		sp->map = (char *)read_xpm(nove_xpm, &sp->width, &sp->height);
		return;
	}
}

void draw_score_header(int x, int y)
{
	Sprite *sp;
	sp = (Sprite *)malloc(sizeof(Sprite));
	sp->x = x; sp->y = y;
	sp->map = (char *)read_xpm(score_xpm, &sp->width, &sp->height);
	draw_img(sp);
	destroy_sprite(sp);
}

void draw_highscore_header(int x, int y)
{
	Sprite *sp;
	sp = (Sprite *)malloc(sizeof(Sprite));
	sp->x = x; sp->y = y;
	sp->map = (char *)read_xpm(highscore_xpm, &sp->width, &sp->height);
	draw_img(sp);
	destroy_sprite(sp);
}
