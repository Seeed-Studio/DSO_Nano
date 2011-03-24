/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : stm32f10x_bkp.h
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : This file contains all the functions prototypes for the
*                      BKP firmware library.
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_BKP_H
#define __STM32F10x_BKP_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_map.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Tamper Pin active level */
#define BKP_TamperPinLevel_High           ((u16)0x0000)
#define BKP_TamperPinLevel_Low            ((u16)0x0001)

#define IS_BKP_TAMPER_PIN_LEVEL(LEVEL) ((LEVEL == BKP_TamperPinLevel_High) || \
                                        (LEVEL == BKP_TamperPinLevel_Low))

/* RTC output source to output on the Tamper pin */
#define BKP_RTCOutputSource_None          ((u16)0x0000)
#define BKP_RTCOutputSource_CalibClock    ((u16)0x0080)
#define BKP_RTCOutputSource_Alarm         ((u16)0x0100)
#define BKP_RTCOutputSource_Second        ((u16)0x0300)

#define IS_BKP_RTC_OUTPUT_SOURCE(SOURCE) ((SOURCE == BKP_RTCOutputSource_None) || \
                                          (SOURCE == BKP_RTCOutputSource_CalibClock) || \
                                          (SOURCE == BKP_RTCOutputSource_Alarm) || \
                                          (SOURCE == BKP_RTCOutputSource_Second))

/* Data Backup Register */
#define BKP_DR1                           ((u16)0x0004)
#define BKP_DR2                           ((u16)0x0008)
#define BKP_DR3                           ((u16)0x000C)
#define BKP_DR4                           ((u16)0x0010)
#define BKP_DR5                           ((u16)0x0014)
#define BKP_DR6                           ((u16)0x0018)
#define BKP_DR7                           ((u16)0x001C)
#define BKP_DR8                           ((u16)0x0020)
#define BKP_DR9                           ((u16)0x0024)
#define BKP_DR10                          ((u16)0x0028)

#define IS_BKP_DR(DR) ((DR == BKP_DR1) ||  (DR == BKP_DR2) || (DR == BKP_DR3) || \
                       (DR == BKP_DR4) ||  (DR == BKP_DR5) || (DR == BKP_DR6) || \
                       (DR == BKP_DR7) ||  (DR == BKP_DR8) || (DR == BKP_DR9) || \
                       (DR == BKP_DR10))

#define IS_BKP_CALIBRATION_VALUE(VALUE) (VALUE <= 0x7F)

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void BKP_DeInit(void);
void BKP_TamperPinLevelConfig(u16 BKP_TamperPinLevel);
void BKP_TamperPinCmd(FunctionalState NewState);
void BKP_ITConfig(FunctionalState NewState);
void BKP_RTCOutputConfig(u16 BKP_RTCOutputSource);
void BKP_SetRTCCalibrationValue(u8 CalibrationValue);
void BKP_WriteBackupRegister(u16 BKP_DR, u16 Data);
u16 BKP_ReadBackupRegister(u16 BKP_DR);
FlagStatus BKP_GetFlagStatus(void);
void BKP_ClearFlag(void);
ITStatus BKP_GetITStatus(void);
void BKP_ClearITPendingBit(void);

#endif /* __STM32F10x_BKP_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
