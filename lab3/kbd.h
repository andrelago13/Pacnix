#ifndef _LCOM_I8042_KBD
#define _LCOM_I8042_KBD

#define KBD_IRQ 1

#define KBD_OUT_BUF 0x60
#define KBC_CMD_REG 0x64

#define BIT(n) (0x01<<(n))

#define ESC_break 0x81

#define SET_RESET_CMD 0xED

#endif
