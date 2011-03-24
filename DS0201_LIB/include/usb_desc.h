/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : Descriptor Header for Mass Storage Device
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define MASS_SIZ_DEVICE_DESC              18
#define MASS_SIZ_CONFIG_DESC              32

#define MASS_SIZ_STRING_LANGID            4
#define MASS_SIZ_STRING_VENDOR            38
#define MASS_SIZ_STRING_PRODUCT           38
#define MASS_SIZ_STRING_SERIAL            26
#define MASS_SIZ_STRING_INTERFACE         16

/* Exported functions ------------------------------------------------------- */
extern const u8 MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC];
extern const u8 MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC];

extern const u8 MASS_StringLangID[MASS_SIZ_STRING_LANGID];
extern const u8 MASS_StringVendor[MASS_SIZ_STRING_VENDOR];
extern const u8 MASS_StringProduct[MASS_SIZ_STRING_PRODUCT];
extern const u8 MASS_StringSerial[MASS_SIZ_STRING_SERIAL];
extern const u8 MASS_StringInterface[MASS_SIZ_STRING_INTERFACE];

#endif /* __USB_DESC_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/


