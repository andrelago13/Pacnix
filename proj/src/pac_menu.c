#include "pac_menu.h"
#include "pacnix.h"
#include "pixmaps.h"
#include "video_gr.h"

void draw_main_menu(Mouse_coord * mouse)
{
	Sprite * sp;
	sp = (Sprite *)malloc(sizeof(Sprite));
	sp->map = (char *)read_xpm(pacman_r3_xpm, &sp->width, &sp->height);
	sp->x = 20;
	sp->y = 20;

	draw_img(sp);
	if(is_in_img(sp, mouse->x_coord, mouse->y_coord))
		surround_img(sp, 2);

	free(sp);
}

int check_mainmenu_click(Mouse_coord * mouse)
{
	if(mouse->lb == 1)
	{
		if((mouse->x_coord > 700))
			return 5;
	}

	return 0;
}
