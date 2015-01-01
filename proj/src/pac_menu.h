#ifndef __PAC_MENU_H
#define __PAC_MENU_H

#include "pacnix.h"
#include <stdio.h>
#include <stdlib.h>

/** @defgroup pac_menu Pac_menu
 * @{
 * @brief Functions for drawing and interacting with game menus
 */

/**
 * @brief Initializes static menu Sprites
 */
void initialize_menu_pieces();

/**
 * @brief Clears memory taken by static menu Sprites
 */
void clear_menu_pieces();

/**
 * @brief Draws the main menu
 *
 * Draws the main menu surrounding the current selection with a rectangle
 *
 * @param mouse current status of the mouse, used to determine current selection
 */
void draw_main_menu(Mouse_coord * mouse);

/**
 * @brief Draws the instructions menu
 */
void draw_inst_menu();

/**
 * @brief Draws the "about" menu
 */
void draw_about_menu();

/**
 * @brief In the main menu, checks if mouse was clicked on an option
 *
 * Checks if the mouse clicked on a menu option, returning it
 *
 * @param mouse pointer to current mouse status
 * @return menu option if mouse is clicking one, -1 otherwise
 */
int check_mainmenu_click(Mouse_coord * mouse);

/**
 * @brief Interprets arrow clicking on main menu
 *
 * If user clicked an arrow, switches the current option accordingly
 * If user clicked enter, returns the current menu option
 *
 * @param key scan-code of key pressed (or released)
 * @return menu option if enter was pressed, -1 otherwise
 */
int arrow_click(unsigned long key);

#endif // __PAC_MENU_H //
