#ifndef __PACNIX_H
#define __PACNIX_H

#include "asprite.h"
#include "mouse.h"

/** @defgroup pacnix Pacnix
 * @{
 * @brief Main module for Pacnix game implementation
 *
 * This module contains the most important function implementations
 * for the game to be possible. Everything from character movement, score
 * storage, menu interaction and interrupt handling is made here. The module is divided in several sections, according
 * to the data structures focused in them
 */


#define MENU_IMG_FILE "/home/pacnix/res/menu_pixmaps.txt"		/**< @brief File storing menu images */
#define HIGHSCORE_FILE "/home/pacnix/res/highscore.txt"		/**< @brief File storing game highscore */

#define MOUSE_X_INI_COORD 450	/**< @brief Initial x coordinate for mouse cursor */
#define MOUSE_Y_INI_COORD 400	/**< @brief Initial y coordinate for mouse cursor */

#define GHOST_SPAWN_X 374		/**< @brief x coordinate for ghost spawn */
#define GHOST_SPAWN_Y 270		/**< @brief y coordinate for ghost spawn */
#define PACMAN_SPAWN_X 374		/**< @brief x coordinate for pacman spawn */
#define PACMAN_SPAWN_Y 480		/**< @brief y coordinate for pacman spawn */

#define PACMAN_SPEED 2			/**< @brief pacman movement speed */
#define PACMAN_LIVES 3			/**< @brief number of lives for pacman */
#define PACMAN_SPAWN_TIME 2		/**< @brief time before pacman spawns */

#define GHOST_MODE_RANDOM 0		/**< @brief value for ghost random mode */
#define GHOST_MODE_CHASE 1		/**< @brief value for ghost chase mode */
#define GHOST_MODE_USER 2		/**< @brief value for ghost user controlled mode */
#define GHOST_MODE_EVADE 3		/**< @brief value for ghost evade mode */
#define GHOST_MODE_TIMED 4		/**< @brief value for ghost timed mode */
#define GHOST_MODE_USER_EVADE 5	/**< @brief value for ghost user controlled when evading pacman mode */

#define ORANGE_GHOST_SPAWN_TIME 8		/**< @brief spawn time for orange ghost */
#define RED_GHOST_SPAWN_TIME 2			/**< @brief spawn time for red ghost */
#define BLUE_GHOST_SPAWN_TIME 6			/**< @brief spawn time for blue ghost */
#define PINK_GHOST_SPAWN_TIME 3			/**< @brief spawn time for pink ghost */

#define RED_GHOST_CHASE_TIME 7			/**< @brief time red ghost stays in chase mode */
#define RED_GHOST_RANDOM_TIME 2			/**< @brief time red ghost stays in random mode */
#define PINK_GHOST_CHASE_TIME 10		/**< @brief time pink ghost stays in chase mode */
#define PINK_GHOST_RANDOM_TIME 7		/**< @brief time pink ghost stays in random mode */
#define BLUE_GHOST_CHASE_TIME 12		/**< @brief time blue ghost stays in chase mode */
#define BLUE_GHOST_RANDOM_TIME 5		/**< @brief time blue ghost stays in random mode */

#define BONUS_X_INI_COORD 375			/**< @brief x coordinate for bonus spawn */
#define BONUS_Y_INI_COORD 480			/**< @brief y coordinate for bonus spawn */
#define BONUS_SCORE 200					/**< @brief score value of the bonus */
#define BONUS_SPAWN_TIME 10				/**< @brief bonus spawn time */
#define BONUS_DURATION 12				/**< @brief bonus duration available */

#define SCORE_GHOST_EATEN 300			/**< @brief score obtained when a ghost is eaten by the pacman */
#define SCORE_PACDOT_EATEN 10			/**< @brief score obtained when a pac-dot is eaten by the pacman */
#define SCORE_ENERGIZER_EATEN 50		/**< @brief score obtained when an energizer is eaten by the pacman */
#define SCORE_PER_REMAINING_LIFE 200	/**< @brief score obtained per remaining life when player wins the game */


