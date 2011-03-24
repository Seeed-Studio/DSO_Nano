/*******************************************************************************
File Name: HW_V1_config.c 
*******************************************************************************/
#include "Function.h"
#include "Menu.h"
#include "stm32f10x_it.h"
#include "stm32f10x_systick.h"
#include "HW_V1_Config.h"
#include <stdlib.h>

void __iar_program_start(void);

const APP_Interface APP_Offsets = {
  APP_SIGNATURE,
  __iar_program_start
};

LIB_Interface *pLib;

volatile unsigned short Refresh_Counter, Delay_Counter;
volatile unsigned char Key_Buffer;

/*******************************************************************************
Function Name : GPIO_Config
Description : gpio configuration 
*******************************************************************************/
//void        GPIO_Config(void)
//{
//   GPIOA_CRL = 0x88888080; /*
//                            * GPIOA Bit0-7 status setting 
//							* |||||||+----- Nib0  Ain   analog input
//                            * ||||||+------ Nib1  NC    pullup input
//                            * |||||+------- Nib2  Vbat  analog input 
//                            * ||||+-------- Nib3  KB    pullup input  
//                            * |||+--------- Nib4  KP    pullup input  
//                            * ||+---------- Nib5  KL    pullup input  
//                            * |+----------- Nib6  KU    pullup input  
//                            * +------------ Nib7  KR    pullup input
//                            */
//   GPIOA_CRH = 0x8884488B; /*
//                            * GPIOA Bit8-15 status setting
//							* |||||||+----- Nib8  MCO    multiplexer output
//                            * ||||||+------ Nib9  NC     pullup input
//                            * |||||+------- Nib10 NC     pullup input  
//                            * ||||+-------- Nib11 D-    Hi-impedance input  
//                            * |||+--------- Nib12 D+    Hi-impedance input  
//                            * ||+---------- Nib13 TMS    pullup input  
//                            * |+----------- Nib14 TCK    pullup input  
//                            * +------------ Nib15 TDI    pullup input
//                            */
//   GPIOA_ODR = 0x0FFFF;
//
//   GPIOB_CRL = 0x88888411; /*
//                            * GPIOB Bit0-7 status setting
//							* |||||||+----- Nib0  A     lower speed output
//                            * ||||||+------ Nib1  D     lower speed output
//                            * |||||+------- Nib2  BT1   Hi-impedance input  
//                            * ||||+-------- Nib3  TDO   pullup input  
//                            * |||+--------- Nib4  TRST  pullup input  
//                            * ||+---------- Nib5  NC    pullup input  
//                            * |+----------- Nib6  NC    pullup input  
//                            * +------------ Nib7  NC    pullup input
//                            */
//   GPIOB_CRH = 0xBBB1B488; /*
//                            * GPIOB Bit8-15 status setting 
//							* |||||||+----- Nib8  NC    pullup input
//                            * ||||||+------ Nib9  NC    pullup input
//                            * |||||+------- Nib10 Vusb  Hi-impedance input  
//                            * ||||+-------- Nib11 V0    multiplexer output  
//                            * |||+--------- Nib12 SDCS  lower speed output  
//                            * ||+---------- Nib13 SCK   multiplexer output  
//                            * |+----------- Nib14 MISO  multiplexer output  
//                            * +------------ Nib15 MOSI  multiplexer output
//                            */
//   GPIOB_ODR = 0x0FFFF;
//
//   GPIOC_CRL = 0x84118881; /*
//                            * GPIOC Bit0-7 status setting 
//							* |||||||+----- Nib0  nCLS  lower speed output
//                            * ||||||+------ Nib1  NC    pullup input
//                            * |||||+------- Nib2  NC    pullup input  
//                            * ||||+-------- Nib3  NC    pullup input  
//                            * |||+--------- Nib4  C     lower speed output  
//                            * ||+---------- Nib5  B     lower speed output  
//                            * |+----------- Nib6  NC    Hi-impedance input  
//                            * +------------ Nib7  NC    pullup input
//                            */
//   GPIOC_CRH = 0x88888884; /*
//                            * GPIOC Bit8-15 status setting 
//							* |||||||+----- Nib8  NC    Hi-impedance input
//                            * ||||||+------ Nib9  NC    pullup input
//                            * |||||+------- Nib10 NC    pullup input  
//                            * ||||+-------- Nib11 NC    pullup input  
//                            * |||+--------- Nib12 NC    pullup input  
//                            * ||+---------- Nib13 NC    pullup input  
//                            * |+----------- Nib14 NC    pullup input  
//                            * +------------ Nib15 NC    pullup input
//                            */
//   GPIOC_ODR = 0x0FFFF;
//
//   GPIOD_CRL = 0x38338838; /*
//                            * GPIOD Bit0-7 status setting 
//							* |||||||+----- Nib0  NC    pullup input
//                            * ||||||+------ Nib1  RS    high speed output
//                            * |||||+------- Nib2  NC    pullup input  
//                            * ||||+-------- Nib3  NC    pullup input  
//                            * |||+--------- Nib4  nRD   high speed output  
//                            * ||+---------- Nib5  nWR   high speed output  
//                            * |+----------- Nib6  NC    pullup input  
//                            * +------------ Nib7  nCS   high speed output
//                            */
//   GPIOD_CRH = 0x88838884; /*
//                            * GPIOD Bit8-15 status setting 
//							* |||||||+----- Nib8  NC    pullup input 
//                            * ||||||+------ Nib9  KD    pullup input 
//                            * |||||+------- Nib10 SDDT  pullup input   
//                            * ||||+-------- Nib11 KM    pullup input   
//                            * |||+--------- Nib12 Fout  high speed& multiplexer output
//                            * ||+---------- Nib13 NC    pullup input   
//                            * |+----------- Nib14 NC    pullup input   
//                            * +------------ Nib15 NC    pullup input 
//                            */
//   GPIOD_ODR = 0x0FF7F;
//
//   GPIOE_CRL = 0x33333333; // Bit0-7  high speed output£¨LCD_DB0-7 £©
//
//   GPIOE_CRH = 0x33333333; // Bit8-15 high speed output£¨LCD_DB8-15£©
//
//   AFIO_MAPR = 0x00001200; /*
//                            * AFIO remapping  the register 
//							*/
//}

