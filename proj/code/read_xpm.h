#ifndef _READ_XPM_H
#define _READ_XPM_H

#include <stdio.h>
#include <stdlib.h>

/** Reads a xpm-like sprite defined in "map" (look at pixmap.h for
 * examples). Returns the address of the allocated memory where the
 * sprite was read. Updates "width" and "heigh" with the sprite
 * dimension.  Return NULL on error.
 * Assumes that VRES and HRES (the screen vertical and horizontal resolution)
 * are externaly defined.
 * 
 * Usage example, using the defined sprite in pixmap.h:
 * <pre>
 *   #include "pixmap.h" // defines  pic1, pic2, etc 
 *   int wd, hg;
 *   char *sprite = read_xpm(pic1, &wd, &hg);
 * </pre>
*/
char *read_xpm(char *map[], int *width, int *height);

/**
 * @brief Reads the next xpm on a file pointer to a sprite
 *
 * @param fo pointer to file with xpm to read
 * @param sp pointer to sprite to modify
 */
void read_file_xpm(FILE * fp, Sprite * sp);


#endif // _READ_XPM_H //