#define GHOST_SPEED 2		/**< @brief ghost's movement speed */


#define DOWN 0		/**< @brief Down direction for character movement */
#define RIGHT 1		/**< @brief Right direction for character movement */
#define UP 2		/**< @brief Up direction for character movement */
#define LEFT 3		/**< @brief Left direction for character movement */

#define COLOR_GHOST_ORANGE 0	/**< @brief Orange color for ghosts */
#define COLOR_GHOST_RED 1		/**< @brief Red color for ghosts */
#define COLOR_GHOST_BLUE 2		/**< @brief Blue color for ghosts */
#define COLOR_GHOST_PINK 3		/**< @brief Pink color for ghosts */

// Directions to find pacman for ghosts in chase mode
#define PACDIR_S 0		/**< @brief South */
#define PACDIR_SSE 1	/**< @brief South-southeast */
#define PACDIR_ESE 2	/**< @brief East-southeast */
#define PACDIR_E 3		/**< @brief East*/
#define PACDIR_ENE 4	/**< @brief East-northeast */
#define PACDIR_NNE 5	/**< @brief North-northeast */
#define PACDIR_N 6		/**< @brief North */
#define PACDIR_NNW 7	/**< @brief North-norwest */
#define PACDIR_WNW 8	/**< @brief West-northwest */
#define PACDIR_W 9		/**< @brief West */
#define PACDIR_WSW 10	/**< @brief West-southwest */
#define PACDIR_SSW 11	/**< @brief South-southwest */

#define COLLISION_TOLERANCE 0	/**< @brief Tolerance in pixels to be subtracted to distance between sprites on collision detection */


#define GAME_FPS 50				/**< @brief FPS rate for the game to be played at */



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// MAIN GAME FUNCTIONS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





/**
 * @brief Determines if a new frame has been completed
 *
 * Acts upon the interrupt of timer 0 (60Hz) and calculates
 * if, upon that interrupt, a new frame has passed, according to
 * the frame rate defined by GAME_FPS
 *
 * @return 1 upon new frame, 0 otherwise
 */
int fps_tick();

/**
 * @brief Manages the menus of the game
 *
 * This function can draw any of the menus of the game,
 * and interpret the user interactions to find out what action
 * should be done next
 *
 * @param prev_score Score of the previous game. If it is greater than 0, the menu will display it
 * @param screen Determines the desired menu (0 - main menu, 1 - instructions, 2 - about)
 * @param highscore current pacman highscore to be displayed
 * @return action to be followed next
 */
int start_menu(int prev_score, int screen, int highscore);

/**
 * @brief Manages the user actions
 *
 * This function starts the game by loading the main menu, and then
 * uses the start_menu() and game_local() functions to find out what to
 * do next. It changes between menus by passing the right argument to start_menu()
 * and also starts the game in single or multiplayer mode, recording it's score
 */
void pacnix_start();

/**
 * @brief Starts a local game
 *
 * This function starts a local game session, choosing between
 * single or multiplayer mode according to it's argument.
 * This function also manages all the info about a game, storing all the characters and
 * entities involved and choosing how to act upon interrupts according to the game mode.
 *
 * @param game_mode 0 if singleplayer, 1 if multiplayer
 * @param highscore current pacman highscore to be displayed
 * @return 0 if score was 0 or game was multiplayer, greater than zero if game was singleplayer and the user scored
 */
int game_local(int game_mode, int highscore);

/**
 * @brief Reads the previous game highscore from the decided file
 *
 * @return highscore of previous game
 */
int read_highscore();

/**
 * @brief Stores a score value to the defined file
 */
void store_highscore(int new_score);





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// PACMAN FUNCTIONS ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/**@brief Struct representing a Pacman
 *
 * Contains the information of the Pacman character,
 * including position, movement info and image info, as well
 * as spawn timing and lives remaining
 */