/*******************************************************************************
Function Name : DMA_configuration
Description : initialize the DMA channel one,ues it to save and transfer the ADC sampling data
*******************************************************************************/
//void     DMA_Configuration(void)
//{
//  DMA_InitTypeDef DMA_InitStructure;
//
//  DMA_DeInit(DMA_Channel1);  
//  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDR;
//  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&Scan_Buffer[0];
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//  DMA_InitStructure.DMA_BufferSize = 0x00001000 / 3;
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
//  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//  DMA_Init(DMA_Channel1, &DMA_InitStructure);
//
//  DMA_ITConfig(DMA_Channel1, DMA_IT_TC, ENABLE);  
//  DMA_ISR = 0x00000000;
//  DMA_IFCR = 0x00000000;
//  DMA_CPAR1 = ADC1_DR_ADDR; // base address of the peripheral's  data register for DMA1
//  DMA_CMAR1 = Scan_Offset[0]; // storage address for DMA1
//  DMA_CNDTR1 = SEGMENT_SIZE; // transmition data register of the DMA1 
//  DMA_CCR1 = 0x00003582;
//  DMA_CCR1 |= 0x00000001; // EN = 1£ºopen the DMA channel one
//}

/*******************************************************************************
Function Name : NVIC_Configuration
Description : Configure DMA inbterrupt channel priority
*******************************************************************************/
void      NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel = DMAChannel1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
Function Name : ADC_Configuration
Description : initialize the ADC status,transfer the data to buffer by DMA after sampling
*******************************************************************************/
//void       ADC_Configuration(void)
//{
//   ADC2_CR1 = 0x00000000;
//   ADC1_CR1 = 0x00000000; /*
//                           * ||||||++---AWDCH[4:0]£ºchannel selection bits for analog watchdog
//                           * ||||||+----=000£ºdisable EOC¡¢AWD¡¢JEOC interrupt
//                           * |||||+-----Nib8_SCAN=0£ºdo not use scan mode
//                           * ||||++-----remain the reset value
//                           * |||+-------DUALMOD=0£ºindependent mode
//                           * ||+--------disable analog watchdog in the regulation and injection channel
//                           * ++---------reserved
//                           */
//   ADC2_CR2 = 0x00100000;
//   ADC1_CR2 = 0x00100100; /*
//                           * |||||||+---ADON=0£ºclose ADC transition/calibration£¬=1£ºenable ADC and start transition
//                           * |||||||+---CONT=0£º1£ºcontinuous transition mode 
//                           * |||||||+---CAL=0£¬=1£ºafter finished the AD calibration,clear the corresponding bit by hardware 
//                           * |||||||+---RSTCAL=0£¬=1£ºinit the calibration register and cleared it when finishend
//                           * ||||||+----reserved |||||+-----ALIGN=0 &
//                           * DMA=1£ºalign right and DMA mode ||||+------
//                           * |||+-------=0000£ºregulation channel ues the CC1 event of Timer1 to start transition£¬=1110£ºSWSTART
//                           * ||+--------=1£ºuse the external trigger to start transition   ++---------reserved
//                           */
//   ADC2_SQR1 = 0x00000000;
//   ADC1_SQR1 = 0x00000000; /*
//                            * ADC rule sequence register one
//                            * ||++++++---SQ13-16=00000£ºthe 13-16 conversion channel number in the conversion array(5Bits*4)
//                            * ++---------reversed
//                            */
//   ADC2_SQR3 = 0x00000002;
//   ADC1_SQR3 = 0x00000000; /*
//                            * ADC rule sequence register three
//                            * ||||||++---SQ1=00000£ºthe first conversion channel number in the conversion array(0)
//                            * +++++++----SQ2-6=00000£ºthe 2-6 conversion channel number in the conversion array(5Bits*5)
//                            * +----------reserved
//                            */
//   ADC2_SMPR2 = 0x00000000;
//   ADC1_SMPR2 = 0x00000000; /*
//                             * ADC sampling timer registers one  
//                             * |||||||+---SMP01=001(3Bits)£ºthe sampling time of the channel 00  
//                             * 1.5T   
//                             * ||||+------SMP04=001(3Bits)£ºthe sampling time of the channel 04
//                             * 7.5T
//                             * |+++++++---SMP11-17=000(3Bits*7)£ºthe sampling time of the channel 11-17
//                             * 1.5T ++---------reserved
//                             */
//   ADC1_CR2 |= 0x00000001;
//   ADC2_CR2 |= 0x00000001; // ADON=1£ºstart ADC1¡¢ADC2
//
//   ADC1_CR2 |= 0x00000008;
//   while (ADC1_CR2 & 0x00000008); // init the calibration register of the ADC1
//
//   ADC2_CR2 |= 0x00000008;
//   while (ADC2_CR2 & 0x00000008); //  init the calibration register of the ADC1
//
//   ADC1_CR2 |= 0x00000004;
//   while (ADC1_CR2 & 0x00000004); // ADC1 calibration
//
//   ADC2_CR2 |= 0x00000004;
//   while (ADC2_CR2 & 0x00000004); // ADC2 calibration
//
//}
/*******************************************************************************
Function Name : Timer_Configuration
Description : initialize the system timer
*******************************************************************************/
//void      Timer_Configuration(void)
//{
////---------------TIM1 for trigger ADC sample at regular time -------------------
//   Set_Base(Item_Index[2]);
//   TIM1_CR1 = 0x0094; /*
//                       * 0000 0000 1001 0100   |||| |||| ||||
//                       * |||+---CEN=0£¬disable timer |||| |||| ||||
//                       * ||+----UDIS=0£¬allow UEV update |||| |||| ||||
//                       * |+-----URS=1£ºproduce interrupt or DMA requst only when the counter overflow   ||||
//                       * |||| ||||
//                       * +------OPM=0£ºWhen the updating  event occurs , counter doesn't stop  ||||
//                       * |||| |||+--------DIR=1£¬down counter |||| ||||
//                       * |++---------CMS=00£¬Edge alignment mode |||| ||||
//                       * +-----------ARPE=1£¬put TIM1_ARR to buffer  ||||
//                       * ||++-------------CKD=00,CK_INT frequency dividing ratio is 1
//                       * ++++-++---------------reserved
//                       */
//   TIM1_RCR = 0x0000; /*
//                       * 0000 0000 0000 0001   |||| |||| ++++
//                       * ++++---the PWM cycle add one in the edge alignment mode 
//                       * ++++-++++-------------reserved
//                       */
//   TIM1_CCER = 0x0001; /*
//                        * 0000 0000 0000 0001   |||| |||| ||||
//                        * |||+---CC1E=1£¬OC1 signal output to the coresponding pin ||||
//                        * |||| |||| ||+----CC1P=0£¬OC1 High Level On |||| ||||
//                        * |||| |+-----CC1NE=0£¬OC1N disable output |||| |||| ||||
//                        * +------CC1NP=0£¬OC1N High Level On
//                        * ++++-++++-++++--------CC2¡¢CC3¡¢CC4 setted to the reset value
//                        */
//   TIM1_CCMR1 = 0x0078; /*
//                         * 0000 0000 0111 1100   |||| |||| ||||
//                         * ||++---CC1S=00£¬CC1 channel setted fot output |||| |||| |||| 
//                         * |+-----OC1FE=1£¬ |||| ||||
//                         * |||| +------OC1PE=1£¬  ||||
//                         * |||| |+++--------0C1M=111£¬PWM mode2 |||| ||||
//                         * +-----------OC1CE=0£¬ETRF input  do not affet OC1REF 
//                         * ++++-++++-------------CC2 channel for reset
//                         */
//   TIM1_BDTR = 0x8000; /*
//                        * 1000 0000 0000 0000  
//                        * |+++-++++-++++-++++---other bits remain unchanged 
//                        * +---------------------MOE=0£¬enable the main output
//                        */
//   TIM1_DIER = 0x4200; /*
//                        * 0100 0011 0000 0000  DMA and Interrupt enabled registers  |     ||
//                        *      +----CC1IE=0£ºdisable capture/compare interrupt one |
//                        * |+-------------UDE=1£ºallow the request of the DMA update  |
//                        * +--------------CC1DE=1£ºallow capture/compare DMA1 request
//                        * +--------------------TDE=1£ºenable request to trigger DMA 
//                        */
//   TIM1_CR1 |= 0x0001; // CEN=1£¬enable TIMER1 counter 
//
////-----------------TIM2_CH4 used for PWM output the offset voltage of Y channel -------------------   
//   TIM2_PSC = 0;
//   TIM2_ARR = 3839; // 72MHz/3840=18.75KHz 
//
//   Set_Y_Pos(Item_Index[Y_SENSITIVITY], Item_Index[V0]);
//   TIM2_CR1 = 0x0084; /*
//                       * 0000 0000 1000 0100   |||| |||| ||||
//                       * |||+---CEN=0£¬disable counter |||| |||| ||||
//                       * ||+----UDIS=0£¬allow UEV update |||| |||| ||||
//                       * |+-----URS=1£ºproduce interrupt or DMA requst only when the counter overflow  ||||
//                       * |||| ||||
//                       * +------OPM=0£ºWhen the updating  event occurs , counter doesn't stop   ||||
//                       * |||| |||+--------DIR=0£¬up counter |||| ||||
//                       * |++---------CMS=00£¬Edge alignment mode |||| ||||
//                       * +-----------ARPE=1£¬put TIM2_ARR to buffer  ||||
//                       * ||++-------------CKD=00,CK_INT frequency dividing ratio is 1
//                       * ++++-++---------------reserved
//                       */
//   TIM2_CCER = 0x3000; /*
//                        * 0011 0000 0000 0000   ||||
//                        * ++++-++++-++++---CC1¡¢CC2¡¢CC3 setted to the reset value
//                        * |||+------------------CC4E=1£¬OC4 signal output to the coresponding pin
//                        * ||+-------------------CC4P=1£¬OC4 Low Level On 
//                        * |+--------------------CC4NE=0£¬OC4N disable output
//                        * +---------------------CC4NP=0£¬OC4N High Level On
//                        */
//   TIM2_CCMR2 = 0x7C00; /*
//                         * 0111 1100 0000 0000   |||| ||||
//                         * ++++-++++---CC3 channel setted fot output ||||
//                         * ||++-------------CC4S=00£¬CC4 used for output |||| 
//                         * |+---------------OC4FE=1£¬
//                         * |||| +----------------OC4PE=1£¬
//                         * 
//                         * |+++------------------0C4M=111£¬PWM mode2
//                         * +---------------------OC4CE=0£¬ETRF input  do not affet OC4REF
//                         * 
//                         */
//   TIM2_CR1 |= 0x0001; // CEN=1£¬enable TIMER2 counter 
//
////-----------------TIM3 for system timer-------------------  
//   TIM3_PSC = 9;
//   TIM3_ARR = 7199; // 1mS clock cycle =£¨PSC+1£©¡Á£¨ARR+1£©/£¨72MHz/2£©£¨uS£©
//
//   TIM3_CCR1 = 3600; // pulse width£¨duty cycle£©50%
//
//   TIM3_CR1 = 0x0084; /*
//                       * 0000 0000 1000 0100   |||| |||| ||||
//                       * |||+---CEN=0£¬disable counter |||| |||| ||||
//                       * ||+----UDIS=0£¬allow UEV update |||| |||| ||||
//                       * |+-----URS=1£ºproduce interrupt or DMA requst only when the counter overflow   ||||
//                       * |||| ||||
//                       * +------OPM=0£ºWhen the updating  event occurs , counter doesn't stop   ||||
//                       * |||| |||+--------DIR=0£¬up counter |||| ||||
//                       * |++---------CMS=00£¬Edge alignment mode |||| ||||
//                       * +-----------ARPE=1£¬put  TIM3_ARR to buffer ||||
//                       * ||++-------------CKD=00,CK_INT frequency dividing ratio is 1
//                       * ++++-++---------------reserved
//                       */
//   TIM3_DIER = 0x0002; /*
//                        * 0000 0000 0000 0010  DMA and Interrupt enabled registers |
//                        * +----CC1IE=1£ºenable capture/compare interrupt one
//                        * +--------------CC1DE=1£ºallow capture/compare DMA1 request
//                        */
//   TIM3_CR1 |= 0x0001; // CEN=1£¬enable TIMER3 
//
////------------------TIM4_CH1 for outputting the selected frequency rectangle wave signal----------------------
//   TIM4_PSC = 7;
//   TIM4_ARR = Fout_ARR[Item_Index[11]]; // clock cycle
//
//   TIM4_CCR1 = (Fout_ARR[Item_Index[11]] + 1) / 2; // pulse width0.5mS£¨duty cycle£º50%£©
//
//   TIM4_CR1 = 0x0084; /*
//                       * 0000 0000 1000 0100   |||| |||| ||||
//                       * |||+---CEN=0£¬disable counter |||| |||| ||||
//                       * ||+----UDIS=0£¬allow UEV update |||| |||| ||||
//                       * |+-----URS=1£ºproduce interrupt or DMA requst only when the counter overflow  ||||
//                       * |||| ||||
//                       * +------OPM=0£ºWhen the updating  event occurs , counter doesn't stop  ||||
//                       * |||| |||+--------DIR=0£¬up counter  |||| ||||
//                       * |++---------CMS=00£¬edge alignment mode |||| ||||
//                       * +-----------ARPE=1£¬put TIM3_ARR to buffer ||||
//                       * ||++-------------CKD=00,CK_INT frequency dividing ratio is 1 
//                       * ++++-++---------------reserved
//                       */
//   TIM4_CCER = 0x0001; /*
//                        * 0000 0000 0000 0001   |||| |||| ||||
//                        * |||+---CC1E=1£¬OC1 signal output to the coresponding pin  ||||
//                        * |||| |||| ||+----CC1P=0£¬OC1 High Level On |||| ||||
//                        * |||| |+-----CC1NE=0£¬OC1N disable output |||| |||| ||||
//                        * +------CC1NP=0£¬OC1N High Level On
//                        * ++++-++++-++++--------CC2¡¢CC3¡¢CC4 setted to the reset value
//                        */
//   TIM4_CCMR1 = 0x0078; /*
//                         * 0000 0000 0111 1100   |||| |||| ||||
//                         * ||++---CC1S=00£¬CC1  channel setted fot output |||| |||| |||| 
//                         * |+-----OC1FE=1£¬ |||| ||||
//                         * |||| +------OC1PE=1£¬  ||||
//                         * |||| |+++--------0C1M=111£¬PWM mode2 |||| ||||
//                         * +-----------OC1CE=0£¬ETRF input  do not affet OC1REF
//                         * ++++-++++-------------CC2 channel for reset
//                         */
//   TIM4_CR1 |= 0x0001; // CEN=1£¬enable TIMER4 
//
//}
/*******************************************************************************
Function Name : Set_Range
Description : set the range of the voltage 
*******************************************************************************/
void     Set_Range(char Range)
{

   switch (Range)
   {
   case 0: // 10mV

   case 1: // 20mV

   case 2: // 50mV

   case 3: // 100mV

      RANGE_A_HIGH();
      RANGE_B_HIGH();
      RANGE_C_HIGH();
      RANGE_D_HIGH();
      break;
   case 4: // 200mV

   case 5: // 500mV

   case 6: // 1V

      RANGE_A_HIGH();
      RANGE_B_HIGH();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 7: // 2V

   case 8: // 5V

   case 9: // 10V

      RANGE_A_LOW();
      RANGE_B_HIGH();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 10: // 0.2V/Div £¨20mV ¡Á10/Div£© 

   case 11: // 0.5V/Div £¨50mV ¡Á10/Div£©

   case 12: // 1V/Div £¨0.1V ¡Á10/Div£©

      RANGE_A_HIGH();
      RANGE_B_LOW();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 13: // 2V/Div £¨0.2V ¡Á10/Div£©

   case 14: // 5V/Div £¨0.5V ¡Á10/Div£©

   case 15: // 10V/Div £¨  1V ¡Á10/Div£©

      RANGE_A_LOW();
      RANGE_B_LOW();
      RANGE_C_HIGH();
      RANGE_D_LOW();
      break;
   case 16: // 20V/Div £¨ 2V ¡Á10/Div£©

   case 17: // 50V/Div £¨ 5V ¡Á10/Div£©

   case 18: // 100V/Div £¨10V ¡Á10/Div£©

      RANGE_A_HIGH();
      RANGE_B_HIGH();
      RANGE_C_LOW();
      RANGE_D_LOW();
      break;
   default: // Connected To GND

      RANGE_A_LOW();
      RANGE_B_LOW();
      RANGE_C_LOW();
      RANGE_D_LOW();
      break;
   }
}

