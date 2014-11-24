#ifndef __KBD_FUNCT_H
#define __KBD_FUNCT_H

/** @defgroup test3 test3
 * @{
 *
 * Functions for testing the kbd code
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

// Assembly function declaration
unsigned long asmHandlerKbd(unsigned long letra);

int kbd_subscribe_int();
int kbd_unsubscribe_int();

int kbd_c_handler();
int kbd_asm_handler();

int kbd_interrupts();
int toggle_led(unsigned short led);
int timed_scan(unsigned short time);
int timer_handler();

int wait_for_esc();

#endif /* __KBD_FUNCT_H */