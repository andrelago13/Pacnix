#ifndef _SPRITE_H_
#define _SPRITE_H_

/** @defgroup sprite Sprite
 * @{
 * @brief Sprite related functions
 */

/**
 * @brief Represents a basic Sprite object
 *
 * A Sprite is an "object" that contains all needed information to
 * create, animate, and destroy a pixmap.  The functions assume that
 * the background is BLACK and they take into account collision with
 * other graphical objects or the screen limits. 
 */
typedef struct {
  int x,y;             /**< current sprite position */
  int width, height;   /**< sprite dimensions */
  char *map;           /**< the sprite pixmap (use read_xpm()) */
} Sprite;

/**
 * @brief Represents a basic moving Sprite object
 *
 * Basically is a Sprite that has a "goal" position in the screen. It can be moved by
 * a funtion in video_gr.c file
 */
typedef struct {
  Sprite *sp;
  double xspeed, yspeed;  /**< current speeds in the x and y direction */
  int x_to_reach, y_to_reach;  /**< x and y coordinates to reach at end of movement */
} Sprite_move;


/** Creates with random speeds (not zero) and position
 * (within the screen limits), a new sprite with pixmap "pic", in
 * memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite * create_sprite(char *pic[], int x_ini, int y_ini);

/**
 * @brief Clears the memmory occupied by a sprite, and it's pointer
 */
void destroy_sprite(Sprite * sp);

/** @} end of sprite */

#endif
