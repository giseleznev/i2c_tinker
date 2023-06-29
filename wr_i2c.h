#define I2C_WRITE 0x60
#define I2C_READ  0x61
#define I2C_BUS 1

typedef unsigned char   u8;

int i2c_init(void);

int i2c_read(u8 reg, u8 *result);

int i2c_write(u8 reg, u8 data);