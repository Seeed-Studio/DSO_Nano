/*******************************************************************************
 File Name: lcd.h      
*******************************************************************************/
#ifndef __LCD_H
#define __LCD_H

//============================ LCD dimensions ================================== 
#define LCD_WIDTH   320
#define LCD_HEIGHT  240
#define LCD_X1      0
#define LCD_X2      (LCD_WIDTH - LCD_X1 - 1)
#define LCD_Y1      0
#define LCD_Y2      (LCD_HEIGHT - LCD_Y1 - 1)
#define X_SIZE      300
#define Y_SIZE      200
#define MIN_X       1
#define MIN_Y       20
#define MAX_X       (MIN_X + X_SIZE - 1)
#define MAX_Y       (MIN_Y + Y_SIZE - 1)


//======================= display color mapping ================================ 
// R = 5 bits 0:4, G = 6 bits 5:10, B = 5 bits 11:15
#define RGB(_R,_G,_B) (((_R & 0x3E) >> 1) | ((_G & 0x3f) << 5) | ((_B & 0x3e) << 10))

#define C_GROUP     0x1082  // 00010 000100 00010 
#define F_SELEC     0x18E3  // 00011 000111 00011 
#define WAV_FLAG    0x0080
#define CH2_FLAG    0x0002
#define REF_FLAG    0x1000
#define LN1_FLAG    0x0020   
#define LN2_FLAG    0x0800   
#define GRD_FLAG    0x0040   
#define LN1_COLOR   (RGB(32,63,32) & ~F_SELEC) | LN1_FLAG
#define LN2_COLOR   (RGB(63,63,63) & ~F_SELEC) | LN2_FLAG
#define GRD_COLOR   (RGB(32,32,32) & ~F_SELEC) | GRD_FLAG
#define WAV_COLOR   (RGB(0,63,63) & ~F_SELEC) | WAV_FLAG
#define CH2_COLOR   (RGB(63,63,0) & ~F_SELEC) | CH2_FLAG
#define REF_COLOR   (RGB(63,0,63) & ~F_SELEC) | REF_FLAG

//========================= color definitions ================================== 
#define YEL         RGB(63,63,0)
#define WHITE       RGB(63,63,63)
#define RED         RGB(63,0,0)
#define GRN         RGB(0,63,0)
#define BLACK       RGB(0,0,0)

#define FRM_COLOR   RGB(24,24,24)
#define BACKGROUND  BLACK

//========================= constant definitions ===============================

#define PRN		0 
#define INV		1 //inverse
#define ADD		0 //add
#define ERASE		1 //erase

extern unsigned const int Logo_Dot[512];

//===================== fuction declarations ===================================
void LCD_WR_REG(unsigned short Reg, unsigned short Data);
void LCD_SET_WINDOW(short x1, short x2,short y1, short y2); 
void Point_SCR(unsigned short x0, unsigned short y0);
void Set_Pixel(unsigned short Color);
void Clear_Screen(unsigned short Color);
void Display_Frame(void);						
void Display_Grid(void); 

void Fill_Rectangle(short x0, short y0, short width, short height, short Color);
void Rounded_Rectangle(short x0, short y0, short width, short height, short Color);
void Display_Info(unsigned short x0, unsigned short y0, char *Pre, long Num);
void Display_Str(short x0, short y0, short Color, char Mode, unsigned const char *s);
void Erase_SEG(unsigned short x, unsigned short y1, unsigned short y2, unsigned short Color);
void Draw_SEG(unsigned short x, unsigned short y1, unsigned short y2, unsigned short Color);
void Draw_Trig_Pos(void);
void Erase_Trig_Pos(void);

void Draw_Vt_Line(unsigned short Vt, char Mode, unsigned short Color);
void Draw_Vi_Line(unsigned short Vi, char Mode, unsigned short Color);
void Draw_Ti_Line(unsigned short Ti, char Mode, unsigned short Color);
void Draw_Ti_Mark(unsigned short Ti, char Mode, unsigned short Color);
void Draw_Vi_Mark(unsigned short Vi, char Mode, unsigned short Color);

extern unsigned short frm_col;

#endif

/********************************* END OF FILE ********************************/
