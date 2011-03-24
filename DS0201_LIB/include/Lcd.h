/*******************************************************************************
* File Name: lcd.h
*******************************************************************************/
#ifndef __LCD_H
#define __LCD_H

//============================== Color definitions =============================

#define RGB(_R,_G,_B) (((_R & 0x3E) >> 1) | ((_G & 0x3f) << 5) | ((_B & 0x3e) << 10))

#define YEL         RGB(63,63,0)
#define WHITE       RGB(63,63,63)
#define RED         RGB(63,0,0)
#define GRN         RGB(0,63,0)
#define BLUE        RGB(0,0,63)
#define BLACK       RGB(0,0,0)

//================================= Display MOde ===============================

#define PRN		0   // normal
#define INV		1   // inverse

//========================== Forward function delclaratons =====================

void LCD_WR_REG(unsigned short Reg, unsigned short Data);
void LCD_Initial(void); 
void Point_SCR(unsigned short x0, unsigned short y0);
void Set_Pixel(unsigned short Color);
void Clear_Screen(unsigned short Color);

unsigned short Get_Font_8x14(unsigned char chr, unsigned char row);
unsigned char Get_Ref_Wave(unsigned short idx);

void Display_Str(unsigned short xi, unsigned short yi, unsigned short Color, unsigned char Mode, unsigned const char *s);
void Display_Info(unsigned short x0, unsigned short y0, char *Pre, long Num);
void Display_Logo(unsigned short Dot_x, unsigned short Dot_y);

#endif

/********************************* END OF FILE ********************************/
