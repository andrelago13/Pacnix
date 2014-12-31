/**
 *  @author Joao Cardoso (jcard@fe.up.pt) ????
 *  Added by pfs@fe.up.pt
 */

#ifndef _ASPRITE_H_
#define _ASPRITE_H_

#include <stdarg.h>
#include "sprite.h"

/** @defgroup animsprite AnimSprite
 * @brief Functions for animated sprites
 *
 * Animated Sprites are Sprites with a set of images available,
 * and switch between them on a certain rate
 */

/**@brief Struct representing Animated Sprites
 *
 * An Animated Sprite is a "sub-classing" of Sprites
 *  where each Sprite is manipulated using Sprite functions
 */
typedef struct {
	Sprite *sp;		///< pointer to first Sprite, the one with overall properties
	int aspeed;		///< animation speed
	int cur_aspeed; ///< current animation speed
	int num_fig; 	///< number of pixmaps 
	int cur_fig; 	///< current pixmap 
	char **map;     ///< pointer to array of each AnimSprite pixmaps
} AnimSprite;

/**
 * @brief Initialize an animated sprite
 *
 * Create an Animated Sprite from multiple pixmaps
 *   At least one pixmap must be specified.
 *   speed in anim's per second
 *
 * @param maps array with set of images for AnimSprite to alternate between
 * @param num_map number of images on previous array
 * @param speed number of frames to remain on an image
 * @param x_ini x coordinate for the sprite
 * @param y_ini y coordinate for the sprite
 * @param map_ini pixmap with the first image to be loaded to the sprite (necessary to set witdth and height of image)
 * @return pointer to AnimSprite created
 */
AnimSprite * create_asprite(char **maps, int num_map, int speed, int x_ini, int y_ini, char *map_ini[]);

/**
 * @brief Animate an Animated Sprite
 *
 * Animates a AnimSprite by incrementing the cur_aspeed counter
 * and checking if the value os aspeed was reached. When this happens,
 * it switches to the next image on the array of images
 *
 * @param fig pointer to AnimSprite to animate
 * @return 0 if animation occurred (image was switched), 1 otherwise
 */
int animate_asprite(AnimSprite *fig);

/** @} end of animsprite */

#endif
