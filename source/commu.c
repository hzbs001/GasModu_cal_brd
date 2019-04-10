#include "stm32f10x.h"
#include "i2c_emu.h"
#include "commu.h"
const uint16_t pos = 192;
union pfloat{
	float a;
	uint8_t b[4];
};

static GPIO_InitTypeDef GPIO_InitStructure;
static uint8_t len1ch = 21;

void op_module(uint16_t *pbuf,uint16_t *pholding)
{
	union pfloat temp;
	static uint32_t iconv = 0x00000001;
	static uint16_t index = 0 ;
	static uint8_t ch_flag = 0;
	uint8_t i = 0;
	
	if(*(pholding+pos+ch_flag*15)==0x0F0F)
	{
		i2c_start();
		i2c_write_byte(0x60);
		if(i2c_waitack()==0)
		{
			i2c_write_byte(0x0F);
			i2c_waitack();
			for(i=0;i<15;i++)
			{
				i2c_write_byte(*(pholding+pos+ch_flag*15+i)>>8);
				i2c_waitack();
				i2c_write_byte(*(pholding+pos+ch_flag*15+i)&0xFF);
				i2c_waitack();
			}
			/****write Flash command***/
			i2c_write_byte(0x99);
			i2c_waitack();
			i2c_write_byte(0x99);
			i2c_waitack();/*********/

			i2c_stop();
		}
	}
	/*i2c_start();
	i2c_write_byte(0x60);
	i2c_waitack();
	i2c_write_byte(0x02);
	i2c_waitack();
	i2c_write_byte(0x41);
	i2c_waitack();
	i2c_write_byte(0xf4);
	i2c_waitack();
	i2c_write_byte(0xa3);
	i2c_waitack();
	i2c_write_byte(0xd7);
	i2c_waitack();
	
	i2c_write_byte(0x41);
	i2c_waitack();
	i2c_write_byte(0xf4);
	i2c_waitack();
	i2c_write_byte(0xa3);
	i2c_waitack();
	i2c_write_byte(0xd7);
	i2c_waitack();
	
	i2c_stop();*/
	
	i2c_start();
	i2c_write_byte(0x60);
	if(i2c_waitack()==0)
	{
		//i2c_waitack();
		//Get ready for reading 0-6
		i2c_write_byte(0x00);
		i2c_waitack();
		i2c_stop();

		i2c_start();
		i2c_write_byte(0x61);
		i2c_waitack();

		*(pbuf+index) = i2c_read_byte();
		i2c_ack();
		*(pbuf+index) <<= 8;
		*(pbuf+index) |= i2c_read_byte();
		i2c_ack();
		
		for(i=0;i<5;i++)
		{
			index++;
			*(pbuf+index) = i2c_read_byte();
			i2c_ack();
			*(pbuf+index) <<= 8;
			*(pbuf+index) |= i2c_read_byte();
			i2c_ack();
		}
		
		index++;
		*(pbuf+index) = i2c_read_byte();
		i2c_ack();
		*(pbuf+index) <<= 8;
		*(pbuf+index) |= i2c_read_byte();
		i2c_ack();
		//i2c_stop();
		
		//skip 9 regs
		for(i=0;i<9;i++)
		{
			i2c_read_byte();
			i2c_ack();
			i2c_read_byte();
			i2c_ack();
		}
		/*i2c_start();
		i2c_write_byte(0x60);
		i2c_waitack();
		i2c_write_byte(0x20);
		i2c_waitack();
		i2c_stop();
		
		i2c_start();
		i2c_write_byte(0x61);
		i2c_waitack();*/
		
		for(i=0;i<13;i++)
		{
			index++;
			*(pbuf+index) = i2c_read_byte();
			i2c_ack();
			*(pbuf+index) <<= 8;
			*(pbuf+index) |= i2c_read_byte();
			i2c_ack();
		}
		
		index++;
		*(pbuf+index) = i2c_read_byte();
		i2c_ack();
		*(pbuf+index) <<= 8;
		*(pbuf+index) |= i2c_read_byte();
		i2c_ack();
		i2c_stop();

	}
	else
	{

		for(i=0;i<len1ch-1;i++)
		{
		*(pbuf+index) = 0x0000;
		index++;
		}
		*(pbuf+index) = 0x0000;
	}
	
	(index==(len1ch*32-1))?(iconv=0x00000001):(iconv <<= 1);
	(index==(len1ch*32-1))?(index=0):(index++);
	(ch_flag==31)?(ch_flag=0):(ch_flag++);
	
	(iconv&0x00000001)?GPIO_SetBits(GPIOE,GPIO_Pin_8):GPIO_ResetBits(GPIOE,GPIO_Pin_8);
	(iconv&0x00000002)?GPIO_SetBits(GPIOE,GPIO_Pin_9):GPIO_ResetBits(GPIOE,GPIO_Pin_9);
	(iconv&0x00000004)?GPIO_SetBits(GPIOE,GPIO_Pin_10):GPIO_ResetBits(GPIOE,GPIO_Pin_10);
	(iconv&0x00000008)?GPIO_SetBits(GPIOE,GPIO_Pin_11):GPIO_ResetBits(GPIOE,GPIO_Pin_11);
	(iconv&0x00000010)?GPIO_SetBits(GPIOE,GPIO_Pin_12):GPIO_ResetBits(GPIOE,GPIO_Pin_12);
	(iconv&0x00000020)?GPIO_SetBits(GPIOE,GPIO_Pin_13):GPIO_ResetBits(GPIOE,GPIO_Pin_13);
	(iconv&0x00000040)?GPIO_SetBits(GPIOE,GPIO_Pin_14):GPIO_ResetBits(GPIOE,GPIO_Pin_14);
	(iconv&0x00000080)?GPIO_SetBits(GPIOE,GPIO_Pin_15):GPIO_ResetBits(GPIOE,GPIO_Pin_15);
	
	(iconv&0x00000100)?GPIO_SetBits(GPIOD,GPIO_Pin_8):GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	(iconv&0x00000200)?GPIO_SetBits(GPIOD,GPIO_Pin_9):GPIO_ResetBits(GPIOD,GPIO_Pin_9);
	(iconv&0x00000400)?GPIO_SetBits(GPIOD,GPIO_Pin_10):GPIO_ResetBits(GPIOD,GPIO_Pin_10);
	(iconv&0x00000800)?GPIO_SetBits(GPIOD,GPIO_Pin_11):GPIO_ResetBits(GPIOD,GPIO_Pin_11);
	(iconv&0x00001000)?GPIO_SetBits(GPIOD,GPIO_Pin_12):GPIO_ResetBits(GPIOD,GPIO_Pin_12);
	(iconv&0x00002000)?GPIO_SetBits(GPIOD,GPIO_Pin_13):GPIO_ResetBits(GPIOD,GPIO_Pin_13);
	(iconv&0x00004000)?GPIO_SetBits(GPIOD,GPIO_Pin_14):GPIO_ResetBits(GPIOD,GPIO_Pin_14);
	(iconv&0x00008000)?GPIO_SetBits(GPIOD,GPIO_Pin_15):GPIO_ResetBits(GPIOD,GPIO_Pin_15);
	
	(iconv&0x00010000)?GPIO_SetBits(GPIOC,GPIO_Pin_6):GPIO_ResetBits(GPIOC,GPIO_Pin_6);
	(iconv&0x00020000)?GPIO_SetBits(GPIOC,GPIO_Pin_7):GPIO_ResetBits(GPIOC,GPIO_Pin_7);
	(iconv&0x00040000)?GPIO_SetBits(GPIOC,GPIO_Pin_8):GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	(iconv&0x00080000)?GPIO_SetBits(GPIOC,GPIO_Pin_9):GPIO_ResetBits(GPIOC,GPIO_Pin_9);
	(iconv&0x00100000)?GPIO_SetBits(GPIOA,GPIO_Pin_12):GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	(iconv&0x00200000)?GPIO_SetBits(GPIOC,GPIO_Pin_10):GPIO_ResetBits(GPIOC,GPIO_Pin_10);
	(iconv&0x00400000)?GPIO_SetBits(GPIOC,GPIO_Pin_11):GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	(iconv&0x00800000)?GPIO_SetBits(GPIOC,GPIO_Pin_12):GPIO_ResetBits(GPIOC,GPIO_Pin_12);
	
	(iconv&0x01000000)?GPIO_SetBits(GPIOD,GPIO_Pin_0):GPIO_ResetBits(GPIOD,GPIO_Pin_0);
	(iconv&0x02000000)?GPIO_SetBits(GPIOD,GPIO_Pin_1):GPIO_ResetBits(GPIOD,GPIO_Pin_1);
	(iconv&0x04000000)?GPIO_SetBits(GPIOD,GPIO_Pin_2):GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	(iconv&0x08000000)?GPIO_SetBits(GPIOD,GPIO_Pin_3):GPIO_ResetBits(GPIOD,GPIO_Pin_3);
	(iconv&0x10000000)?GPIO_SetBits(GPIOD,GPIO_Pin_4):GPIO_ResetBits(GPIOD,GPIO_Pin_4);
	(iconv&0x20000000)?GPIO_SetBits(GPIOD,GPIO_Pin_5):GPIO_ResetBits(GPIOD,GPIO_Pin_5);
	(iconv&0x40000000)?GPIO_SetBits(GPIOD,GPIO_Pin_6):GPIO_ResetBits(GPIOD,GPIO_Pin_6);
	(iconv&0x80000000)?GPIO_SetBits(GPIOD,GPIO_Pin_7):GPIO_ResetBits(GPIOD,GPIO_Pin_7);
		
}

void commu_GPIO_cfg(void)
{

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = \
		GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9|\
		GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = 0xFF00;//PE8~PE15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	/*********** Settings for ADC *****************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_SetBits(GPIOE,GPIO_Pin_8);//Initilize
}
