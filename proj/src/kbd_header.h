#ifndef _LCOM_I8042_KBD
#define _LCOM_I8042_KBD

#define KBD_IRQ 1

// Keyboard make-codes
#define DOWN_ARROW 0x50
#define RIGHT_ARROW 0x4D
#define UP_ARROW 0x48
#define LEFT_ARROW 0x4B

#define A_KEY 0x1E
#define B_KEY 0x30
#define C_KEY 0x2E
#define D_KEY 0x20
#define E_KEY 0x12
#define F_KEY 0x21
#define G_KEY 0x22
#define H_KEY 0x23
#define I_KEY 0x17
#define J_KEY 0x24
#define K_KEY 0x25
#define L_KEY 0x26
#define M_KEY 0x32
#define N_KEY 0x31
#define O_KEY 0x18
#define P_KEY 0x19
#define Q_KEY 0x10
#define R_KEY 0x13
#define S_KEY 0x1F
#define T_KEY 0x14
#define U_KEY 0x16
#define V_KEY 0x2F
#define W_KEY 0x11
#define X_KEY 0x2D
#define Y_KEY 0x15
#define Z_KEY 0x2C

#define KBD_OUT_BUF 0x60
#define KBD_IN_BUF 0x60
#define KBC_CMD_REG 0x64
#define STAT_REG 0x64

#define BIT(n) (0x01<<(n))
#define BREAK_CODE BIT(7)

#define ESC_break 0x81

#define SET_RESET_CMD 0xED
#define READ_CMD_BYTE 0x20
#define WRITE_CMD_BYTE 0x60

#define TWO_BYTE_SCAN 0xE0

#define ACK 0xFA
#define RESEND 0xFE
#define ERR 0xFC

#define DELAY_US    20000

#endif
