#define LEFT_MOTOR 0xA0				//Left motor's PIC i2c address
#define RIGHT_MOTOR 0x0A				//Right motor's PIC i2c address
#define STD_CMD_LENGTH 5 			//Command's length
#define GO_FLAG 0xF0
#define BREAK_FLAG 0x0F
#define DONE_FLAG 0xFF

typedef enum {NOTHING, CONTROL_READ,
              ADDRESS_READ, READ_COMMAND_READ} I2C_STATE;