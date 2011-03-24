/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : usb_scsi.c
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

/* Includes ------------------------------------------------------------------*/
#include "usb_scsi.h"
#include "memory.h"
#include "usb_bot.h"
#include "usb_regs.h"
#include "usb_mem.h"
#include "HW_V1_Config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
extern u8 Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
extern u8 Bot_State;
extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW;
u32 Mass_Memory_Size;
u32 Mass_Block_Size;
u32 Mass_Block_Count;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SCSI_Inquiry_Cmd
* Description    : SCSI Inquiry Command routine.
*******************************************************************************/
void SCSI_Inquiry_Cmd(void)
{
  u8* Inquiry_Data;
  u16 Inquiry_Data_Length;

  if (CBW.CB[1] & 0x01)/*Evpd is set*/
  {
    Inquiry_Data = Page00_Inquiry_Data;
    Inquiry_Data_Length = 5;
  }
  else
  {
    Inquiry_Data = Standard_Inquiry_Data;
    if (CBW.CB[4] <= STANDARD_INQUIRY_DATA_LEN)
      Inquiry_Data_Length = CBW.CB[4];
    else
      Inquiry_Data_Length = STANDARD_INQUIRY_DATA_LEN;
  }
  Transfer_Data_Request(Inquiry_Data, Inquiry_Data_Length);
}

/*******************************************************************************
* Function Name  : SCSI_ReadFormatCapacity_Cmd
* Description    : SCSI ReadFormatCapacity Command routine.
*******************************************************************************/
void SCSI_ReadFormatCapacity_Cmd(void)
{
  if (!SD_Card_Check())
  {
    Set_Scsi_Sense_Data(NOT_READY, MEDIUM_NOT_PRESENT);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
    Bot_Abort(DIR_IN);
    return;
  }

  ReadFormatCapacity_Data[4] = (u8)(Mass_Block_Count >> 24);
  ReadFormatCapacity_Data[5] = (u8)(Mass_Block_Count >> 16);
  ReadFormatCapacity_Data[6] = (u8)(Mass_Block_Count >>  8);
  ReadFormatCapacity_Data[7] = (u8)(Mass_Block_Count);

  ReadFormatCapacity_Data[9] = (u8)(Mass_Block_Size >>  16);
  ReadFormatCapacity_Data[10] = (u8)(Mass_Block_Size >>  8);
  ReadFormatCapacity_Data[11] = (u8)(Mass_Block_Size);
  Transfer_Data_Request(ReadFormatCapacity_Data, READ_FORMAT_CAPACITY_DATA_LEN);
}

/*******************************************************************************
* Function Name  : SCSI_ReadCapacity10_Cmd
* Description    : SCSI ReadCapacity10 Command routine.
*******************************************************************************/
void SCSI_ReadCapacity10_Cmd(void)
{
  if (!SD_Card_Check())
  {
    Set_Scsi_Sense_Data(NOT_READY, MEDIUM_NOT_PRESENT);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
    Bot_Abort(DIR_IN);
    return;
  }

  ReadCapacity10_Data[0] = (u8)((Mass_Block_Count - 1) >> 24);
  ReadCapacity10_Data[1] = (u8)((Mass_Block_Count - 1) >> 16);
  ReadCapacity10_Data[2] = (u8)((Mass_Block_Count - 1) >>  8);
  ReadCapacity10_Data[3] = (u8)(Mass_Block_Count - 1);

  ReadCapacity10_Data[4] = (u8)(Mass_Block_Size >>  24);
  ReadCapacity10_Data[5] = (u8)(Mass_Block_Size >>  16);
  ReadCapacity10_Data[6] = (u8)(Mass_Block_Size >>  8);
  ReadCapacity10_Data[7] = (u8)(Mass_Block_Size);
  Transfer_Data_Request(ReadCapacity10_Data, READ_CAPACITY10_DATA_LEN);
}

/*******************************************************************************
* Function Name  : SCSI_ModeSense6_Cmd
* Description    : SCSI ModeSense6 Command routine.
*******************************************************************************/
void SCSI_ModeSense6_Cmd (void)
{
  Transfer_Data_Request(Mode_Sense6_data, MODE_SENSE6_DATA_LEN);
}

/*******************************************************************************
* Function Name  : SCSI_ModeSense10_Cmd
* Description    : SCSI ModeSense10 Command routine.
*******************************************************************************/
void SCSI_ModeSense10_Cmd(void)
{
  Transfer_Data_Request(Mode_Sense10_data, MODE_SENSE10_DATA_LEN);
}

