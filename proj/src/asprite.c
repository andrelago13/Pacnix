#include "asprite.h"
#include "read_xpm.h"

AnimSprite * create_asprite(char **maps, int num_map, int speed, int x_ini, int y_ini, char *map_ini[])
{
	AnimSprite *anim;
	anim = (AnimSprite *)malloc(sizeof(AnimSprite));
	anim->aspeed = speed;
	anim->cur_aspeed = 0;
	anim->num_fig = num_map;
	anim->cur_fig = 0;

	anim->map = maps;

	Sprite sprite;
	sprite.x = x_ini;
	sprite.y = y_ini;
	sprite.map = read_xpm(map_ini, &sprite.width, &sprite.height);

	anim->sp = (Sprite *)malloc(sizeof(Sprite));
	*anim->sp = sprite;
	return anim;
}

int animate_asprite(AnimSprite *fig)
{
	fig->cur_aspeed++;

	if(fig->cur_aspeed == fig->aspeed)
	{
		if(fig->cur_fig == fig->num_fig - 1)
			fig->cur_fig = 0;
		else
			fig->cur_fig++;

		fig->sp->map = fig->map[fig->cur_fig];

		fig->cur_aspeed = 0;

		return 0;
	}

	return 1;
}
