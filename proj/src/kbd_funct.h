#ifndef __KBD_FUNCT_H
#define __KBD_FUNCT_H

/** @defgroup kbd_funct kbd_funct
 * @brief Functions for interface with keyboard
 *
 * Functions for use of keyboard, related with interruption subscription
 * and handling (read scancodes, toggle leds and waiting for esc pressing)
 */

/**
 * @brief To test reading of scancode via KBD interrupts
 *
 * Displays the scancodes received from the keyboard
 * Exits upon release of the ESC key
 *
 * @param ass Which IH to use: if 0, in C, otherwise in assembly
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_scan(unsigned short ass);

/**
 * @brief To test keyboard commands
 *
 * Toggles LED indicators, one per second, according to the
 *  arguments
 *
 * @param n number of elements of the second argument
 * @param leds array with the LEDs to toggle (Ranges from 0 t0 2)
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_leds(unsigned short n, unsigned short *leds);

/**
 * @brief To test handling of more than one interrupt
 *
 *  Similar to kbd_test_scan() except that it
 *  should terminate also if no scancodes are received for n seconds
 *
 * @param n Number of seconds without scancodes before exiting
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_test_timed_scan(unsigned short n);

/**
 * @brief Assembly function declaration
 */
unsigned long asmHandlerKbd(unsigned long letra);

/**
 * @brief Subscribes keyboard interrupts to global variable hook
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribes keyboard interrupts on global hook variable
 */
int kbd_unsubscribe_int();

/**
 * @brief C handler for keyboard interrupts
 *
 *  Similar to kbd_test_scan() except that it
 *  should terminate also if no scancodes are received for n seconds
 *
 * @param n Number of seconds without scancodes before exiting
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_c_handler();

/**
 * @brief ASM handler for keyboard interrupts
 */
int kbd_asm_handler();

/**
 * @brief Driver for keyboard interrupts
 *
 * Continuously "looks for" keyboard interrupts, handling them with
 * the given handler
 *
 * @param handler() function to call to handle a keybord interrupt
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int kbd_interrupts(int handler());

/**
 * @brief Toggle the specified led
 *
 * Changes the state of a given keyboard led to the opposite of
 * the one he is currently at
 *
 * @param led 0-scroll 1-num 2-caps
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int toggle_led(unsigned short led);

/**
 * @brief Reads and displays keyboard scancodes for "time" seconds after the last interrupt
 *
 * @param time number of seconds to read scancodes for, after the last one
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int timed_scan(unsigned short time);

/**
 * @brief Timer interrupt handler for use in timed_scan()
 *
 * @return Return 1 if a second has passed, 0 otherwise
 */
int timer_handler();

/**
 * @brief Only terminates when the ESC breakcode is read
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int wait_for_esc();

#endif /* __KBD_FUNCT_H */