typedef struct {
	AnimSprite *img;
	int direction;			///<  0 - down, 1 - right, 2 - up, 3 - left
	int desired_direction; 	///< same as previous. Indicates the direction the user wants pacman to go. It only goes that direction if there are no obstacles
	int mode;				///<  0 - normal, 1 - powered (has eaten Energizer)
	int lives;				///< number of lives left before pacman dies
	int spawn_timer;		///< time in seconds it takes pacman to spawn when he dies
	int speed;				///< speed of pacman's movement
} Pacman;



/**
 * @brief Initializes a Pacman object
 *
 * This function initializes all the pacman info, such as
 * game_related info (lives, mode, etc) and sprites
 *
 * @param xi initial x coordinate for pacman
 * @param yi initial y coordinate for pacman
 * @param speed pacman movement speed
 * @param lives ammount of lives for pacman to start with
 * @param spawn_time pacman's spawn timer
 * @return pointer to created pacman object
 */
Pacman * pacman_init(int xi, int yi, int speed, int lives, int spawn_time);

/**
 * @brief Rotates the pacman
 *
 * This function changes the pacman's direction
 * and it's image set to correspond that direction
 *
 * @param pacman pointer to pacman
 * @param direction new pacman direction
 */
void pacman_rotate(Pacman * pacman, int direction);

/**
 * @brief Changes pacman's desired direction
 *
 * This function reads a keyboard scan_code
 * changing the pacman's desired direction if
 * that scan_code is an arrow make_code
 *
 * @param pacman pointer to pacman
 * @param scan_code scan_code of key read
 */
void pacman_read_key(Pacman * pacman, unsigned long scan_code);

/**
 * @brief Tries to rotate pacman to it's desired direction
 *
 * This function checks if the Pacman's desired direction
 * is blocked by obstacles or not. If it isn't, it
 * changes the pacman's current direction to the one specified
 *
 * @param pacman pointer to pacman
 */
void pacman_try_rotate(Pacman * pacman);

/**
 * @brief Move the pacman upon new frame
 *
 * This function updates pacman's direction with it's
 * desired direction (calling pacman_try_rotate()) and
 * then moves it forward if there are no obstacles in front
 *
 * @param pacman pointer to pacman
 */
void pacman_move_tick(Pacman * pacman);

/**
 * @brief Searches pacman's front for obstacles
 *
 * @param pacman pointer to pacman
 * @return 1 if front is blocked, 0 otherwise
 */
int pacman_check_surroundings(Pacman * pacman);

/**
 * @brief Manages pacman's status before spawning
 *
 * This function decrements pacman's spawn timer and
 * keeps it turning right while that counter does
 * not reach zero
 *
 * @param pacman pointer to pacman
 */
void pacman_spawn_timer(Pacman * pacman);

/**
 * @brief Returns the array of 7 pacman sprites for a given direction
 *
 * @param direction direction pacman must be facing in return array of sprites
 */
char ** pacman_maps(int direction);

/**
 * @brief Returns pacman's right sprites
 *
 * @return array of 7 sprites of pacman looking to the right
 */
char ** pacman_right_maps();

/**
 * @brief Returns pacman's left sprites
 *
 * @return array of 7 sprites of pacman looking to the left
 */
char ** pacman_left_maps();

/**
 * @brief Returns pacman's up sprites
 *
 * @return array of 7 sprites of pacman looking to the up
 */
char ** pacman_up_maps();

/**
 * @brief Returns pacman's down sprites
 *
 * @return array of 7 sprites of pacman looking to the down
 */
char ** pacman_down_maps();





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// GHOST FUNCTIONS ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




/**@brief Struct representing a Ghost
 *
 * Contains the information of the Ghost character,
 * including position, movement info and image info, as well
 * as spawn timing, lives remaining and acting mode
 */
