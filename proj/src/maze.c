#include "maze.h"
#include "sprite.h"
#include "video_gr.h"
#include <math.h>

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
static Sprite *energizer_img;
static Sprite *dot_img;
int energizer_counter;
int energizer_status;
static Sprite *spc_down_left_img;
static Sprite *vw_left_img;
static Sprite *vw_down_left_end_img;
static Sprite *spc_down_right_img;
static Sprite *vw_right_img;
static Sprite *vw_down_right_end_img;
static Sprite *spc_up_left_img;
static Sprite *vw_up_left_end_img;
static Sprite *spc_up_right_img;
static Sprite *vw_up_right_end_img;
static Sprite *bottom_border_img;
static Sprite *top_border_img;
static Sprite *top_lid_img;
static Sprite *bottom_lid_img;
static Sprite *left_lid_img;
static Sprite *right_lid_img;


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
	teleporter_img->map = (char *)read_xpm(teleporter_xpm, &teleporter_img->width, &teleporter_img->height);

	energizer_img = (Sprite *)malloc(sizeof(Sprite));
	energizer_img->map = (char *)read_xpm(energizer_xpm, &energizer_img->width, &energizer_img->height);

	dot_img = (Sprite *)malloc(sizeof(Sprite));
	dot_img->map = (char *)read_xpm(dot_xpm, &dot_img->width, &dot_img->height);

	spc_down_left_img = (Sprite *)malloc(sizeof(Sprite));
	spc_down_left_img->map = (char *)read_xpm(spc_down_left_xpm, &spc_down_left_img->width, &spc_down_left_img->height);

	vw_left_img = (Sprite *)malloc(sizeof(Sprite));
	vw_left_img->map = (char *)read_xpm(vw_left_xpm, &vw_left_img->width, &vw_left_img->height);

	vw_down_left_end_img = (Sprite *)malloc(sizeof(Sprite));
	vw_down_left_end_img->map = (char *)read_xpm(vw_down_left_end_xpm, &vw_down_left_end_img->width, &vw_down_left_end_img->height);

	spc_down_right_img = (Sprite *)malloc(sizeof(Sprite));
	spc_down_right_img->map = (char *)read_xpm(spc_down_right_xpm, &spc_down_right_img->width, &spc_down_right_img->height);

	vw_right_img = (Sprite *)malloc(sizeof(Sprite));
	vw_right_img->map = (char *)read_xpm(vw_right_xpm, &vw_right_img->width, &vw_right_img->height);

	vw_down_right_end_img = (Sprite *)malloc(sizeof(Sprite));
	vw_down_right_end_img->map = (char *)read_xpm(vw_down_right_end_xpm, &vw_down_right_end_img->width, &vw_down_right_end_img->height);

	spc_up_left_img = (Sprite *)malloc(sizeof(Sprite));
	spc_up_left_img->map = (char *)read_xpm(spc_up_left_xpm, &spc_up_left_img->width, &spc_up_left_img->height);

	vw_up_left_end_img =(Sprite *)malloc(sizeof(Sprite));
	vw_up_left_end_img->map = (char *)read_xpm(vw_up_left_end_xpm, &vw_up_left_end_img->width, &vw_up_left_end_img->height);

	spc_up_right_img = (Sprite *)malloc(sizeof(Sprite));
	spc_up_right_img->map = (char *)read_xpm(spc_up_right_xpm, &spc_up_right_img->width, &spc_up_right_img->height);

	vw_up_right_end_img = (Sprite *)malloc(sizeof(Sprite));
	vw_up_right_end_img->map = (char *)read_xpm(vw_up_right_end_xpm, &vw_up_right_end_img->width, &vw_up_right_end_img->height);

	bottom_border_img = (Sprite *)malloc(sizeof(Sprite));
	bottom_border_img->map = (char *)read_xpm(bottom_border_xpm, &bottom_border_img->width, &bottom_border_img->height);

	top_border_img = (Sprite *)malloc(sizeof(Sprite));
	top_border_img->map = (char *)read_xpm(top_border_xpm, &top_border_img->width, &top_border_img->height);

	top_lid_img = (Sprite *)malloc(sizeof(Sprite));
	top_lid_img->map = (char *)read_xpm(top_lid_xpm, &top_lid_img->width, &top_lid_img->height);

	bottom_lid_img = (Sprite *)malloc(sizeof(Sprite));
	bottom_lid_img->map = (char *)read_xpm(bottom_lid_xpm, &bottom_lid_img->width, &bottom_lid_img->height);

	left_lid_img = (Sprite *)malloc(sizeof(Sprite));
	left_lid_img->map = (char *)read_xpm(left_lid_xpm, &left_lid_img->width, &left_lid_img->height);

	right_lid_img = (Sprite *)malloc(sizeof(Sprite));
	right_lid_img->map = (char *)read_xpm(right_lid_xpm, &right_lid_img->width, &right_lid_img->height);

	energizer_counter = (int) BLINK_INTERVAL;
	energizer_status = 1;
}

