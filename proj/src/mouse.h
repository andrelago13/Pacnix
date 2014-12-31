#ifndef __MOUSE_H
#define __MOUSE_H

#include "sprite.h"

/** @defgroup mouse mouse
 * @brief Functions to manage mouse interaction
 */

/**
 * @brief To test packet reception via interrupts
 *
 * Displays the packets received from the mouse
 * Exits after receiving the number of packets received in argument
 *
 * @param cnt Number of packets to receive and display before exiting
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int test_packet(unsigned short cnt);

/**
 * @brief To test handling of more than one interrupt
 *
 *  Similar test_packet() except that it
 *  should terminate if no packets are received for idle_time seconds
 *
 * @param idle_time Number of seconds without packets before exiting
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int test_async(unsigned short idle_time);

/**
 * @brief To test reading and parsing of configuration
 *
 *  Reads mouse configuration and displays it in a human-friendly way
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int test_config(void);

/**
 * @brief Subscribes mouse interrupts to global variable hook
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_mouse_subscribe_int();

/**
 * @brief Unsubscribes mouse interrupts on global variable hook
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_mouse_unsubscribe_int();

/**
 * @brief Subscribes mouse interrupts to specified hook
 *
 * @param hook pointer to hook for interrupt subscription
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_subscribe(unsigned int *hook);

/**
 * @brief Unsubscribes mouse interrupts on specified hook
 *
 * @param hook pointer to hook on whom interrupts were subscribed
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int mouse_unsubscribe(unsigned int *hook);


/**
 * @brief Read "packets" complete mouse interrupt packets
 *
 * @param packets number of complete packets to read
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int interrupt_cycle(int packets);

/**
 * @brief Reads and prints complete 3 byte mouse packets
 *
 * @return Return 1 when third and final byte was read and printed
 */
int mouse_handler();


/**
 * @brief Sets mouse stream mode (enabling packets)
 */
void set_stream();

/**
 * @brief Disables mouse stream mode (disabling packets)
 */
void dis_stream();


/**
 * @brief Read complete mouse packets until mouse is idle for sec seconds
 *
 * @param sec number of idle seconds to wait for new mouse interrupt
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timed_mouse(unsigned short sec);

/**
 * @brief Timer interrupt handler for use with timed_mouse()
 *
 * @return Return 1 when a second has passed, 0 otherwise
 */
int timer_handler_mouse();

/**
 * @brief Reads mouse configuration, calling print_config() to print it
 */
void read_config();

/**
 * @brief Prints mouse configuration given as argument
 *
 * @param status array with 3 bytes of mouse configuration
 */
void print_config(unsigned char status[]);


/**@brief Struct representing a complete 3 byte packet
 *
 * Contains the information of 3 mouse packets, representing the
 * status of the mouse after an interrupt
 */
typedef struct
{
	int x_delta, y_delta;	///< x and y displacement of mouse since the last interrupt
	int x_ovf, y_ovf;		///< signal x or y overflow since last interrupt
	int mb, lb, rb;			///< status of the mouse buttons (0-released, 1-pressed)
} Mouse_packet;

/**@brief Struct representing a mouse cursor
 */
typedef struct
{
	int x_coord, y_coord;	///< x and y coordinates of the cursor
	int mb, lb, rb;			///< status of the mouse buttons (0-released, 1-pressed)
	Sprite img;				///< image of the cursor
} Mouse_coord;

/**
 * @brief Reads a 3 byte mouse packet upon separated interrupts
 *
 * Is called as a handler for mouse interrupts, modifying the
 * Mouse_packet given when the third and last byte was read
 * It ignores the first x bytes read (specified in a global variable)
 * since first couple of packets are often incorrect
 *
 * @param mouse pointer to Mouse_packet to modify
 * @return 1 upon third and last byte, 0 otherwise
 */
int mouse_read_packet(Mouse_packet *mouse);

/**
 * @brief Updates mouse coordinates and image upon packet read
 *
 * @param mouse pointer to mouse to update
 * @param delta pointer to packet to update mouse with
 */
void update_mouse(Mouse_coord *mouse, Mouse_packet *delta);

/**
 * @brief Tells the mouse_read_packet() function to ignore the following 9 bytes
 */
void reset_mouse_packets();

#endif /* __MOUSE_H */