typedef struct {
	Sprite *img;
	int direction;			///< 0 - down, 1 - right, 2 - up, 3 - left
	int mode;				///< 0 - random, 1 - chase, 2 - user controlled, 3 - evade pacman, 4 - timed, 5 - user controlled evading
	int desired_direction; 	///< for user_controlled movement
	int prev_mode;			///< auxiliary variable to store the mode a ghost had before becoming user_controlled
	int color;				///< 0 - orange, 1 - red, 2 - blue, 3 - pink
	int speed;				///< ghost movement speed
	int chase_time;			///< time in seconds ghost will be chasing pacman
	int random_time;		///< time in seconds ghost will be in random mode
	int curr_counter;		///< counter of seconds, used to switch between modes
	int temp_mode;			///< current temporary ghost mode when changing between modes
	int escape_counter;		///< number of seconds left when ghost is escaping from pacman
	int spawn_timer;		///< number of seconds left before ghost spawns
} Ghost;



/**
 * @brief Creates a single-mode ghost object
 *
 * This function creates a ghost that never changes it's mode,
 * except when pacman eats an energizer.
 *
 * @param xi intial x ghost coordinate
 * @param yi initial y ghost coordinate
 * @param speed ghost movement speed
 * @param color ghost's color
 * @param mode ghost's acting mode
 * @param spawn_time value to set ghost spawn_timer to
 * @return pointer to created ghost object
 */
Ghost * ghost_init(int xi, int yi, int speed, int color, int mode, int spawn_time);

/**
 * @brief Creates a double-mode ghost object
 *
 * This function creates a ghost that changes between chase and random
 * modes at a certain rate that can be different for both modes
 *
 * @param xi intial x ghost coordinate
 * @param yi initial y ghost coordinate
 * @param speed ghost movement speed
 * @param color ghost's color
 * @param mode ghost's acting mode
 * @param spawn_time value to set ghost spawn_timer to
 * @param chase_time seconds for ghost to be in chase mode
 * @param random_time seconds for ghost to be in rando mode
 * @return pointer to created ghost object
 */
Ghost * ghost_init_switcher(int xi, int yi, int speed, int color, int mode, int spawn_time, int chase_time, int random_time);

/**
 * @brief Update ghost to new direction
 *
 * This function changes the ghost's direction and
 * it's maps accordingly
 *
 * @param ghost pointer to ghost to change
 * @param direction new ghost direction
 */
void ghost_rotate(Ghost * ghost, int direction);

/**
 * @brief Returns the ghost color and direction image pretended
 *
 * @param color of the ghost (9 and 10 are used for when the ghost is evading, for a different image)
 * @param direction new ghost direction
 * @return pointer to new image's map
 */
char* ghost_dir_map(int color, int direction);

/**
 * @brief Rotates a user controlled ghost when a key is pressed or released
 *
 * @param ghost pointer to the ghost to rotate
 * @param scan_code scan code of the key involved
 */
void ghost_read_key(Ghost * ghost, unsigned long scan_code);

/**
 * @brief Checks the ghost's front for obstacles
 *
 * @param ghost pointer to ghost to check
 * @return 1 if front is blocked, 0 otherwise
 */
int ghost_check_surroundings(Ghost * ghost);

/**
 * @brief Moves any ghost according to it's mode
 *
 * As a note, to make the game easier for the pacman,
 * ghosts in evade mode sometimes act randomly (at a given
 * percentage). If they were always evading the pacman, it would
 * be almost impossible for him to catch them unless
 * by trapping them in corners of the map, and even then it would
 * be very difficult and unlikely.
 *
 * @param ghost pointer to ghost to move
 * @param pacman pointer to current game's pacman
 */
void move_ghost(Ghost * ghost, Pacman * pacman);

