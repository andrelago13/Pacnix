#include "sprite.h"

Sprite * create_sprite(char *pic[], int x_ini, int y_ini)
{
	Sprite *sp;
	sp = (Sprite *)malloc(sizeof(Sprite));

	sp->x = x_ini;
	sp->y = y_ini;
	sp->map = (char *) read_xpm(pic, &sp->width, &sp->height);

	return sp;
}

void destroy_sprite(Sprite * sp)
{
	free(sp->map);
	free(sp);
}
