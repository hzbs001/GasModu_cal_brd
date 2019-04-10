#include "stm32f10x.h"

void i2c_emu_GPIO_cfg(void);
void delay_us(uint16_t time);
void i2c_start(void);
void i2c_stop(void);
void i2c_ack(void);
void i2c_noack(void);
uint8_t i2c_waitack(void);
uint8_t i2c_read_byte(void);
void i2c_write_byte(uint8_t);