/**
 * @brief Moves any ghost in random mode
 *
 * When the ghost is at an intersection (a point of the map
 * with at least 3 alternative ways or 2 that are not opposing directions)
 * , this function makes it move to a random direction between those available
 *
 * @param ghost pointer to ghost to move
 */
void move_ghost_random(Ghost * ghost);

/**
 * @brief Moves any ghost in chase mode
 *
 * When the ghost is at an intersection (a point of the map
 * with at least 3 alternative ways or 2 that are not opposing directions)
 * , this function calculates the ideal direction to follow, and follows it
 * if it isn't blocked. If it is, it either goes to a second prefered direction
 * or chooses a random one between those available
 *
 * @param ghost pointer to ghost to move
 * @param pacman pointer to current game's pacman
 */
void move_ghost_chase(Ghost * ghost, Pacman * pacman);

/**
 * @brief Moves any ghost controlled by an user
 *
 * This function tries to move a ghost to it's desired direction, which
 * is set by the user's actions (arrow key pressing)
 *
 * @param ghost pointer to ghost to move
 */
void move_ghost_user(Ghost * ghost);

/**
 * @brief Moves any ghost in escape mode
 *
 * This function is similar to move_ghost_chase() and it's mechanisms
 * are similar as well, the only difference is that it tries to follow
 * the opposing direction to the pacman.
 *
 * @param ghost pointer to ghost to move
 * @param pacman pointer to current game's pacman
 */
void move_ghost_escape(Ghost * ghost, Pacman * pacman);

/**
 * @brief Moves any ghost that switches between two modes
 *
 * This function is similar to the others, but instead it checks
 * the ghost's temp_mode instead of mode member
 *
 * @param ghost pointer to ghost to move
 * @param pacman pointer to current game's pacman
 */
void move_timed_ghost(Ghost * ghost, Pacman * pacman);

/**
 * @brief Moves any user controlled ghost, when the pacman is energizes
 *
 * This function is similar to the move_ghost_user() function,
 * but this one ensures the ghost's image will be that of an escaping ghost.
 *
 * @param ghost pointer to ghost to move
 */
void move_ghost_user_esc(Ghost * ghost);

/**
 * @brief Determines pacman's direction
 *
 * This function determines the relative direction of the pacman to the ghost,
 * according to the axels defined in constants. This is useful for both
 * chase and evade modes
 *
 * @param ghost pointer to ghost to analyse
 * @param pacman pointer to current game's pacman
 * @return pacman's direction according to constants defined above
 */
int get_pacman_dir(Ghost * ghost, Pacman * pacman);

/**
 * @brief Checks if a certain point is in the ghost
 *
 * @param ghost pointer to ghost to check
 * @param x_click x coordinate of the point
 * @param y_click y coordinate of the point
 * @return 1 if point is in ghost, 0 otherwise
 */
int is_in_ghost(Ghost * ghost, int x_click, int y_click);

/**
 * @brief Switches some ghosts to their previous modes
 *
 * This function is used when the mouse is clicked, to determine
 * how ghost's mode reacts to it. Basically, all ghosts but ghosts[exception]
 * return to their previous modes, paying attention to wether they were functioning
 * normally or evading the pacman. This is very usefull when the user clicks a ghost
 * or an empty space
 *
 * @param ghosts array with the 4 ghosts
 * @param exception index for ghost whose mode must remain the same (<0 or >3 if no exception must exist)
 */
void switch_ghosts_to_auto(Ghost *ghosts[], int exception);

/**
 * @brief Analyses user controlled ghosts when a key is pressed or release
 *
 * If there is any user controlled ghost, it's desired direction is
 * changed according to the scan_code passed as argument
 *
 * @param ghosts array with the 4 ghosts
 * @param scan_code scan code of key
 */
void check_user_ghosts(Ghost *ghosts[], unsigned long scan_code);

/**
 * @brief Changes a ghost's desired direction according to a key make code
 *
 * @param ghost whose direction must be changed
 * @param scan_code scan code of key
 */
