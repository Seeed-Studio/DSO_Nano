/*******************************************************************************
 File name  :  HW_V1_config.h
 *******************************************************************************/
#ifndef __HWV1_CONFIG_H
#define __HWV1_CONFIG_H

#include "Interface.h"

#define TIM1_CR1    (*((vu32 *)(TIM1_BASE+0x00)))
#define TIM1_CR2    (*((vu32 *)(TIM1_BASE+0x04)))
#define TIM1_DIER   (*((vu32 *)(TIM1_BASE+0x0C)))
#define TIM1_SR     (*((vu32 *)(TIM1_BASE+0x10)))
#define TIM1_CCMR1  (*((vu32 *)(TIM1_BASE+0x18)))
#define TIM1_CCER   (*((vu32 *)(TIM1_BASE+0x20)))
#define TIM1_PSC    (*((vu32 *)(TIM1_BASE+0x28)))
#define TIM1_ARR    (*((vu32 *)(TIM1_BASE+0x2C)))
#define TIM1_RCR    (*((vu32 *)(TIM1_BASE+0x30)))
#define TIM1_CCR1   (*((vu32 *)(TIM1_BASE+0x34)))
#define TIM1_BDTR   (*((vu32 *)(TIM1_BASE+0x44)))

#define TIM2_CR1    (*((vu32 *)(TIM2_BASE+0x00)))
#define TIM2_DIER   (*((vu32 *)(TIM2_BASE+0x0C)))
#define TIM2_SR     (*((vu32 *)(TIM2_BASE+0x10)))
#define TIM2_CCMR2  (*((vu32 *)(TIM2_BASE+0x1C)))
#define TIM2_CCER   (*((vu32 *)(TIM2_BASE+0x20)))
#define TIM2_PSC    (*((vu32 *)(TIM2_BASE+0x28)))
#define TIM2_ARR    (*((vu32 *)(TIM2_BASE+0x2C)))
#define TIM2_CCR4   (*((vu32 *)(TIM2_BASE+0x40)))

#define TIM3_CR1    (*((vu32 *)(TIM3_BASE+0x00)))
#define TIM3_DIER   (*((vu32 *)(TIM3_BASE+0x0C)))
#define TIM3_SR     (*((vu32 *)(TIM3_BASE+0x10)))
#define TIM3_CCMR2  (*((vu32 *)(TIM3_BASE+0x1C)))
#define TIM3_CCER   (*((vu32 *)(TIM3_BASE+0x20)))
#define TIM3_PSC    (*((vu32 *)(TIM3_BASE+0x28)))
#define TIM3_ARR    (*((vu32 *)(TIM3_BASE+0x2C)))
#define TIM3_CCR1   (*((vu32 *)(TIM3_BASE+0x34)))
#define TIM4_CR1    (*((vu32 *)(TIM4_BASE+0x00)))
#define TIM4_DIER   (*((vu32 *)(TIM4_BASE+0x0C)))
#define TIM4_SR     (*((vu32 *)(TIM4_BASE+0x10)))
#define TIM4_CCMR1  (*((vu32 *)(TIM4_BASE+0x18)))
#define TIM4_CCMR2  (*((vu32 *)(TIM4_BASE+0x1C)))
#define TIM4_CCER   (*((vu32 *)(TIM4_BASE+0x20)))
#define TIM4_PSC    (*((vu32 *)(TIM4_BASE+0x28)))
#define TIM4_ARR    (*((vu32 *)(TIM4_BASE+0x2C)))
#define TIM4_CCR1   (*((vu32 *)(TIM4_BASE+0x34)))

