/*******************************************************************************
 file name  : Interface.h
 *******************************************************************************/

#ifndef __INTERFACE_H
#define __INTERFACE_H

#define APP_VECTORS 0x0800C000
#define LIB_VECTORS 0x08004000

#define APP_SIGNATURE 0xFaFa0313
#define LIB_SIGNATURE 0xaFaF0313

#define LIB_STACK_SIZE 0x1000

typedef struct {
  unsigned long Signature;
  void (*APP_Start)(void);
} APP_Interface;

typedef struct {
  unsigned long Signature;
  void (*USB_Istr)(void);
  void (*CTR_HP)(void);
  void (*SD_Set_Changed)(void);
  unsigned char (*MSD_WriteBlock)(unsigned char *pBuffer, unsigned long WriteAddr, unsigned short NumByteToWrite);
  unsigned char (*MSD_ReadBlock)(unsigned char *pBuffer, unsigned long ReadAddr, unsigned short NumByteToRead);
  unsigned short (*GetFont_8x14)(unsigned char chr, unsigned char row);
  unsigned char (*Get_Ref_Wave)(unsigned short idx);
} LIB_Interface;

#endif
                       
//****************************** END OF FILE ***********************************
