#ifndef __MOUSE_H
#define __MOUSE_H

/** @defgroup test4 test4
 * @{
 *
 * Functions for testing the kbd code
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

int kbd_mouse_subscribe_int();
int kbd_mouse_unsubscribe_int();

int interrupt_cycle(int packets);
int mouse_handler();

void set_stream();
void dis_stream();

int timed_mouse(unsigned short sec);
int timer_handler_mouse();

void read_config();
void print_config(unsigned char status[]);

#endif /* __MOUSE_H */