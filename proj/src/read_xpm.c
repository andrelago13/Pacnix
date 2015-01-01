#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "video.h"
#include "sprite.h"

char *read_xpm(char *map[], int *wd, int *ht)
{
  int width, height, colors;
  char sym[256];
  int  col;
  int i, j;
  char *pix, *pixtmp, *tmp, *line;
  char symbol;

  /* read width, height, colors */
  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }
#ifdef DEBUG
  printf("%d %d %d\n", width, height, colors);
#endif
  if (width > HRES || height > VRES || colors > 256) {
    printf("read_xpm: incorrect width, height, colors\n");
    return NULL;
  }

  *wd = width;
  *ht = height;

  for (i=0; i<256; i++)
    sym[i] = 0;

  /* read symbols <-> colors */
  for (i=0; i<colors; i++) {
    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
      return NULL;
    }
#ifdef DEBUG
    printf("%c %d\n", symbol, col);
#endif
    if (col > 256) {
      printf("read_xpm: incorrect color at line %d\n", i+1);
      return NULL;
    }
    sym[col] = symbol;
  }
  
  /* allocate pixmap memory */
  pix = pixtmp = malloc(width*height);

  /* parse each pixmap symbol line */
  for (i=0; i<height; i++) {
    line = map[colors+1+i];
#ifdef DEBUG
    printf("\nparsing %s\n", line);
#endif
    for (j=0; j<width; j++) {
      tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
      if (tmp == NULL) {
    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
    	  return NULL;
      }
      *pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
#ifdef DEBUG
      printf("%c:%d ", line[j], tmp-sym);
#endif
    }
  }

  return pix;
}

void read_file_xpm(FILE * fp, Sprite * sp)
{
	int colors;
	fscanf(fp, "%d %d %d\n", &sp->width, &sp->height, &colors);

	char *char_arr = malloc(colors*sizeof(char));
	char *color_arr = malloc(colors*sizeof(int));

	int i = 0;
	while(i < colors)
	{
		char str;
		int x;
		fscanf(fp, "%c %d\n", &str, &x);

		char_arr[i] = str;
		color_arr[i] = (char) x;

		i++;
	}

	sp->map = (char *)malloc(sizeof(char)*sp->width*sp->height);

	int size = 0;
	char pixel;

	while(size < sp->width * sp->height)
	{
		fscanf(fp, "%c", &pixel);
		if(pixel == (char)'\n')
		{
			fscanf(fp, "%c", &pixel);
		}

		int index = 0;
		while(index < colors)
		{
			if(char_arr[index] == pixel)
			{
				sp->map[size] = color_arr[index];
				index = colors;
			}
			index++;
		}

		size++;
	}
	fscanf(fp, "%c", &pixel);
}
