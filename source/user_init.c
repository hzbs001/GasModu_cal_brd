#include "stm32f10x.h"
#include "user_init.h"

static NVIC_InitTypeDef NVIC_InitStructure;

void RCC_cfg()
{
	/*******Using HSI = 8MHz***********/
	/*RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_6);//HSI=8MHz,HSI/2*6
	RCC_HSEConfig(RCC_HSE_OFF);*/
	
	/****Using HSE***When select HSE, freq is much slower than expect??
	RCC_HSICmd(DISABLE);
	RCC_HSEConfig(RCC_HSE_ON);
	RCC_WaitForHSEStartUp();
	RCC_PREDIV1Config(RCC_PREDIV1_Div2);//12MHz/2=6MHz
	RCC_PLLConfig(RCC_PLLSource_PREDIV1,RCC_PLLMul_4);//4*6=24MHz*/
	RCC_HSEConfig(RCC_HSE_OFF);
	
	RCC_PLLCmd(DISABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI); //sysclk=8MHz
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 	//Hclk=8MHz
	RCC_PCLK1Config(RCC_HCLK_Div2); 		//Pclk=4MHz*/
	RCC_PCLK2Config(RCC_HCLK_Div2); 		//Pclk=4MHz*/
	
	/*RCC_HSEConfig(RCC_HSE_OFF);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_3);//HSI=8MHz,HSI/2*3
	RCC_PLLCmd(ENABLE);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //sysclk=12MHz
	RCC_HCLKConfig(RCC_SYSCLK_Div2); 	//Hclk=6MHz
	RCC_PCLKConfig(RCC_HCLK_Div1); 		//Pclk=6MHz*/
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); //ADC_clk=1MHz
	
	/****
	 *******By lwang,2019/01/19*************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_ADC1 | \
												 RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | \
												 RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | \
												 RCC_APB2Periph_GPIOE , ENABLE);
	
}
void NVIC_cfg()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	/*************For DMA1 *******************/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	/*************For Metronome *******************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	/*************For modbus***********************/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