/*******************************************************************************
Function Name : Set_Base
Description : set the base level of the Horizontal scan
Para : Base is the index of the Scan_PSC&Scan_ARR
*******************************************************************************/
void     Set_Base(char Base)
{
   TIM1_PSC = Scan_PSC[Base];
   TIM1_ARR = Scan_ARR[Base];
   TIM1_CCR1 = (Scan_ARR[Base] + 1) / 2;
}

void    ADC_Stop(void)
{
   DMA_CCR1 = 0x00000000; // disable DMA1
   ScanMode = 0;     // 0=idle, 1=pre-fetch, 2=trig-fetch, 3=post-fetch
}
/*******************************************************************************
Function Name : ADC_Start
Description : start the ADC sampling scan
*******************************************************************************/
void     ADC_Start(void)
{
   ADC_Stop(); // disable DMA1
   ScanSegment = 0;   // scan buffer has three virtual segments (pre-fetch, trig-seek and post-fetch)
   ScanMode = 1;     // 0=idle, 1=pre-fetch, 2=trig-seek, 3=post-fetch
   DMA_CPAR1 = ADC1_DR_ADDR; // base address of the peripheral's data register for DMA1
   DMA_CMAR1 = (u32)Scan_Buffer;
   DMA_CNDTR1 = SEGMENT_SIZE;
   DMA_CCR1 = 0x00003583; // enable DMA1
}
/*******************************************************************************
Function Name : Set_Y_Pos
Description : set the baseline of the no-voltage
Para : i is the index of the array, Y0 is the voltage 
*******************************************************************************/
void    Set_Y_Pos(unsigned short i, unsigned short Y0)
{
   TIM2_CCR4 = ((240 - Y0) * Y_POSm[i]) / 32 + Y_POSn[i];

}
/*******************************************************************************
Function Name : Test_USB_ON
Description : detect the USB status
Return:	 1: USB plug in, 0:USB plug out
*******************************************************************************/
char            Test_USB_ON(void)
{
   if (GPIOB_IDR & 0x0400)
      return 1;
   else
      return 0;
}

/*******************************************************************************
Function Name : SD_Card_ON
Description : detect the SD card status
Return:	 1: SD card plug in, 0:SD card plug out
*******************************************************************************/
char            SD_Card_ON(void)
{
   if (GPIOD_IDR & 0x0400)
      return 0;
   else
      return 1; // SD_Card_ON

}

/*******************************************************************************
 Delay ms
*******************************************************************************/
void Delayms(unsigned short delay)
{
  Delay_Counter = delay;
  while (Delay_Counter);
}

/*******************************************************************************
 wait for keypress
*******************************************************************************/
void WaitForKey(void)
{
  Key_Buffer = 0;
  while (!Key_Buffer);
}

/******************************** END OF FILE *********************************/