#define ADC2_CR1    (*((vu32 *)(ADC2_BASE+0x04)))
#define ADC2_CR2    (*((vu32 *)(ADC2_BASE+0x08)))
#define ADC2_SMPR1  (*((vu32 *)(ADC2_BASE+0x0C)))
#define ADC2_SMPR2  (*((vu32 *)(ADC2_BASE+0x10)))
#define ADC2_SQR1   (*((vu32 *)(ADC2_BASE+0x2C)))
#define ADC2_SQR3   (*((vu32 *)(ADC2_BASE+0x34)))
#define ADC1_CR1    (*((vu32 *)(ADC1_BASE+0x04)))
#define ADC1_CR2    (*((vu32 *)(ADC1_BASE+0x08)))
#define ADC1_SMPR1  (*((vu32 *)(ADC1_BASE+0x0C)))
#define ADC1_SMPR2  (*((vu32 *)(ADC1_BASE+0x10)))
#define ADC1_SQR1   (*((vu32 *)(ADC1_BASE+0x2C)))
#define ADC1_SQR3   (*((vu32 *)(ADC1_BASE+0x34)))
#define ADC1_DR     (*((vu32 *)(ADC1_BASE+0x4C)))
#define DMA_ISR     (*((vu32 *)(DMA_BASE+0x00)))
#define DMA_IFCR    (*((vu32 *)(DMA_BASE+0x04)))
#define DMA_CCR1    (*((vu32 *)(DMA_BASE+0x08)))
#define DMA_CNDTR1  (*((vu32 *)(DMA_BASE+0x0C)))
#define DMA_CPAR1   (*((vu32 *)(DMA_BASE+0x10)))
#define DMA_CMAR1   (*((vu32 *)(DMA_BASE+0x14)))
#define DMA_CCR2    (*((vu32 *)(DMA_BASE+0x1C)))
#define DMA_CNDTR2  (*((vu32 *)(DMA_BASE+0x20)))
#define DMA_CPAR2   (*((vu32 *)(DMA_BASE+0x24)))
#define DMA_CMAR2   (*((vu32 *)(DMA_BASE+0x28)))
#define ADC1_DR_ADDR  ((u32)0x4001244C)

#define GPIOA_CRL   (*((vu32 *)(GPIOA_BASE+0x00)))
#define GPIOB_CRL   (*((vu32 *)(GPIOB_BASE+0x00)))
#define GPIOC_CRL   (*((vu32 *)(GPIOC_BASE+0x00)))
#define GPIOD_CRL   (*((vu32 *)(GPIOD_BASE+0x00)))
#define GPIOE_CRL   (*((vu32 *)(GPIOE_BASE+0x00)))

#define GPIOA_CRH   (*((vu32 *)(GPIOA_BASE+0x04)))
#define GPIOB_CRH   (*((vu32 *)(GPIOB_BASE+0x04)))
#define GPIOC_CRH   (*((vu32 *)(GPIOC_BASE+0x04)))
#define GPIOD_CRH   (*((vu32 *)(GPIOD_BASE+0x04)))
#define GPIOE_CRH   (*((vu32 *)(GPIOE_BASE+0x04)))

#define GPIOA_ODR   (*((vu32 *)(GPIOA_BASE+0x0C)))
#define GPIOB_ODR   (*((vu32 *)(GPIOB_BASE+0x0C)))
#define GPIOC_ODR   (*((vu32 *)(GPIOC_BASE+0x0C)))
#define GPIOD_ODR   (*((vu32 *)(GPIOD_BASE+0x0C)))
#define GPIOE_ODR   (*((vu32 *)(GPIOE_BASE+0x0C)))

#define GPIOA_IDR   (*((vu32 *)(GPIOA_BASE+0x08)))
#define GPIOB_IDR   (*((vu32 *)(GPIOB_BASE+0x08)))
#define GPIOC_IDR   (*((vu32 *)(GPIOC_BASE+0x08)))
#define GPIOD_IDR   (*((vu32 *)(GPIOD_BASE+0x08)))
#define GPIOE_IDR   (*((vu32 *)(GPIOE_BASE+0x08)))

#define GPIOA_BSRR  (*((vu32 *)(GPIOA_BASE+0x10)))
#define GPIOB_BSRR  (*((vu32 *)(GPIOB_BASE+0x10)))
#define GPIOC_BSRR  (*((vu32 *)(GPIOC_BASE+0x10)))
#define GPIOD_BSRR  (*((vu32 *)(GPIOD_BASE+0x10)))
#define GPIOE_BSRR  (*((vu32 *)(GPIOE_BASE+0x10)))

