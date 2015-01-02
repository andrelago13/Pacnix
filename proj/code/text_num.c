#include "text_num.h"

int draw_num(int number, int x, int y, unsigned long color)
{
	Sprite *temp;
	temp = (Sprite *)malloc(sizeof(Sprite));
	temp->y = y;
	temp->x = x;

	if(number == 0)
	{
		temp->map = (char *)read_xpm(zero_xpm, &temp->width, &temp->height);
		temp->x -= (temp->width + DIGIT_SPACING);
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

			temp->x -= (temp->width + DIGIT_SPACING);
			print_num(temp, color);
		}
	}

	int ret = temp->x;
	destroy_sprite(temp);
	return ret;
}

int draw_num_width(int number, int x, int y, unsigned long color, int width)
{
	Sprite *temp;
	temp = (Sprite *)malloc(sizeof(Sprite));
	temp->y = y;
	temp->x = x;

	while(width > 0)
	{
		int digit = number % 10;
		number = number / 10;
		digit_map(digit, temp);

		temp->x -= (temp->width + DIGIT_SPACING);
		print_num(temp, color);
		width--;

		if(width > 0)
			free(temp->map);
	}

	int ret = temp->x;
	destroy_sprite(temp);
	return ret;

}

int draw_time_width(int number, int x, int y, unsigned long color, int width)
{
	Sprite *temp;
	temp = (Sprite *)malloc(sizeof(Sprite));
	temp->y = y;
	temp->x = x;

	while(width > 0)
	{
		int digit = number % 10;
		number = number / 10;
		digit_map(digit, temp);

		temp->x -= (temp->width + DIGIT_SPACING);
		print_num(temp, color);
		temp->x -= (MAX_DIGIT_WIDTH-temp->width);
		width--;

		if(width > 0)
			free(temp->map);
	}

	int ret = temp->x;
	destroy_sprite(temp);
	return ret;
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

void draw_date(Date * date, int x, int y)
{
	int curr_x = x;
	int curr_y = y;

	curr_x = draw_num_width(date->year + 2000, curr_x, curr_y, 63, 4);

	curr_x -= DIGIT_SPACING;

	Sprite temp;
	temp.map = (char *)read_xpm(bar_xpm, &temp.width, &temp.height);
	curr_x -= temp.width;
	temp.x = curr_x;
	temp.y = y;
	draw_img(&temp);

	curr_x -= DIGIT_SPACING;

	curr_x = draw_num_width(date->month, curr_x, curr_y, 63, 2);

	curr_x -= (DIGIT_SPACING + temp.width);

	temp.x = curr_x;
	draw_img(&temp);

	draw_num_width(date->day, curr_x, curr_y, 63, 2);

	curr_x = x;
	curr_y += temp.height + DIGIT_SPACING;

	temp.y = curr_y;

	draw_time_width(date->seconds, curr_x, curr_y, 63, 2);
	curr_x = x - SECONDS_MINUTES_WIDTH;


	free(temp.map);
	temp.map = (char *)read_xpm(dots_xpm, &temp.width, &temp.height);
	temp.x = curr_x - (temp.width + DIGIT_SPACING);
	curr_x = temp.x;
	draw_img(&temp);

	curr_x -= DIGIT_SPACING;

	draw_time_width(date->minutes, curr_x, curr_y, 63, 2);

	curr_x -= SECONDS_MINUTES_WIDTH;

	curr_x -= (DIGIT_SPACING + temp.width);

	temp.x = curr_x;
	draw_img(&temp);

	curr_x -= DIGIT_SPACING;

	draw_time_width(date->hours, curr_x, curr_y, 63, 2);
}