void ghost_change_desired_direction(Ghost *ghost, unsigned long scan_code);

/**
 * @brief Tries to rotate a ghost to it's desired direction
 *
 * Used only for user controlled ghosts, so that they act upon
 * user commands similarly to the pacman
 *
 * @param ghost to act upon
 */
void ghost_try_rotate(Ghost * ghost);

/**
 * @brief Modify ghosts when pacman is energized
 *
 * This function is called when pacman eats an energizer, putting
 * the ghosts in the correct evade modes, since ghosts can be acting
 * on their own or user controlled. It also sets the ghost's escape_timer
 * to time seconds
 *
 * @param ghosts array with the 4 ghosts
 * @param time ammount of seconds ghosts must be escaping
 */
void all_ghosts_escape(Ghost * ghosts[], int time);

/**
 * @brief Decrement ghost's escape timer
 *
 * Checks if a second has passed to decrement (or not) the
 * ghost's escape timer
 *
 * @param ghosts array with the 4 ghosts
 */
void all_ghosts_escape_tick(Ghost * ghosts[]);

/**
 * @brief Process all ghost's spawn timers
 *
 * Analyses each ghost's spawn timer, decrementing them
 * every second and making them "appear" when it is over
 *
 * @param ghosts array with the 4 ghosts
 */
void all_ghosts_spawn_timer(Ghost * ghosts[]);

/**
 * @brief Resets all ghosts to their original states (when pacman dies)
 *
 * @param ghosts array with the 4 ghosts
 */
void reset_all_ghosts(Ghost * ghosts[]);

/**
 * @brief Resets a specific ghost to it's original state
 *
 * @param ghost pointer to ghost to reset
 */
void reset_ghost(Ghost * ghost);

/**
 * @brief Makes all necessary changes when a ghost is eaten
 *
 * @param ghost pointer to eaten ghost
 */
void ghost_eaten(Ghost * ghost);

/**
 * @brief Checks if mouse has clicked any ghost
 *
 * If mouse is pressing any ghost, it changes that ghost's status, making it
 * become user controlled
 *
 * @param ghosts array with 4 ghosts
 * @param mouse Mouse_coord representing current game's mouse
 */
void check_for_click(Ghost *ghosts[], Mouse_coord *mouse);





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////// BONUS FUNCTIONS ////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**@brief Struct representing a Bonus
 *
 * This struct represents the fruit bonuses that sometimes appear
 * at the pacman's spawn place. Information about the score value of the bonuses,
 * their active/inactive status, time to spawn and duration is stored here.
 * A Bonus is only consumable when it is active. When it is active and is consumed,
 * It becomes inactive and the spawn counter is reset
 */
typedef struct {
	int score;			///< Score player gets for eating the fruits
	Sprite *sp;			///< Image of the bonus (one of the available fruits)
	int active;			///< Status of the bonus. 0 - invisible(inactive), 1 - visible(active)
	int spawn_timer;	///< Time it takes for the bonus to appear once it is inactive
	int spawn;			///< Current counter for bonus appearing
	int duration;		///< Duration when active
	int curr_duration;	///< Current duration counter
} Bonus;

/**
 * @brief Creates a bonus object
 *
 *	The created object starts unavailable, appearing after "spawn_timer" seconds
 *
 * @param xi x coordinate for the bonus
 * @param yi y coordinate for the bonus
 * @param score score value of the bonus (increment on player's score when he consumes the bonus)
 * @param spawn_timer ammount of seconds the bonus stays inactive
 * @param duration ammount of seconds the bonus stays active (unless it is consumed)
 * @return pointer to Bonus object created
 */
Bonus * bonus_init(int xi, int yi, int score, int spawn_timer, int duration);

/**
 * @brief Acts on the bonus when a second has passed
 *
 * This function decrements the current counter on the Bonus, switching between modes when any
 * of the counters reaches zero. When the Bonus's status changes from inactive to active,
 * it randomly chooses an image for the Bonus (between those available)
 *
 * @param bonus pointer to Bonus to act upon
 */