#define GPIOA_BRR   (*((vu32 *)(GPIOA_BASE+0x14)))
#define GPIOB_BRR   (*((vu32 *)(GPIOB_BASE+0x14)))
#define GPIOC_BRR   (*((vu32 *)(GPIOC_BASE+0x14)))
#define GPIOD_BRR   (*((vu32 *)(GPIOD_BASE+0x14)))
#define GPIOE_BRR   (*((vu32 *)(GPIOE_BASE+0x14)))

#define AFIO_MAPR   (*((vu32 *)(AFIO_BASE+0x04)))

#define SCS_BASE       ((u32)0xE000E000)
#define SysTick_BASE   (SCS_BASE + 0x0010)

#define MSD_CS_LOW()    GPIOB_BRR  = GPIO_Pin_12  //Select MSD Card
#define MSD_CS_HIGH()   GPIOB_BSRR = GPIO_Pin_12  //Deselect MSD Card

#define KEY_UP         (GPIO_Pin_6)  //GPIOA
#define KEY_DOWN       (GPIO_Pin_9)  //GPIOD
#define KEY_LEFT       (GPIO_Pin_5)  //GPIOA
#define KEY_RIGHT      (GPIO_Pin_7)  //GPIOA
#define KEY_PLAY       (GPIO_Pin_4)  //GPIOA
#define KEY_M          (GPIO_Pin_11) //GPIOD
#define KEY_B          (GPIO_Pin_3)  //GPIOA

#define LDC_DATA_OUT        GPIOE_ODR
#define LDC_DATA_INP        GPIOE_IDR
#define LCD_DATA_BUS_INP()  GPIOE_CRL = GPIOC_CRH = 0x44444444
#define LCD_DATA_BUS_OUT()  GPIOE_CRL = GPIOC_CRH = 0x33333333

#define LCD_nRST_LOW()  GPIOC_BRR  = GPIO_Pin_0
#define LCD_nRST_HIGH() GPIOC_BSRR = GPIO_Pin_0
#define LCD_RS_LOW()    GPIOD_BRR  = GPIO_Pin_1
#define LCD_RS_HIGH()   GPIOD_BSRR = GPIO_Pin_1
#define LCD_nWR_LOW()   GPIOD_BRR  = GPIO_Pin_5
#define LCD_nWR_HIGH()  GPIOD_BSRR = GPIO_Pin_5
#define LCD_nWR_ACT()   GPIOD_BSRR = GPIOD_BRR  = GPIO_Pin_5
#define LCD_nRD_LOW()   GPIOD_BRR  = GPIO_Pin_4
#define LCD_nRD_HIGH()  GPIOD_BSRR = GPIO_Pin_4
#define LCD_nRD_ACT()   GPIOD_BSRR = GPIOB_BRR  = GPIO_Pin_4

#define RANGE_A_LOW()   GPIOB_BRR  = GPIO_Pin_0
#define RANGE_A_HIGH()  GPIOB_BSRR = GPIO_Pin_0
#define RANGE_B_LOW()   GPIOC_BRR  = GPIO_Pin_5
#define RANGE_B_HIGH()  GPIOC_BSRR = GPIO_Pin_5
#define RANGE_C_LOW()   GPIOC_BRR  = GPIO_Pin_4
#define RANGE_C_HIGH()  GPIOC_BSRR = GPIO_Pin_4
#define RANGE_D_LOW()   GPIOB_BRR  = GPIO_Pin_1
#define RANGE_D_HIGH()  GPIOB_BSRR = GPIO_Pin_1

#define FOUT_LOW()      GPIOD_BRR  = GPIO_Pin_12
#define FOUT_HIGH()     GPIOD_BSRR = GPIO_Pin_12

void NVIC_Configuration(void);
//void GPIO_Config(void);
//void DMA_Configuration(void);
//void ADC_Configuration(void);
//void Timer_Configuration(void);

void Set_Range(char Range);
void Set_Base(char Base);
void ADC_Stop(void);
void ADC_Start(void); 
void Set_Y_Pos(unsigned short i, unsigned short Y0); 
char Test_USB_ON(void); 
char SD_Card_ON(void); 

void Delayms(unsigned short delay);
void WaitForKey(void);

extern LIB_Interface *pLib;

extern volatile unsigned short Delay_Counter;
extern volatile unsigned short Refresh_Counter;
extern volatile unsigned char Key_Buffer;

#endif 
/****************************** END OF FILE ***********************************/
