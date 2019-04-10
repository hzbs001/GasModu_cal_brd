//#include "stdint.h"
#include "stm32f10x.h"

#include "user_init.h"

#include "i2c_emu.h"
#include "commu.h"

#include "mbport.h"
#include "mb.h"

#include "metronome.h"

#define TC_CAL1            ((uint16_t *)0x1FFFF7B8)
#define VREFINT_CAL            ((uint16_t *)0x1FFFF7BA)

/* ----------------------- Defines ------------------------------------------*/
const uint8_t REG_INPUT_START = 0;
//const uint8_t REG_INPUT_NREGS = 8;
const uint8_t REG_HOLDING_START = 0;
//const uint8_t REG_HOLDING_NREGS = 16;
float T,H;
/* ----------------------- Static variables ---------------------------------*/
static uint16_t   usRegInputStart = REG_INPUT_START;
static uint16_t   usRegInputBuf[REG_INPUT_NREGS];

static uint16_t   usRegHoldingStart = REG_HOLDING_START;
static uint16_t   usRegHoldingBuf[REG_HOLDING_NREGS];

/* ----------------------- Start implementation -----------------------------*/
int main( void )
{
	static uint16_t tmp1,tmp2,i;
	static int16_t sens_coff[8];
	static int16_t temp_coff[8];
	static int16_t humi_coff[4];
	static uint16_t gain,rs,rl;
	static float f1,v0;

	//clock settings and interrupt management
	RCC_cfg();
	NVIC_cfg();
	
	//metronome
	mtn_TIM_cfg();
	
	commu_GPIO_cfg();
	i2c_emu_GPIO_cfg();
	//For Modbus
	mb_TIM_cfg();
	mb_USART_cfg();
	//led_GPIO_cfg();
	
	setMBAddr(88);
	
	usRegHoldingBuf[35] = 0x5858;
	//usRegInputBuf[1] = 0x8989;
  while(1)
  {
		mb_Service();		//modbus service
		
		//adcDataProcess(usRegHoldingBuf);
		if(time2tell())
		{
			op_module(usRegInputBuf,usRegHoldingBuf);
		}
  }
}

void mbRegHolding(uint8_t *p,uint16_t regAddr,uint16_t regNum,RD_WR mode)
{
	uint16_t i;
	uint16_t iRegIndex;
	if((regAddr >= REG_HOLDING_START) && ((regAddr+regNum) <= (REG_HOLDING_START+REG_HOLDING_NREGS)))
	{
		iRegIndex = (uint16_t)(regAddr-usRegHoldingStart);
		
		switch(mode)
		{
			case READ:
			{
				for(i=0;i<regNum;i++)
				{
					*p++ = (uint8_t)(usRegHoldingBuf[iRegIndex+i] >> 8 );  //High byte first
					*p++ = (uint8_t)(usRegHoldingBuf[iRegIndex+i] & 0xFF); //low byte first
				}
			}
			break;
			case WRITE:
			{
				for(i=0;i<regNum;i++)
				{
						*(usRegHoldingBuf+iRegIndex+i)= ((*p++))<<8; 	//High byte
						*(usRegHoldingBuf+iRegIndex+i)|= *(p++); 			//Low byte
				}

			}	
		}
	}
}

void mbRegInput(uint8_t *p,uint16_t regAddr,uint16_t regNum)
{
	uint16_t i;
	uint16_t iRegIndex;
	if((regAddr >= REG_INPUT_START) && ((regAddr+regNum) <= (REG_INPUT_START+REG_INPUT_NREGS)))
	{
		iRegIndex = (uint16_t)(regAddr-usRegInputStart);
		for(i=0;i<regNum;i++)
		{
			*p++ = (uint8_t)(usRegInputBuf[iRegIndex+i] >> 8 );  //High byte first
			*p++ = (uint8_t)(usRegInputBuf[iRegIndex+i] & 0xFF); //low byte first
		}
	}
}
/*void updateReg(void)
{
	usRegHoldingBuf[31] += 1;
}

void updateReg2(void)
{
	usRegHoldingBuf[31] += 2;
}*/

void updateADCValue(uint16_t value,uint8_t ich)
{
	*(usRegHoldingBuf+ich) = value;
}
