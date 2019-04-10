#include "i2c_emu.h"
#define I2C_SCL_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define I2C_SCL_SET	GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define I2C_SDA_CLR	GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define I2C_SDA_SET	GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define I2C_READ_BIT GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)

static GPIO_InitTypeDef GPIO_InitStructure;

void i2c_emu_GPIO_cfg(void)
{
	//GPIOB1: CLK
	//GPIOA5: D/C
	//GPIOA6: RST
	//GPIOA6: DAT
		
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void delay_us(uint16_t time)
{
	uint8_t i = 19;//1/8e6*40=5us
	while(time--)
	{
		while(i--);
	}
}

void i2c_start(void)
{
	
	I2C_SDA_SET;
	I2C_SCL_SET;
	delay_us(1);
	I2C_SDA_CLR;
	delay_us(1);
}

void i2c_stop(void)
{
	
	I2C_SDA_CLR;
	I2C_SCL_SET;
	delay_us(1);
	I2C_SDA_SET;
	delay_us(1);
}

void i2c_ack(void)
{
	
	I2C_SCL_CLR;
	delay_us(1);
	I2C_SDA_CLR;
	delay_us(1);
	I2C_SCL_SET;
	delay_us(1);
	I2C_SCL_CLR;
	delay_us(1);
	I2C_SDA_SET;
}

void i2c_noack(void)
{
	
	I2C_SCL_CLR;
	delay_us(1);
	I2C_SDA_SET;
	delay_us(1);
	I2C_SCL_SET;
	delay_us(1);
	I2C_SCL_CLR;
	delay_us(1);
}

uint8_t i2c_waitack(void)
{
	uint8_t re;
	
	I2C_SDA_SET;
	delay_us(1);
	I2C_SCL_SET;
	delay_us(1);
	if(I2C_READ_BIT)
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_CLR;
	delay_us(1);
	return re;
}

uint8_t i2c_read_byte(void)
{
	uint8_t i;
	uint8_t value;
	value = 0;
	
	for(i=0;i<8;i++)
	{
		value <<= 1;
		I2C_SCL_CLR;
		I2C_SDA_SET;
		delay_us(1);
		I2C_SCL_SET;	
		if(I2C_READ_BIT)
		{
			value++;
		}
		delay_us(1);
	}
	return value;
}

void i2c_write_byte(uint8_t data)
{
	uint8_t i;

	I2C_SCL_CLR;
	delay_us(1);
	for(i=0;i<8;i++)
	{
		if(data & 0x80)
		{
			I2C_SDA_SET;
		}
		else
		{
			I2C_SDA_CLR;
		}
		delay_us(1);
		I2C_SCL_SET;
		delay_us(1);
		I2C_SCL_CLR;
		//delay_us(1);
		data <<= 1;
	}
}

