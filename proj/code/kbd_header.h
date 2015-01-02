#ifndef _LCOM_I8042_KBD
#define _LCOM_I8042_KBD

#define KBD_IRQ 1

// Keyboard make-codes
#define DOWN_ARROW 0x50		/**< @brief down arrow make-code */
#define RIGHT_ARROW 0x4D	/**< @brief right arrow make-code */
#define UP_ARROW 0x48		/**< @brief up arrow make-code */
#define LEFT_ARROW 0x4B		/**< @brief left arrow make-code */

#define A_KEY 0x1E	/**< @brief A make-code */
#define B_KEY 0x30	/**< @brief B make-code */
#define C_KEY 0x2E	/**< @brief C make-code */
#define D_KEY 0x20	/**< @brief D make-code */
#define E_KEY 0x12	/**< @brief E make-code */
#define F_KEY 0x21	/**< @brief F make-code */
#define G_KEY 0x22	/**< @brief G make-code */
#define H_KEY 0x23	/**< @brief H make-code */
#define I_KEY 0x17	/**< @brief I make-code */
#define J_KEY 0x24	/**< @brief J make-code */
#define K_KEY 0x25	/**< @brief K make-code */
#define L_KEY 0x26	/**< @brief L make-code */
#define M_KEY 0x32	/**< @brief M make-code */
#define N_KEY 0x31	/**< @brief N make-code */
#define O_KEY 0x18	/**< @brief O make-code */
#define P_KEY 0x19	/**< @brief P make-code */
#define Q_KEY 0x10	/**< @brief Q make-code */
#define R_KEY 0x13	/**< @brief R make-code */
#define S_KEY 0x1F	/**< @brief S make-code */
#define T_KEY 0x14	/**< @brief T make-code */
#define U_KEY 0x16	/**< @brief U make-code */
#define V_KEY 0x2F	/**< @brief V make-code */
#define W_KEY 0x11	/**< @brief W make-code */
#define X_KEY 0x2D	/**< @brief X make-code */
#define Y_KEY 0x15	/**< @brief Y make-code */
#define Z_KEY 0x2C	/**< @brief Z make-code */

#define ENTER_KEY 0x1C	/**< @brief enter make-code */

#define KBD_OUT_BUF 0x60	/**< @brief port for keyboard OUT_BUF */
#define KBD_IN_BUF 0x60		/**< @brief port for keyboard IN_BUF */
#define KBC_CMD_REG 0x64	/**< @brief port for keyboard controller command register */
#define STAT_REG 0x64		/**< @brief port for keyboard controller status register */

#define BIT(n) (0x01<<(n))	/**< @brief 2^n */
#define BREAK_CODE BIT(7)	/**< @brief 2^7, all break_codes have bit 7 set */

#define ESC_break 0x81	/**< @brief esc break-code */

#define SET_RESET_CMD 0xED
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60

#define TWO_BYTE_SCAN 0xE0

#define ACK 0xFA
#define RESEND 0xFE
#define ERR 0xFC

#define DELAY_US    20000

#endif
