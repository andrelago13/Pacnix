#ifndef _LCOM_I8042_KBD
#define _LCOM_I8042_KBD

#define KBD_IRQ 1

#define KBD_OUT_BUF 0x60
#define KBD_IN_BUF 0x60
#define KBC_CMD_REG 0x64

#define BIT(n) (0x01<<(n))
#define BREAK_CODE BIT(7)

#define ESC_break 0x81

#define SET_RESET_CMD 0xED
#define READ_CMD_BYTE 0x20

#define TWO_BYTE_SCAN 0xE0

#define ACK 0xFA
#define RESEND 0xFE
#define ERR 0xFC

#define DELAY_US    20000

#endif