/*******************************************************************************
* Function Name  : SCSI_RequestSense_Cmd
* Description    : SCSI RequestSense Command routine.
*******************************************************************************/
void SCSI_RequestSense_Cmd(void)
{
  u8 Request_Sense_data_Length;

  if (CBW.CB[4] <= REQUEST_SENSE_DATA_LEN)
  {
    Request_Sense_data_Length = CBW.CB[4];
  }
  else
  {
    Request_Sense_data_Length = REQUEST_SENSE_DATA_LEN;
  }
  Transfer_Data_Request(Scsi_Sense_Data, Request_Sense_data_Length);
}

/*******************************************************************************
* Function Name  : Set_Scsi_Sense_Data
* Description    : Set Scsi Sense Data routine.
* Input          : u8 Sens_Key
                   u8 Asc.
*******************************************************************************/
void Set_Scsi_Sense_Data(u8 Sens_Key, u8 Asc)
{
  Scsi_Sense_Data[2] = Sens_Key;
  Scsi_Sense_Data[12] = Asc;
}

/*******************************************************************************
* Function Name  : SCSI_Start_Stop_Unit_Cmd
* Description    : SCSI Start_Stop_Unit Command routine.
*******************************************************************************/
void SCSI_Start_Stop_Unit_Cmd(void)
{
  Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
}

/*******************************************************************************
* Function Name  : SCSI_Read10_Cmd
* Description    : SCSI Read10 Command routine.
*******************************************************************************/
void SCSI_Read10_Cmd(void)
{

  if (Bot_State == BOT_IDLE)
  {
    if (!(Address_Management_Test(SCSI_READ10)))/*address out of range*/
    {
      return;
    }

    if ((CBW.bmFlags & 0x80) != 0)
    {
      Bot_State = BOT_DATA_IN;
      Read_Memory();
    }
    else
    {
      Bot_Abort(BOTH_DIR);
      Set_Scsi_Sense_Data(ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
      Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
    }
    return;
  }
  else if (Bot_State == BOT_DATA_IN)
  {
    Read_Memory();
  }
}

/*******************************************************************************
* Function Name  : SCSI_Write10_Cmd
* Description    : SCSI Write10 Command routine.
*******************************************************************************/
void SCSI_Write10_Cmd(void)
{

  if (Bot_State == BOT_IDLE)
  {
    if (!(Address_Management_Test(SCSI_WRITE10)))/*address out of range*/
    {
      return;
    }

    if ((CBW.bmFlags & 0x80) == 0)
    {
      Bot_State = BOT_DATA_OUT;
      SetEPRxStatus(ENDP2, EP_RX_VALID);
    }
    else
    {
      Bot_Abort(DIR_IN);
      Set_Scsi_Sense_Data(ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
      Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    }
    return;
  }
  else if (Bot_State == BOT_DATA_OUT)
  {
    Write_Memory();
  }
}

/*******************************************************************************
* Function Name  : SCSI_Verify10_Cmd
* Description    : SCSI Verify10 Command routine.
*******************************************************************************/
void SCSI_Verify10_Cmd(void)
{
  if ((CBW.dDataLength == 0) && !(CBW.CB[1] & BLKVFY))/* BLKVFY not set*/
  {
    Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
  }
  else
  {
    Bot_Abort(BOTH_DIR);
    Set_Scsi_Sense_Data(ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
  }
}

/*******************************************************************************
* Function Name  : SCSI_Valid_Cmd
* Description    : Valid Commands routine.
*******************************************************************************/
void SCSI_Valid_Cmd(void)
{
  if (CBW.dDataLength != 0)
  {
    Bot_Abort(BOTH_DIR);
    Set_Scsi_Sense_Data(ILLEGAL_REQUEST, INVALID_COMMAND);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
  }
  else
    Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
}

/*******************************************************************************
* Function Name  : SCSI_Invalid_Cmd
* Description    : Invalid Commands routine
*******************************************************************************/
void SCSI_Invalid_Cmd(void)
{
  if (CBW.dDataLength == 0)
  {
    Bot_Abort(DIR_IN);
  }
  else
  {
    if ((CBW.bmFlags & 0x80) != 0)
    {
      Bot_Abort(DIR_IN);
    }
    else
    {
      Bot_Abort(BOTH_DIR);
    }
  }
  Set_Scsi_Sense_Data(ILLEGAL_REQUEST, INVALID_COMMAND);
  Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
}

/*******************************************************************************
* Function Name  : SCSI_TestUnitReady_Cmd
* Description    : Valid Commands routine.
*******************************************************************************/
void SCSI_TestUnitReady_Cmd(void)
{
  if (!SD_Card_Check())
  {
    Set_Scsi_Sense_Data(NOT_READY, MEDIUM_NOT_PRESENT);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
    Bot_Abort(DIR_IN);
    return;
  }
  else if (SD_Is_Changed())
  {
    Set_Scsi_Sense_Data(UNIT_ATTENTION, MEDIUM_CHANGED);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_ENABLE);
    Bot_Abort(DIR_IN);
    return;
  }
  else
  {
    Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
  }
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
