#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

/** @defgroup video_gr video_gr
 * @brief Functions for outputing data to screen in graphics mode
 */

#include "sprite.h"

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * @brief Draws a square in the screen (interior filled)
 *
 * Uses the given coordinates to draw a size*size square filled
 * with color "color" in the screen
 *
 * @param x x coordinate of top left square corner
 * @param y y coordinate of top left square corner
 * @param size length of square sides
 * @param color hexadecimal value of square color
 * @return 0 upon success, 1 on failure
 */
int draw_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color);

/**
 * @brief Paints specified pixel of the screen with given color
 *
 * @param x x coordinate of pixel
 * @param y y coordinate of pixel
 * @param color hexadecimal value of pixel color
 * @return 0 upon success, 1 on failure
 */
int paint_pixel(unsigned short x, unsigned short y, unsigned long color);

/**
 * @brief Draws a line connecting two given points of the screen
 *
 * @param xi x coordinate of one line limit
 * @param yi y coordinate of one line limit
 * @param xf y coordinate of the other line limit
 * @param yf y coordinate of the other line limit
 * @param color hexadecimal value of line color
 * @return 0 upon success, 1 on failure
 */
int draw_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color);

/**
 * @brief Draws the Sprite given as argument
 *
 * @param img pointer to the sprite to draw
 * @return 0 upon success, 1 on failure
 */
int draw_img(Sprite *img);

/**
 * @brief Draws the Sprite given as argument as screen background
 *
 *	Draws the given Sprite, which should have the dimensions of the screen
 *	as the background of the screen
 *
 * @param map pointer to array representing the image
 */
void draw_back_map(char *map);

/**
 * @brief Draws the Sprite_move given, moving it from it's
 * start point to the finnish point
 *
 * @param img pointer to the Sprite_move to draw and move
 * @return 0 upon success, 1 on failure
 */
int move_img(Sprite_move *img);

/**
 * @brief Deletes the Sprite given from the screen
 *
 * Fills the area occupied by the Sprite with color black
 *
 * @param img pointer to the Sprite to delete
 * @return 0 upon success, 1 on failure
 */
int delete_img(Sprite *img);

/**
 * @brief Displays VBE configuration in a text mode screen
 *
 * @return 0 upon success, 1 on failure
 */
int test_controller_config();

/**
 * @brief Updates the video_mem buffer with contents of double, secondary buffer
 *
 * This function is used to implement the double buffering feature of the program. When anything is written to the screen,
 * it is actually written to a temporary buffer, video_mem_buffer. Upon passing of a new frame (controlled by function caller)
 * The auxiliary buffer is coppied to the actual video_mem buffer, making it appear on the screen. This prevents problems
 * on large information display with several sprites (moving or not)
 */
void update_buffer();

/**
 * @brief Draws a rectangle with vertexes on given coordinates
 *
 * Draws a rectangle which's opposing vertexes are on given coordinates,
 * filling it with the given color
 *
 * @param xi x coordinate of one vertex
 * @param yi y coordinate of one vertex
 * @param xf x coordinate of the other vertex
 * @param yf y coordinate of the other vertex
 * @param color hexadecimal value of rectangle color
 * @return 0 upon success, 1 on failure
 */
int draw_rectangle(int xi, int yi, int xf, int yf, unsigned long color);

/**
 * @brief Surrounds an image with the borders of a rectangle with the same size
 *
 * Draws 4 lines surrounding the given image, providing a frame for
 * it with the exact same size
 *
 * @param sp pointer to sprite to surround
 * @param color hexadecimal value of border color
 */
void surround_img(Sprite * sp, unsigned long color);

/**
 * @brief Determine if a point is in an image
 *
 * Determines if the point whose coordinates are given
 * is "inside" the area occupied by the specified sprite
 *
 * @param sp pointer to sprite to evaluate
 * @param x x coordinate of point to evaluate
 * @param y y coordinate of point to evaluate
 * @return 1 if true, 0 if false
 */
int is_in_img(Sprite * sp, int x, int y);

/**
 * @brief Prints a sprite with a number with the given color
 *
 * This function receives a Sprite that contains the image of a white
 * number on a transparent backgound, and prints it, replacing the white color
 * of the number with the value specified as argument. This is very useful for
 * displaying integer numbers with different colors, without the need of
 * editing the image file.
 *
 * @param sp pointer to sprite to draw
 * @param color hexadecimal value of color to draw the number with
 */
void print_num(Sprite * sp, unsigned long color);

/**
 * @brief Returns a pointer to a specific pixel of the screen
 *
 * @param x x coordinate of pixel to return
 * @param y y coordinate of pixel to return
 * @return char pointer to the specified pixel
 */
char * pixel_vid(unsigned int x, unsigned int y);

/**
 * @brief Fills the screen with a given color
 *
 * Useful for drawing a background color efficiently and
 * "resetting" the auxiliar video buffer
 *
 * @param color hexadecimal value of color to fill screen with
 */
void fill_screen(unsigned long color);


/** @} end of video_gr */

#endif /* __VIDEO_GR_H */