void bonus_timer_tick(Bonus * bonus);

/**
 * @brief Draws the bonus if it is active
 *
 * @param bonus pointer to Bonus to draw
 */
void draw_bonus(Bonus * bonus);

/**
 * @brief Resets a Bonus
 *
 * This makes the Bonus inactive and restarts it's spawn counter
 *
 * @param bonus pointer to Bonus to act reset
 */
void reset_bonus(Bonus * bonus);




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// AUXILIARY FUNCTIONS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/**
 * @brief Check all borders of a rectangle for obstacles
 *
 * This function checks if there are any obstacles two pixels in front of all
 * the faces of a rectangle (representing a picture). It changes the array passed
 * as argument (passed by reference). This array will become to have 4 integers, representing
 * each side of the rectangle (0 - down, 1 - right, ...). If a value is 0, there are no
 * obstacles in that direction, 1 means the direction is blocked
 *
 * @param xi x coordinate of upper left corner of rectangle
 * @param yi y coordinate of upper left corner of rectangle
 * @param width rectangle width
 * @param height rectangle height
 * @param sides array to be changed
 */
void check_all_surroundings(int xi, int yi, int width, int height, int sides[]);

/**
 * @brief Draws the pacman's lives
 *
 * Draws pacman symbols in the coordinates specified for the right symbol (other go left of it).
 * It draws as many symbols as lives given
 *
 * @param lives number of lives to draw
 * @param xi x coordinate for right symbol
 * @param yi y coordinate for right symbol
 */
void draw_lives(int lives, int xi, int yi);

/**
 * @brief Returns random integer between a and b (including both)
 *
 * @param a lower interval value
 * @param b top interval value
 * @return random integer calculated
 */
int rand_integer_between(int a, int b);

/**
 * @brief Calculates the probability for percentage
 *
 * Calculates a random integer between 0 and 100, and checks if
 * that integer is lower than percentage, returning true in that case.
 * Basically, it has a probability of "percentage" to return true
 *
 * @param percentage value of percentage to calculate
 * @return 1 if true, 0 otherwise
 */
int probability(int percentage);

/**
 * @brief Returns the previous direction to dir, counter-clockwise
 *
 * @param direction given
 * @return value of previous direction (counter-clockwise)
 */
int prev_revclock_dir(int dir);

/**
 * @brief Returns the next direction to dir, counter-clockwise
 *
 * @param direction given
 * @return value of next direction (counter-clockwise)
 */
int next_revclock_dir(int dir);

/**
 * @brief Checks if pacman and any ghosts are colliding
 *
 * This function determines if the pacman and any of the ghosts are colliding,
 * returning the index of the ghost colliding in the array (or -1 if there are
 * no collisions)
 *
 * @param ghosts array with the 4 ghosts
 * @param pacman pointer to current game's pacman
 * @return 0-3 if ghosts[return] has collided with pacman, -1 otherwise
 */
int check_collisions(Ghost *ghosts[], Pacman * pacman);

/**
 * @brief Returns 1 if the Pacman has eaten an active bonus
 *
 * @param pacman pointer to game's pacman
 * @param bonus pointer to current bonus
 * @return 1 if bonus is active and "colliding" with pacman, 0 otherwise
 */
int check_eat_bonus(Pacman * pacman, Bonus * bonus);

/**
 * @brief Returns the distance between two sprites
 *
 * Calculates the distance between the center points of two Sprites
 *
 * @param sp1 pointer to first sprite
 * @param sp2 pointer to second sprite
 * @return distance between them in pixels
 */
double get_dist(Sprite *sp1, Sprite *sp2);

/**
 * @brief Empties the keyboard buffer to ensure no interrupt was left forgotten
 */
void empty_buf();


#endif /*__PACNIX_H */
