/*******************************************************************************
* File Name: main.c
*******************************************************************************/

#include "Lcd.h"
#include "stm32f10x_lib.h"
#include "usb_lib.h"
#include "HW_V1_Config.h"
#include "memory.h"

void main(void)
{
/*--------------initialization-----------*/

  Set_System();
  NVIC_Configuration();
  GPIO_Config();
  SD_Card_Check();
  USB_Init();
  ADC_Configuration();
  Timer_Configuration();
  LCD_Initial();
  Clear_Screen(BLACK); 
  Display_Logo(110,150);  
 
/*----------Power ON Information----------*/ 

  Display_Str(80, 87, GRN,   PRN, "System Initializing");
  Display_Str(102, 71, GRN,   PRN, "Please Wait");
  Display_Str(8, 39, WHITE, PRN, "DSO FW Copyright (c) BenF 2010-2011"); 
  Display_Str(8, 23, YEL,   PRN, "LIB ver 3.13");
  
  //WaitForKey();

  // check for presence of APP and jump to start
  pApp = (APP_Interface *)*(u32 *)(APP_VECTORS + 7 * 4);
  if (pApp->Signature == APP_SIGNATURE)
      pApp->APP_Start();

  Display_Str(150, 23, RED, PRN, "No APP found");
  while (1);
}
/********************************* END OF FILE ********************************/
