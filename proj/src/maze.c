#include "maze.h"
#include "sprite.h"
#include "video_gr.h"

// ALL MAZE PIECES
static Sprite *blank_img;
static Sprite *tlc_img;
static Sprite *blc_img;
static Sprite *trc_img;
static Sprite *brc_img;
static Sprite *hw_img;
static Sprite *vw_img;
static Sprite *tlb_img;
static Sprite *blb_img;
static Sprite *trb_img;
static Sprite *brb_img;
static Sprite *teleporter_img;

void initialize_map_pieces()
{
	blank_img = (Sprite *)malloc(sizeof(Sprite));
	blank_img->map = (char *)read_xpm(blank_xpm, &blank_img->width, &blank_img->height);

	tlc_img = (Sprite *)malloc(sizeof(Sprite));
	tlc_img->map = (char *)read_xpm(tlc_xpm, &tlc_img->width, &tlc_img->height);

	blc_img = (Sprite *)malloc(sizeof(Sprite));
	blc_img->map = (char *)read_xpm(blc_xpm, &blc_img->width, &blc_img->height);

	brc_img = (Sprite *)malloc(sizeof(Sprite));
	brc_img->map = (char *)read_xpm(brc_xpm, &brc_img->width, &brc_img->height);

	trc_img = (Sprite *)malloc(sizeof(Sprite));
	trc_img->map = (char *)read_xpm(trc_xpm, &trc_img->width, &trc_img->height);

	hw_img = (Sprite *)malloc(sizeof(Sprite));
	hw_img->map = (char *)read_xpm(hw_xpm, &hw_img->width, &hw_img->height);

	vw_img = (Sprite *)malloc(sizeof(Sprite));
	vw_img->map = (char *)read_xpm(vw_xpm, &vw_img->width, &vw_img->height);

	tlb_img = (Sprite *)malloc(sizeof(Sprite));
	tlb_img->map = (char *)read_xpm(tlb_xpm, &tlb_img->width, &tlb_img->height);

	trb_img = (Sprite *)malloc(sizeof(Sprite));
	trb_img->map = (char *)read_xpm(trb_xpm, &trb_img->width, &trb_img->height);

	brb_img = (Sprite *)malloc(sizeof(Sprite));
	brb_img->map = (char *)read_xpm(brb_xpm, &brb_img->width, &brb_img->height);

	blb_img = (Sprite *)malloc(sizeof(Sprite));
	blb_img->map = (char *)read_xpm(blb_xpm, &blb_img->width, &blb_img->height);

	teleporter_img = (Sprite *)malloc(sizeof(Sprite));
	teleporter_img->map = (char *)read_xpm(blank_xpm, &teleporter_img->width, &teleporter_img->height);
}

Sprite * piece(int id)
{
	switch(id)
	{
	case 0:
		return blank_img;
	/*case 1:
		return dot_img;
	case 2 :
		return energizer_img;*/			// waiting for images
	case 3:
		return tlc_img;
	case 4:
		return blc_img;
	case 5:
		return trc_img;
	case 6:
		return brc_img;
	case 7:
		return vw_img;
	case 8:
		return hw_img;
	case 9:
		return tlb_img;
	case 10:
		return trb_img;
	case 11:
		return blb_img;
	case 12:
		return brb_img;
	case 50:
		return teleporter_img;
	}

	return blank_img; // default in case of failure
}

Pacman_map * map1_initialize(int xi, int yi)
{
	Pacman_map *map1;
	map1 = (Pacman_map *) malloc(sizeof(Pacman_map));

	map1->width = 24;
	map1->height = 24;
	map1->x = xi;
	map1->y = yi;

	int map_matrix[576] = {
			3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 5,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 9, 8, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 11, 8, 12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 9, 8, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 11, 8, 12, 0, 0, 0, 7, 0, 8, 5, 0, 9, 10, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 6,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 5,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			4, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			3, 8, 8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7,
			4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6
	};

	int i = 0;
	map1->teleporter1_x = 0;
	map1->teleporter1_y = 20;
	map1->teleporter2_x = 23;
	map1->teleporter2_y = 15;

	map_matrix[map1->teleporter1_x + map1->teleporter1_y*map1->width] = 50;
	map_matrix[map1->teleporter2_x + map1->teleporter2_y*map1->width] = 50;

	//printf("T1: %d, T2: %d\n", map1->teleporter1, map1->teleporter2);

	memcpy(map1->matrix, map_matrix, 576*sizeof(int));

	return map1;
}