Sprite * piece(int id)
{
	switch(id)
	{
	case 0:
		return blank_img;
	case 1:
		return dot_img;
	case 2 :
		return energizer_img;
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
	case 13:
		return spc_down_left_img;
	case 14:
		return vw_left_img;
	case 15:
		return vw_down_left_end_img;
	case 16:
		return spc_down_right_img;
	case 17:
		return vw_right_img;
	case 18:
		return vw_down_right_end_img;
	case 19:
		return spc_up_left_img;
	case 20:
		return vw_up_left_end_img;
	case 21:
		return spc_up_right_img;
	case 22:
		return vw_up_right_end_img;
	case 23:
		return bottom_border_img;
	case 24:
		return top_border_img;
	case 25:
		return top_lid_img;
	case 26:
		return bottom_lid_img;
	case 27:
		return left_lid_img;
	case 28:
		return right_lid_img;
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

	int map_matrix[] = {
			3, 8, 8, 8, 	8, 8, 8, 8, 	8, 8, 8, 13,	 16, 8, 8, 8, 	8, 8, 8, 8,	  8, 8, 8, 5,
			7, 1, 1, 1,		1, 1, 1, 1, 	1, 1, 1, 14,	 17,1, 1, 1,	1, 1, 1, 1,	  1, 1, 1, 7,
			7, 2, 9, 10, 	1, 9, 24, 10, 	1, 25, 1, 14, 	 17, 1, 25, 1, 	9, 24, 10, 1, 9, 10, 2, 7,
			7, 1, 11, 12, 	1, 11, 23, 12, 	1, 26, 1, 11, 	 12, 1, 26, 1,  11, 23, 12, 1,11, 12, 1, 7,
			7, 1, 1, 1,		1, 1, 1, 1,		1, 1, 1, 1,		 1, 1, 1, 1, 	1, 1, 1, 1,   1, 1, 1, 7,
			7, 1, 9, 10, 	1, 25, 1, 27, 	8, 5, 1, 27, 	 28, 1, 3, 8, 	28, 1, 25, 1, 9, 10, 1, 7,
			7, 1, 11, 12, 	1, 7, 1, 1, 	1, 7, 1, 1, 	 1, 1, 7, 1, 	1, 1, 7, 1,   11, 12, 1, 7,
			7, 1, 1, 1, 	1, 7, 8, 28, 	1, 4, 8, 8, 	 8, 8, 6, 1, 	27, 8, 7, 1,  1, 1, 1, 7,
			7, 24, 24, 10, 	1, 7, 0, 0, 	0, 0, 0, 0, 	 0, 0, 0, 0, 	0, 0, 7, 1,   9, 24, 24, 7,
			7, 0, 0, 17, 	1, 7, 0, 25, 	1, 25, 0, 27, 	 28, 0, 25, 1, 	25, 0, 7, 1,  14, 0, 0, 7,
			7, 23, 23, 12,  1, 26, 0, 7, 	1, 7, 0, 0, 	 0, 0, 7, 1, 	7, 0, 26, 1,  11, 23, 23, 7,
			50, 0, 0, 0, 	0, 0, 0, 7, 	1, 26, 0, 9, 	 10, 0, 26, 1, 	7, 0, 0, 0,   0, 0, 0, 50,
			7, 24, 24, 10, 	1, 25, 0, 7, 	1, 0, 0, 11, 	 12, 0, 0, 1, 	7, 0, 25, 1,  9, 24, 24, 7,
			7, 0, 0, 17, 	1, 7, 0, 7, 	1, 25, 0, 0, 	 0, 0, 25, 1, 	7, 0, 7, 1,   14, 0, 0, 7,
			7, 23, 23, 12,  1, 26, 0, 26, 	1, 26, 0, 27, 	 28, 0, 26, 1,  26, 0, 26, 1, 11, 23, 23, 7,
			7, 1, 1, 1, 	1, 0, 0, 0, 	0, 0, 0, 0, 	 0, 0, 0, 0, 	0, 0, 0, 1,   1, 1, 1, 7,
			7, 1, 27, 5, 	1, 3, 8, 28, 	1, 27, 8, 8, 	 8, 8, 28, 1, 	27, 8, 5, 1,  3, 28, 1, 7,
			7, 2, 1, 7, 	1, 7, 1, 1, 	1, 1, 1, 1, 	 1, 1, 1, 1, 	1, 1, 7, 1,	  7, 1, 2, 7,
			7, 28, 1, 26,	1, 26, 1, 25, 	1, 27, 8, 8, 	 8, 8, 28, 1,   25, 1, 26, 1, 26, 1, 27, 7,
			7, 1, 1, 1, 	1, 1, 1, 7, 	1, 1, 1, 14, 	 17, 1, 1, 1,   7, 1, 1, 1,   1, 1, 1, 7,
			7, 1, 27, 8, 	8, 8, 8, 8, 	8, 28, 1, 11, 	 12, 1, 27, 8,  8, 8, 8, 8,   8, 28, 1, 7,
			7, 1, 1, 1, 	1, 1, 1, 1, 	1, 1, 1, 1, 	 1, 1, 1, 1, 	1, 1, 1, 1,   1, 1, 1, 7,
			4, 8, 8, 8, 	8, 8, 8, 8, 	8, 8, 8, 8, 	 8, 8, 8, 8, 	8, 8, 8, 8,   8, 8, 8, 6,
			0, 0, 0, 0, 	0, 0, 0, 0, 	0, 0, 0, 0, 	 0, 0, 0, 0, 	0, 0, 0, 0,   0, 0, 0, 0
	};

	int i = 0;
	map1->teleporter1_x = 0;
	map1->teleporter1_y = 11;
	map1->teleporter2_x = 23;
	map1->teleporter2_y = 11;

	map_matrix[map1->teleporter1_x + map1->teleporter1_y*map1->width] = 50;
	map_matrix[map1->teleporter2_x + map1->teleporter2_y*map1->width] = 50;

	memcpy(map1->matrix, map_matrix, 576*sizeof(int));

	return map1;
}

void draw_map(Pacman_map * map)
{
	Sprite *tmp_piece;
	long int x = map->x;
	long int y = map->y;
	int counter = 0;
	int prev_piece = 99;

	energizer_counter--;
	if(energizer_counter == 0)
	{
		energizer_counter = (int)BLINK_INTERVAL;
		energizer_status = abs(energizer_status - 1);
	}

	long int i = 0;
	for(; i < (map->height * map->width); i++)
	{
		if(prev_piece != map->matrix[i])
		{
			tmp_piece = piece(map->matrix[i]);
			prev_piece = map->matrix[i];
		}

		if((map->matrix[i] == 2) && (energizer_status < 1))
		{
			tmp_piece = piece(0);
		}

		tmp_piece->x = x;
		tmp_piece->y = y;


		draw_img(tmp_piece);
		x = x+30;
		counter++;
		if(counter == map->width)
		{
			counter = 0;
			x = map->x;
			y += 30;
		}
	}
}

int is_in_cell(Pacman_map *map, int cell_x, int cell_y, int x_img, int y_img)
{
	int x_coord = cell_x*CELL_SIZE+map->x;
	int y_coord = cell_y*CELL_SIZE + map->y;

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

double ghost_dist_teleporter(Ghost *ghost, Pacman_map *map, int *direction)
{
	double dist1 = sqrt((ghost->img->x + ghost->img->width - (map->teleporter1_x*CELL_SIZE+map->x+CELL_SIZE/2))^2 + (ghost->img->y + ghost->img->height - (map->teleporter1_y*CELL_SIZE+map->y+CELL_SIZE/2))^2);
	double dist2 = sqrt((ghost->img->x + ghost->img->width - (map->teleporter2_x*CELL_SIZE+map->x+CELL_SIZE/2))^2 + (ghost->img->y + ghost->img->height - (map->teleporter2_y*CELL_SIZE+map->y+CELL_SIZE/2))^2);

	if(dist1 < dist2)
	{
		if(map->teleporter1_x == 0)
			*direction = LEFT;
		else
			*direction = RIGHT;
		return dist1;
	}
	else
	{
		if(map->teleporter2_x == 0)
			*direction = LEFT;
		else
			*direction = RIGHT;
		return dist2;
	}
}

int cell_type(int x, int y, Pacman_map *map)
{
	int pos = cell_pos(x, y, map);
	return map->matrix[pos];
}

int cell_pos(int x, int y, Pacman_map *map)
{
	int x_pos_pix = x - map->x;
	int y_pos_pix = y - map->y;
	int x_pos = 0;
	int y_pos = 0;

	while(x_pos_pix >= CELL_SIZE)
	{
		x_pos_pix -= CELL_SIZE;
		x_pos++;
	}
	while(y_pos_pix >= CELL_SIZE)
	{
		y_pos_pix -= CELL_SIZE;
		y_pos++;
	}

	return x_pos + y_pos*map->width;
}

int fill_cell(int x, int y, Pacman_map *map, int value)
{
	int pos = cell_pos(x, y, map);
	map->matrix[pos] = value;
}
