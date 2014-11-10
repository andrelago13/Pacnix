#ifndef _LCOM_I8042_KBD
#define _LCOM_I8042_KBD

#define KBD_IRQ 1

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

// Commands for the mouse

#define MOUSE_IRQ 12

#define DIS_DATA_REP 0XF5   // If it's on stream mode, the host must use this cmd (
#define EN_DATA_REP 0XF4

#define RESET 0XFF

#define SET_DEFAU 0XF6		   //BEWARE because enters on stream mode
#define SET_SAMPLE_RAT 0XF3
#define SET_REMOTE_MOD 0XF0		//Enters in remote mode
#define SER_WRAP_MOD 0XEE		//Enters in wrap mode
#define RESET_WRAP_MOD 0XEC		//Enters in stream mode or remote mode

#define READ_DATA 0XEB			//Only way to read data in remote mode

#define SET_STREAM_MOD 0XEA			//Enters on stream mode
#define DIS_STREAM_MODE 0xF5
#define STATUS_REQ 0XE9			//Sends the following 3-byte status packet

#define GET_DEVICE_ID 0XF2		//0X00 for standard PS/2 mouse
#define SET_SCALL_2_1 0XE7
#define SET_SCALL_1_1 0XE6
#define SET_RESOL 0XE8

#define IBF BIT(1)

#define WRITE_MOUSE 0xD4

#define DISABLE_MOUSE 0xA7
#define ENABLE_MOUSE 0xA8

#define Y_OVF BIT(7)
#define X_OVF BIT(6)
#define Y_SIGN BIT(5)
#define X_SIGN BIT(4)
#define PACKET_BYTE_1 BIT(3)
#define MB BIT(2)
#define RB BIT(1)
#define LB BIT(0)

#define SIGNED 0xFF00

#define REMOTE BIT(6)
#define ENABLE BIT(5)
#define SCALING BIT(4)
#define RESOLUTION BIT(1) | BIT(0)
#define SAMPLE_RATE 0xFF

// TEST
#define DIS_MOUSE_CMD BIT(5)
#define EN_MOUSE_CMD BIT(1)

#define NEGATIVE 0xFF00
#define POSITIVE 0x00FF


#endif
