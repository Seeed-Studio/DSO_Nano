/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : usb_scsi.h
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : All processing related to the SCSI commands
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_SCSI_H
#define __USB_SCSI_H

/* Includes ------------------------------------------------------------------*/
#include "USB_lib.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* SCSI Commands */
#define SCSI_FORMAT_UNIT                            0x04
#define SCSI_INQUIRY                                0x12
#define SCSI_MODE_SELECT6                           0x15
#define SCSI_MODE_SELECT10                          0x55
#define SCSI_MODE_SENSE6                            0x1A
#define SCSI_MODE_SENSE10                           0x5A
#define SCSI_ALLOW_MEDIUM_REMOVAL                   0x1E
#define SCSI_READ6                                  0x08
#define SCSI_READ10                                 0x28
#define SCSI_READ12                                 0xA8
#define SCSI_READ16                                 0x88

#define SCSI_READ_CAPACITY10                        0x25
#define SCSI_READ_CAPACITY16                        0x9E

#define SCSI_REQUEST_SENSE                          0x03
#define SCSI_START_STOP_UNIT                        0x1B
#define SCSI_TEST_UNIT_READY                        0x00
#define SCSI_WRITE6                                 0x0A
#define SCSI_WRITE10                                0x2A
#define SCSI_WRITE12                                0xAA
#define SCSI_WRITE16                                0x8A

#define SCSI_VERIFY10                               0x2F
#define SCSI_VERIFY12                               0xAF
#define SCSI_VERIFY16                               0x8F

#define SCSI_SEND_DIAGNOSTIC                        0x1D
#define SCSI_READ_FORMAT_CAPACITIES                 0x23

#define NOT_READY		                    0x02
#define UNIT_ATTENTION                              0x06
#define MEDIUM_CHANGED                              0x28
#define MEDIUM_NOT_PRESENT 			    0x3A
#define NO_SENSE                                    0x00
#define ILLEGAL_REQUEST                             0x05
#define INVALID_COMMAND                             0x20
#define INVALID_FIELED_IN_COMMAND                   0x24
#define PARAMETER_LIST_LENGTH_ERROR                 0x1A
#define INVALID_FIELD_IN_PARAMETER_LIST             0x26
#define ADDRESS_OUT_OF_RANGE                        0x21


#define READ_FORMAT_CAPACITY_DATA_LEN               0x0C
#define READ_CAPACITY10_DATA_LEN                    0x08
#define MODE_SENSE10_DATA_LEN                       0x08
#define MODE_SENSE6_DATA_LEN                        0x04
#define REQUEST_SENSE_DATA_LEN                      0x12
#define STANDARD_INQUIRY_DATA_LEN                   0x24
#define BLKVFY                                      0x04

extern  u8 Page00_Inquiry_Data[];
extern  u8 Standard_Inquiry_Data[];
extern  u8 Mode_Sense6_data[];
extern  u8 Mode_Sense10_data[];
extern  u8 Scsi_Sense_Data[];
extern  u8 ReadCapacity10_Data[];
extern  u8 ReadFormatCapacity_Data [];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SCSI_Inquiry_Cmd(void);
void SCSI_ReadFormatCapacity_Cmd(void);
void SCSI_ReadCapacity10_Cmd(void);
void SCSI_RequestSense_Cmd (void);
void SCSI_Start_Stop_Unit_Cmd(void);
void SCSI_ModeSense6_Cmd (void);
void SCSI_ModeSense10_Cmd (void);
void SCSI_Write10_Cmd(void);
void SCSI_Read10_Cmd(void);
void SCSI_Verify10_Cmd(void);

void SCSI_Invalid_Cmd(void);
void SCSI_Valid_Cmd(void);

void Set_Scsi_Sense_Data(u8 Sens_Key, u8 Asc);

void SCSI_TestUnitReady_Cmd(void);
#define SCSI_Prevent_Removal_Cmd         SCSI_Valid_Cmd

/* Invalid (Unsupported) commands */
#define SCSI_READ_CAPACITY16_Cmd         SCSI_Invalid_Cmd
#define SCSI_FormatUnit_Cmd              SCSI_Invalid_Cmd
#define SCSI_Write6_Cmd                  SCSI_Invalid_Cmd
#define SCSI_Write16_Cmd                 SCSI_Invalid_Cmd
#define SCSI_Write12_Cmd                 SCSI_Invalid_Cmd
#define SCSI_Read6_Cmd                   SCSI_Invalid_Cmd
#define SCSI_Read12_Cmd                  SCSI_Invalid_Cmd
#define SCSI_Read16_Cmd                  SCSI_Invalid_Cmd
#define SCSI_Send_Diagnostic_Cmd         SCSI_Invalid_Cmd
#define SCSI_Mode_Select6_Cmd            SCSI_Invalid_Cmd
#define SCSI_Mode_Select10_Cmd           SCSI_Invalid_Cmd
#define SCSI_Verify12_Cmd                SCSI_Invalid_Cmd
#define SCSI_Verify16_Cmd                SCSI_Invalid_Cmd

#endif /* __USB_SCSI_H */

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/

