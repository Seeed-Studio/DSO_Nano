/*******************************************************************************
* File Name: memory.h
*******************************************************************************/
#ifndef __memory_H
#define __memory_H

#include "usb_lib.h"

void SD_Set_Changed(void);
u8 SD_Is_Changed(void);
u8 SD_Card_Check(void);
void Read_Memory(void);
void Write_Memory(void);
bool Address_Management_Test(u8 Cmd);

#endif 

/********************************* END OF FILE ********************************/
