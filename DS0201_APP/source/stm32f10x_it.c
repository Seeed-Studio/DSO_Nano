/*******************************************************************************
File Name: stm32f10x_it.c  
*******************************************************************************/
#include "stm32f10x_it.h"
#include "HW_V1_Config.h"
#include "Function.h"
#include "Menu.h"
#include "ASM_Function.h"

void            NMIException(void)
{
}

void            HardFaultException(void)
{
   while (1)
   {
   }
}

void            MemManageException(void)
{
   while (1)
   {
   }
}

void            BusFaultException(void)
{
   while (1)
   {
   }
}
void            UsageFaultException(void)
{
   while (1)
   {
   }
}

void            DebugMonitor(void)
{
}

void            SVCHandler(void)
{
}

void            PendSVC(void)
{
}

void            SysTickHandler(void)
{
}

void            WWDG_IRQHandler(void)
{
}

void            PVD_IRQHandler(void)
{
}

void            TAMPER_IRQHandler(void)
{
}

void            RTC_IRQHandler(void)
{
}

void            FLASH_IRQHandler(void)
{
}

void            RCC_IRQHandler(void)
{
}

void            EXTI0_IRQHandler(void)
{
}

void            EXTI1_IRQHandler(void)
{
}

void            EXTI2_IRQHandler(void)
{
}

void            EXTI3_IRQHandler(void)
{
}

void            EXTI4_IRQHandler(void)
{
}

void            DMAChannel1_IRQHandler(void)
{
    unsigned char ss = ScanSegment;

    DMA_IFCR = 0x00000002; // clear transfer complete interrupt for DMA channel1
  
    if (ss >= 2) ss = 0;
    else ss++;
    if (ScanMode <= 1) ScanMode = 2;  // advance to trig-fetch
    if ((ScanMode < 3) || (SyncSegment != ((ss < 2) ? ss + 1 : 0))) {
      DMA_CCR1 = 0x00000000; // disable DMA1
      DMA_CMAR1 = (u32)&Scan_Buffer + ss * SEGMENT_SIZE * sizeof(Scan_Buffer[0]);
      DMA_CNDTR1 = SEGMENT_SIZE;
      DMA_CCR1 = 0x00003583; // enable DMA1
    } else ScanMode = 0;  // idle, DMA disabled
    ScanSegment = ss;
}

void            DMAChannel2_IRQHandler(void)
{
}

void            DMAChannel3_IRQHandler(void)
{
}

void            DMAChannel4_IRQHandler(void)
{
}

void            DMAChannel5_IRQHandler(void)
{
}

void            DMAChannel6_IRQHandler(void)
{
}

void            DMAChannel7_IRQHandler(void)
{
}

void            ADC_IRQHandler(void)
{
}

void            USB_HP_CAN_TX_IRQHandler(void)
{
   pLib->CTR_HP();
}

void            USB_LP_CAN_RX0_IRQHandler(void)
{
   pLib->USB_Istr();
}

void            CAN_RX1_IRQHandler(void)
{
}

void            CAN_SCE_IRQHandler(void)
{
}

void            EXTI9_5_IRQHandler(void)
{
}

void            TIM1_BRK_IRQHandler(void)
{
}

void            TIM1_UP_IRQHandler(void)
{
}

void            TIM1_TRG_COM_IRQHandler(void)
{
}

void            TIM1_CC_IRQHandler(void)
{
}

void            TIM2_IRQHandler(void)
{
}
/****************************************************************************
Function Name : TIM3_IRQHandler
Description : TIMER 3 interrupt request handle,
              its mainly uesed for system clock delay 
	      and detect the key periodically
******************************************************************************/
void            TIM3_IRQHandler(void)
{
  static unsigned char Counter_20ms, Bat_Counter = 5;
  static unsigned char KeyCode_Last, Key_Wait_Counter, m_count;
  unsigned char   KeyCode = 0;

   TIM3_SR = 0; // clear the interrupt flag

   if (Delay_Counter)
      Delay_Counter--;
   
   if (Refresh_Counter)
      Refresh_Counter--;
   
   if (Counter_20ms)
      Counter_20ms--;
  
   else { // check key press every 20ms, battery status every 100ms
      Counter_20ms = 20;

      if (++Bat_Counter >= 5) {
        static u32 BatSum = 3200 * 32;
        u32 BatAvg = BatSum / 32,
            BatNow = *((vu16 *)(ADC1_DR_ADDR + 2));
        u8 PwrLvl;
        BatAvg = BatSum / 32;
        BatSum = BatSum + BatNow - BatAvg;
        if (GPIOB_IDR & 0x0400) PwrLvl = 4; // USB power
        else if (BatAvg > 3000) PwrLvl = 3;
        else if (BatAvg > 2730) PwrLvl = 2;
        else if (BatAvg > 2460) PwrLvl = 1;
        else                    PwrLvl = 0;
        if (PwrLvl != Item_Index[POWER_INFO]) {
          Item_Index[POWER_INFO] = PwrLvl;
          Update[POWER_INFO] = 1;
        }
        Bat_Counter = 0;
      }
      
      if (Key_Wait_Counter) 
        Key_Wait_Counter--;
      
      if (!(GPIOD_IDR & KEY_M)) KeyCode = KEYCODE_M;
      else if (!(GPIOA_IDR & KEY_B)) KeyCode = KEYCODE_B;
      else if (!(GPIOA_IDR & KEY_PLAY)) KeyCode = KEYCODE_PLAY;
      else if (!(GPIOA_IDR & KEY_UP)) KeyCode = KEYCODE_UP;
      else if (!(GPIOD_IDR & KEY_DOWN)) KeyCode = KEYCODE_DOWN;
      else if (!(GPIOA_IDR & KEY_LEFT)) KeyCode = KEYCODE_LEFT;
      else if (!(GPIOA_IDR & KEY_RIGHT)) KeyCode = KEYCODE_RIGHT;
    
      if (KeyCode) {
        if (KeyCode == KeyCode_Last) {
          if (Key_Wait_Counter == 0) {
            if (KeyCode >= KEYCODE_UP) {
              Key_Wait_Counter = 3; // 60ms repeat interval
              Key_Buffer = KeyCode;
            } else if ((KeyCode == KEYCODE_M) && (m_count == 0)) {
              m_count++;
              Key_Buffer = KEYCODE_B;
            }
          }
        } else {
          Key_Wait_Counter = 25;  // 500ms delay before repeat
          m_count = 0;
          Key_Buffer = KeyCode;
        }
      }
      KeyCode_Last = KeyCode;
   }
}

void            TIM4_IRQHandler(void)
{
}

void            I2C1_EV_IRQHandler(void)
{
}

void            I2C1_ER_IRQHandler(void)
{
}

void            I2C2_EV_IRQHandler(void)
{
}

void            I2C2_ER_IRQHandler(void)
{
}

void            SPI1_IRQHandler(void)
{
}

void            SPI2_IRQHandler(void)
{
}

void            USART1_IRQHandler(void)
{
}

void            USART2_IRQHandler(void)
{
}

void            USART3_IRQHandler(void)
{
}

void            EXTI15_10_IRQHandler(void)
{
}

void            RTCAlarm_IRQHandler(void)
{
}

void            USBWakeUp_IRQHandler(void)
{
}
/********************************* END OF FILE ********************************/
