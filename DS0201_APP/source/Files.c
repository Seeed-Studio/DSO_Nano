/*******************************************************************************
File Name: files.c  
*******************************************************************************/
#include "Lcd.h"
#include "Files.h"
#include "Function.h"
#include "Menu.h"
#include "stm32f10x_flash.h"
#include "string.h"
#include "HW_V1_Config.h"
#include "ASM_Function.h"

#define Page_Address    0x0801F000 //start address of the stm32 flash

unsigned const char BmpHead[54] = {
   0x42, 0x4D, 0x36, 0x58, 0x02, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x36, 0x00, 0x00, 0x00, 0x28, 0x00,
   0x00, 0x00, 0x40, 0x01, 0x00, 0x00, 0xF0, 0x00,
   0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x58, 0x02, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
unsigned int    Root_Addr, File_Addr, Dir_Addr, FDT_Start, FDT_Cluster, ClusterNum, FAT1_Addr;

unsigned short  SectorSize, SecPerClus, DirBlkNum, Dir_Offset;

unsigned char   FAT16, F_Buff[512];

/*******************************************************************************
Function Name : FAT_Info
Description : read the basic infomation of the SD card
*******************************************************************************/
char FAT_Info(void)
{
    unsigned int DiskStart;
    unsigned short RsvdSecCnt, FAT_Size, DIR_Size;
   
    // read partition table
    if (pLib->MSD_ReadBlock(F_Buff, 0, 512))
       return 0xFF; // Disk error!

    // find first sector in first partition
    DiskStart = (F_Buff[0x1C9] << 24) + (F_Buff[0x1C8] << 16)
       + (F_Buff[0x1C7] << 8) + F_Buff[0x1C6];
    DiskStart = DiskStart * 512;
    // read first sector in partition (FAT table)
    if (pLib->MSD_ReadBlock(F_Buff, DiskStart, 512))
       return 0xFF; // Disk error!

   SectorSize = (F_Buff[0x0C] << 8) + F_Buff[0x0B];
   SecPerClus = F_Buff[0x0D];
   RsvdSecCnt = (F_Buff[0x0F] << 8) + F_Buff[0x0E];
   DIR_Size = (F_Buff[0x12] << 8) + F_Buff[0x11];
   if (DIR_Size) {
      FAT16 = 1;  // FAT16
      FAT_Size = (F_Buff[0x17] << 8) + F_Buff[0x16];
   } else {
      FAT16 = 0;  // FAT32
      FAT_Size = (F_Buff[0x25] << 8) + F_Buff[0x24];
   }
   FAT1_Addr = DiskStart + RsvdSecCnt * SectorSize;
   Root_Addr = FAT1_Addr + F_Buff[0x10] * FAT_Size * SectorSize;
   if (FAT16) {
      DirBlkNum = DIR_Size / (512 / 32); // 32 bytes per dir entry
   } else {
      FDT_Cluster = (F_Buff[0x2f] << 24) + (F_Buff[0x2e] << 16)
                   + (F_Buff[0x2d] << 8) + F_Buff[0x2c];
      FDT_Start = Root_Addr + (FDT_Cluster - 2) * SecPerClus * SectorSize;
   }

   return 0; // Disk ok!

}

/*******************************************************************************
Function Name : SetClusterNext
Description : Point File_Addr to next FAT cluster in current file
*******************************************************************************/
unsigned char SetClusterNext(void)
{
  unsigned short i;
  unsigned int addr = FAT1_Addr;

  if (FAT16) {
    addr += 512 * (ClusterNum / (512 / 2));
    i = ClusterNum % (512 / 2) * 2;
  } else {
    addr += 512 * (ClusterNum / (512 / 4));
    i = ClusterNum % (512 / 4) * 4;
  }
  
  if (pLib->MSD_ReadBlock(F_Buff, addr, 512))
     return 0xff;

  if (FAT16) {
    ClusterNum = (F_Buff[i + 1] << 8) + F_Buff[i];
    if (ClusterNum >= 0xfff0) return 0xff;
    File_Addr = Root_Addr + DirBlkNum * 512 + (ClusterNum - 2) * SecPerClus * SectorSize;
  } else {    
    ClusterNum = ((F_Buff[i + 3] & 0x0f) << 24) + (F_Buff[i + 2] << 16) 
               + (F_Buff[i + 1] << 8) + F_Buff[i];
    if (ClusterNum >= 0x0ffffff0) return 0xff;
    File_Addr = Root_Addr + (ClusterNum - 2) * SecPerClus * SectorSize;
  }

  return 0;
}

unsigned char DirMatch(unsigned const char *p1, unsigned const char *p2, unsigned short n)
{
  unsigned short i;
  
  for (i=0; i < n; i++) {
    if (p1[i] > p2[i]) {
      if ((p1[i] - ('a' - 'A')) != p2[i]) return 0;
    } else if (p1[i] < p2[i]) {
      if ((p2[i] - ('a' - 'A')) != p1[i]) return 0;
    }
  }
  return 1;
}

/*******************************************************************************
Function Name : DirTouch
Description : Update directory timestamp
*******************************************************************************/
char DirTouch(void)
{
  if (pLib->MSD_ReadBlock(F_Buff, Dir_Addr, 512))
     return 0xff;
  
  F_Buff[Dir_Offset + 0x16] += 1;

  return pLib->MSD_WriteBlock(F_Buff, Dir_Addr, 512);
}

/*******************************************************************************
Function Name : Open_File
Description : find an open file
Para :  name is prefix, n is the number part of name, ext is extension part of name
*******************************************************************************/
char Open_File(unsigned const char *name, unsigned char *num, unsigned const char *ext)
{
   unsigned short  i, j = strlen((char const *)name), n = 0;

   File_Addr = FDT_Start;     // address of first FAT32 directory block
   ClusterNum = FDT_Cluster;  // cluster number of FDT_Start
   while (1)
   {
      if (FAT16)
        Dir_Addr = Root_Addr;
      else
        Dir_Addr = File_Addr;

      Dir_Addr += n * 512;
      if (pLib->MSD_ReadBlock(F_Buff, Dir_Addr, 512))
         return 0xff; // Read Sector fail

      for (i = 0; i < 512; i += 32)
      {
         if (F_Buff[i + 0x0b] & 0x1f) continue; // skip directory, volume label, system,  hidden, read only
         if (DirMatch(F_Buff + i, name, j) && DirMatch(F_Buff + i + 8, ext, 3) && DirMatch(F_Buff + i + j, num, 3))
         {
            unsigned int FileSize;
            FileSize = (F_Buff[i + 0x1f] << 24) + (F_Buff[i + 0x1e] << 16)
                        + (F_Buff[i + 0x1d] << 8) + (F_Buff[i + 0x1c]);
            if (FileSize == 0)
              return 0xff;  // file exists, but is empty
      
            if (FAT16) {
               ClusterNum = (F_Buff[i + 0x1B] << 8) + F_Buff[i + 0x1A];
               File_Addr = Root_Addr + DirBlkNum * 512 + (ClusterNum - 2) * SecPerClus * SectorSize;
            } else {
               ClusterNum = (F_Buff[i + 0x15] << 24) + (F_Buff[i + 0x14] << 16)
                          + (F_Buff[i + 0x1B] << 8) + F_Buff[i + 0x1A];
               File_Addr = Root_Addr + (ClusterNum - 2) * SecPerClus * SectorSize;
            }

            Dir_Offset = i;
            return 0; // Open BMP File ok!
         }
      }
      n++;
      if (FAT16) {
        if (n >= DirBlkNum) break;
      } else {
        if (n >= (SectorSize/512*SecPerClus)) {
          if (SetClusterNext()) break;
          n = 0;
        }
      }
   }
   return 0xff; // file not found
}
/*******************************************************************************
Function Name : Writ_BMP_File
Description : write the opened image file 
*******************************************************************************/
char     Writ_BMP_File(void)
{
   unsigned short  x, y, i = sizeof(BmpHead), n = 0, R, G, B, Color;
   char rval = 0;

   memcpy(F_Buff, BmpHead, sizeof(BmpHead));
   for (y = 0; y < LCD_HEIGHT; ++y)
   {
      for (x = 0; x < LCD_WIDTH; ++x)
      {
         Color = __Get_Pixel(x, y);
         R = Color & 0x001F;
         G = (Color & 0x07E0) >> 6;
         B = (Color & 0xF800) >> 11;
         Color = (R << 10) | (G << 5) | B;
         F_Buff[i] = (char)Color;
         F_Buff[i + 1] = (char)(Color >> 8);
         i += 2;
         if (i >= 512)
         { // Buffer full
            if (pLib->MSD_WriteBlock(F_Buff, File_Addr, 512))
               return 0xFF; // File Write Error

            i = 0;
            File_Addr += 512;
            n++;
            if (n >= (SectorSize/512*SecPerClus)) {
              if (SetClusterNext())
                return 0xFF;  // insufficient space
              n = 0;
            }
         }
      }
   }
   if (i > 0)
     rval = pLib->MSD_WriteBlock(F_Buff, File_Addr, 512);
   
   pLib->SD_Set_Changed();
   return rval;
}
/*******************************************************************************
Function Name : Read_File
Description : read the specified file
*******************************************************************************/
char            Read_File(void)
{
   return pLib->MSD_ReadBlock(F_Buff, File_Addr, 512);
}
/*******************************************************************************
Function Name : Write_File
Description : write the specified file and mark the sequence number
*******************************************************************************/
char            Write_File(void)
{
   char rval = pLib->MSD_WriteBlock(F_Buff, File_Addr, 512);
   
   pLib->SD_Set_Changed();
   return rval;
}

/*******************************************************************************
Function Name : Read_Parameter
Description : read the user setting parameter from flash
*******************************************************************************/
void            Read_Parameter(void)
{
   unsigned short  i;

   for (i = 0; i < 128; i++)
      F_Buff[i] = *((vu8 *)(Page_Address + i));
}
/*******************************************************************************
Function Name : Write_Parameter
Description : write the items parameter to flash
*******************************************************************************/
char     Write_Parameter(void)
{
   unsigned short  i, j, k;

   FLASH_Unlock();
   j = FLASH_ErasePage(Page_Address);
   if (j == FLASH_COMPLETE)
   {
      for (i = 0; i < 128; i += 2)
      {
         k = F_Buff[i] + (F_Buff[i + 1] << 8);
         j = FLASH_ProgramHalfWord(Page_Address + i, k);
         if (j != FLASH_COMPLETE)
            break;
      }
   }
   FLASH_Lock();
   return j;
}

/******************************** END OF FILE *********************************/