void draw_map(Pacman_map * map)
{
	Sprite *tmp_piece;
	//tmp_piece = (Sprite *)malloc(sizeof(Sprite));
	long int x = map->x;
	long int y = map->y;
	int counter = 0;
	int prev_piece = 99;

	long int i = 0;
	for(; i < (map->height * map->width); i++)
	{
		if(prev_piece != map->matrix[i])
		{
			tmp_piece = piece(map->matrix[i]);
			prev_piece = map->matrix[i];
		}
		tmp_piece->x = x;
		tmp_piece->y = y;
		draw_img(tmp_piece);
		x = x+30;
		counter++;
		if(counter == 24)
		{
			counter = 0;
			x = map->x;
			y += 30;
		}
	}

	tmp_piece = piece(8);
	tmp_piece->x = map->x + 12*30;
	tmp_piece->y = map->y + 23*30;
	//draw_img(tmp_piece);
}

int is_in_cell(Pacman_map *map, int cell_x, int cell_y, int x_img, int y_img)
{
	int x_coord = cell_x*CELL_SIZE+map->x;
	int y_coord = cell_y*CELL_SIZE + map->y;
	printf("==> C(%d,%d)  P(%d, %d)\n", x_coord, y_coord, x_img, y_img);

	if((x_img >= x_coord) && (x_img <= x_coord+5))
	{
		if((y_img >= y_coord) && (y_img <= y_coord+5))
		{
			return 1;
		}
	}
	return 0;
}

void check_pacman_teleport(Pacman *pacman, Pacman_map *map)
{
	if(is_in_cell(map, map->teleporter1_x, map->teleporter1_y, pacman->img->sp->x, pacman->img->sp->y))
	{
		pacman->img->sp->x = map->teleporter2_x*CELL_SIZE+map->x;
		pacman->img->sp->y = map->teleporter2_y*CELL_SIZE+map->y;

		if(map->teleporter2_x == map->width - 1)
			pacman->img->sp->x -= 10;
		else
			pacman->img->sp->x += 10;
	}
	else if(is_in_cell(map, map->teleporter2_x, map->teleporter2_y, pacman->img->sp->x, pacman->img->sp->y))
	{
		pacman->img->sp->x = map->teleporter1_x*CELL_SIZE+map->x;
		pacman->img->sp->y = map->teleporter1_y*CELL_SIZE+map->y;

		if(map->teleporter1_x == map->width - 1)
			pacman->img->sp->x -= 10;
		else
			pacman->img->sp->x += 10;
	}
}

void check_ghost_teleport(Ghost *ghost, Pacman_map *map)
{
	if(is_in_cell(map, map->teleporter1_x, map->teleporter1_y, ghost->img->x, ghost->img->y))
	{
		ghost->img->x = map->teleporter2_x*CELL_SIZE+map->x;
		ghost->img->y = map->teleporter2_y*CELL_SIZE+map->y;

		if(map->teleporter2_x == map->width - 1)	 // Right teleporter
			ghost->img->x -= 10;
		else
			ghost->img->x += 10;
	}
	else if(is_in_cell(map, map->teleporter2_x, map->teleporter2_y, ghost->img->x, ghost->img->y))
	{
		ghost->img->x = map->teleporter1_x*CELL_SIZE+map->x;
		ghost->img->y = map->teleporter1_y*CELL_SIZE+map->y;

		if(map->teleporter1_x == map->width - 1)	// Left teleporter
			ghost->img->x -= 10;
		else
			ghost->img->x += 10;
	}
}

void check_ghosts_teleport(Ghost *ghosts[], Pacman_map *map)
{
	int i = 0;
	for(;i<4;i++)
	{
		check_ghost_teleport(ghosts[i], map);
	}
}
