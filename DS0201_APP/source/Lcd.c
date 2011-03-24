/*******************************************************************************
File Name: lcd.c      
*******************************************************************************/
#include "Function.h"
#include "Menu.h"
#include "lcd.h"
#include "stm32f10x_it.h"
#include "HW_V1_Config.h"
#include "ASM_Function.h"

unsigned short frm_col = BACKGROUND;

/*******************************************************************************
 LCD_WR_REG: Set LCD Register  Input: Register addr., Data
*******************************************************************************/
void LCD_WR_REG(unsigned short Reg, unsigned short Data) 
{
  LDC_DATA_OUT=Reg;     //Reg. Addr.
  LCD_RS_LOW();         //RS=0,Piont to Index Reg.
  LCD_nWR_ACT();        //WR Cycle from 1 -> 0 -> 1
  LCD_RS_HIGH();        //RS=1,Piont to object Reg.
  LDC_DATA_OUT=Data;    //Reg. Data 
  LCD_nWR_ACT();        //WR Cycle from 1 -> 0 -> 1
}

/*******************************************************************************
Function Name : LCD_SET_WINDOW
Description : use  (x1,y1) and (x2,y2) to set a rectangle  area
Para :  (x1,y1) and (x2,y2) 
*******************************************************************************/
void    LCD_SET_WINDOW(short x1, short x2, short y1, short y2)
{
   LCD_WR_REG(0x0050, y1);
   LCD_WR_REG(0x0051, y2);
   LCD_WR_REG(0x0052, x1);
   LCD_WR_REG(0x0053, x2);

   LCD_WR_REG(0x0020, y1);
   LCD_WR_REG(0x0021, x1);

   LDC_DATA_OUT = 0x0022; // Reg. Addr.

   LCD_RS_LOW(); // RS=0,Piont to Index Reg.

   LCD_nWR_ACT(); // WR Cycle from 1 -> 0 -> 1

   LCD_nWR_ACT(); // WR Cycle from 1 -> 0 -> 1

   LCD_RS_HIGH(); // RS=1,Piont to object Reg.

}
/*******************************************************************************
Point_SCR: Set display position   Input: X, Y 
*******************************************************************************/
void Point_SCR(unsigned short x0, unsigned short y0) 
{
  LCD_WR_REG(0x0020,y0);
  LCD_WR_REG(0x0021,x0);
  LDC_DATA_OUT=0x0022;  //DRAM Reg.      
  LCD_RS_LOW();             
  LCD_nWR_ACT();        //WR Cycle from 1 -> 0 -> 1
  LCD_nWR_ACT();        //WR Cycle from 1 -> 0 -> 1
  LCD_RS_HIGH();            
}
/*******************************************************************************
 Set_Pixel: Set a Pixel  Input: Color
*******************************************************************************/
void Set_Pixel(unsigned short Color) 
{
  LDC_DATA_OUT=Color;   //Color Data
  LCD_nWR_ACT();        //WR Cycle from 1 -> 0 -> 1
}
/*******************************************************************************
Function Name : Clear_Screen
Description : clear screen
*******************************************************************************/
void    Clear_Screen(unsigned short Color)
{
   Fill_Rectangle(LCD_X1, LCD_Y1, LCD_WIDTH, LCD_HEIGHT, Color);
}
/*******************************************************************************
Function Name : Display_Frame
Description : intialize backgrund for the top, bottom and right menu area
*******************************************************************************/
void    Display_Frame(void)
{
   // bottom frame
   Fill_Rectangle(LCD_X1, LCD_Y1, LCD_WIDTH, MIN_Y - 1, FRM_COLOR);

   // right frame
   Fill_Rectangle(MAX_X + 2, MIN_Y - 1, LCD_WIDTH - MAX_X - 1, LCD_HEIGHT - MIN_Y * 2 + 2, FRM_COLOR);

   // top frame
   Fill_Rectangle(LCD_X1, MAX_Y + 2, LCD_WIDTH, MIN_Y - 1, FRM_COLOR);
}
/*******************************************************************************
Function Name : Display_Grid
Description : draw the grid
*******************************************************************************/
void  Display_Grid(void)
{
   unsigned short  i, j;
   short n;

   // grid background + outside frame
   Fill_Rectangle(MIN_X - 1, MIN_Y - 1, X_SIZE + 2, Y_SIZE + 2, BACKGROUND);
   
   // horizontal frame
   for (i = MIN_X; i <= MAX_X; i++) {
      Point_SCR(i, MIN_Y);
      Set_Pixel(GRD_COLOR);
      Point_SCR(i, MAX_Y);
      Set_Pixel(GRD_COLOR);
     if ((((i - MIN_X) % 5) == 0) && (i != MIN_X) && (i != MAX_X)) {
       for (n = 1; n <= 2; n++) {
          Point_SCR(i, MIN_Y + n);
          Set_Pixel(GRD_COLOR);
          Point_SCR(i, MAX_Y - n);
          Set_Pixel(GRD_COLOR);
       }
     }
   }

   // vertical frame
   for (j = MIN_Y + 1; j <= (MAX_Y - 1); j++) {
      Point_SCR(MIN_X, j);
      Set_Pixel(GRD_COLOR);
      Point_SCR(MAX_X, j);
      Set_Pixel(GRD_COLOR);
     if (((j - MIN_Y) % 5) == 0) {
       for (n = 1; n <= 2; n++) {
          Point_SCR(MIN_X + n, j);
          Set_Pixel(GRD_COLOR);
          Point_SCR(MAX_X - n, j);
          Set_Pixel(GRD_COLOR);
       }
     }
   }

   // horizontal grid lines
   for (j = MIN_Y + 25; j <= (MAX_Y - 25 + 1); j += 25)
   {
      for (i = MIN_X + 5; i <= (MAX_X - 5 + 1); i += 5)
      {
         if (j == (MIN_Y + Y_SIZE / 2)) {
           for (n = -1; n <= 1; n++) {
             Point_SCR(i, j + n);
             Set_Pixel(GRD_COLOR);
           }
         } else {
           Point_SCR(i, j);
           Set_Pixel(GRD_COLOR);
         }
      }
   }
   // vertical grid lines
   for (i = MIN_X + 25; i <= (MAX_X - 25 + 1); i += 25)
   {
      for (j = MIN_Y + 5; j <= (MAX_Y - 5 + 1); j += 5)
      {
         if (i == (MIN_X + X_SIZE / 2)) {
           for (n = -1; n <= 1; n++) {
             Point_SCR(i + n, j);
             Set_Pixel(GRD_COLOR);
           }
         } else {
           Point_SCR(i, j);
           Set_Pixel(GRD_COLOR);
         }
      }
   }
}
/*******************************************************************************
Function Name : Draw_SEG
Description : draw a vertical segment
Para : x is the horizontal coordinate, |y1-y2| is the segment heigth, Color
*******************************************************************************/
void Draw_SEG(unsigned short x, unsigned short y1, unsigned short y2, unsigned short Color)
{
   unsigned short  j, clip = 0;

   if (y1 == 0xff) y1 = y2;
   if (y2 == 0xff) y2 = y1;
   if (y1 == 0xff) return;
   
   if (y1 > y2)
   {
      unsigned short t = y2;
      y2 = y1;
      y1 = t;
   }
   if (y2 >= MAX_Y)
      y2 = MAX_Y - 1;
   if (y1 <= MIN_Y)
      y1 = MIN_Y + 1;

   x += MIN_X;
   if (Popup.Active && (x >= Popup.x) && (x < (Popup.x + Popup.width))) clip = 1;
   for (j = y1; j <= y2; j++) {
      if (clip && (j >= Popup.y) && (j < (Popup.y + Popup.height))) continue;
      __Add_Color(x, j, Color);
   }
}
/*******************************************************************************
Function Name : Erase_SEG
Description : use the Color to erase one line segment
*******************************************************************************/
void Erase_SEG(unsigned short x,unsigned short y1,unsigned short y2,unsigned short Color)
{
   unsigned short  j, clip = 0;

   if (y1 == 0xff) y1 = y2;
   if (y2 == 0xff) y2 = y1;
   if (y1 == 0xff) return;

   if (y1 > y2)
   {
      unsigned short t = y2;
      y2 = y1;
      y1 = t;
   }
   if (y2 >= MAX_Y)
      y2 = MAX_Y - 1;
   if (y1 <= MIN_Y)
      y1 = MIN_Y + 1;

   x += MIN_X;
   if (Popup.Active && (x >= Popup.x) && (x < (Popup.x + Popup.width))) clip = 1;
   for (j = y1; j <= y2; j++) {
     if (clip && (j >= Popup.y) && (j < (Popup.y + Popup.height))) continue;
     __Erase_Color(x, j, Color); //  erase the wave curve 
   }

}
/*******************************************************************************
Function Name : Draw_Vt_Line
Description : draw or erase trigger/sensitivity line
*******************************************************************************/
void Draw_Vt_Line(unsigned short Vt, char Mode, unsigned short Color)
{
   unsigned short  i, clip = 0;

   if (Popup.Active && (Vt >= Popup.y) && (Vt < (Popup.y + Popup.height))) clip = 1;
   for (i = MIN_X + 2; i < MAX_X; i += 2)
   {
      if (clip && (i >= Popup.x) && (i < (Popup.x + Popup.width))) continue;
      if (Mode == ADD)
         __Add_Color(i, Vt, Color);
      else
         __Erase_Color(i, Vt, Color);
   }
}
void Draw_Vi_Line(unsigned short Vi, char Mode, unsigned short Color)
{
   unsigned short  i, clip = 0;

   if (Popup.Active && (Vi >= Popup.y) && (Vi < (Popup.y + Popup.height))) clip = 1;
   for (i = MIN_X + 5; i < MAX_X; i += 5)
   {
      if (clip && (i >= Popup.x) && (i < (Popup.x + Popup.width))) continue;
      if (Mode == ADD)
         __Add_Color(i, Vi, Color);
      else
         __Erase_Color(i, Vi, Color);
   }
}
void Draw_Ti_Line(unsigned short Ti, char Mode, unsigned short Color)
{
   unsigned short  j, clip = 0;

   if (Popup.Active && (Ti >= Popup.x) && (Ti < (Popup.x + Popup.width))) clip = 1;
   for (j = MIN_Y + 3; j < MAX_Y; j += 3)
   {
      if (clip && (j >= Popup.y) && (j < (Popup.y + Popup.height))) continue;
      if (Mode == ADD)
         __Add_Color(Ti, j, Color);
      else
         __Erase_Color(Ti, j, Color);
   }
}
/*******************************************************************************
Function Name : Draw_Vi_Mark
Description : draw or erase Voltage marker
*******************************************************************************/
void     Draw_Vi_Mark(unsigned short Vi, char Mode, unsigned short Color)
{
   if (Mode == ADD)
   {
      __Add_Color(MIN_X - 1, Vi - 2, Color);
      __Add_Color(MIN_X - 1, Vi - 1, Color);
      __Add_Color(MIN_X - 1, Vi, Color);
      __Add_Color(MIN_X - 1, Vi + 1, Color);
      __Add_Color(MIN_X - 1, Vi + 2, Color);

      __Add_Color(MIN_X, Vi - 1, Color);
      __Add_Color(MIN_X, Vi, Color);
      __Add_Color(MIN_X, Vi + 1, Color);

      __Add_Color(MIN_X + 1, Vi, Color);

      __Add_Color(MAX_X + 1, Vi - 2, Color);
      __Add_Color(MAX_X + 1, Vi - 1, Color);
      __Add_Color(MAX_X + 1, Vi, Color);
      __Add_Color(MAX_X + 1, Vi + 1, Color);
      __Add_Color(MAX_X + 1, Vi + 2, Color);

      __Add_Color(MAX_X, Vi - 1, Color);
      __Add_Color(MAX_X, Vi, Color);
      __Add_Color(MAX_X, Vi + 1, Color);

      __Add_Color(MAX_X - 1, Vi, Color);
   }
   else
   {
      __Erase_Color(MIN_X - 1, Vi - 2, Color); // erase the left triangle
      __Erase_Color(MIN_X - 1, Vi - 1, Color);
      __Erase_Color(MIN_X - 1, Vi, Color);
      __Erase_Color(MIN_X - 1, Vi + 1, Color);
      __Erase_Color(MIN_X - 1, Vi + 2, Color);

      __Erase_Color(MIN_X, Vi - 1, Color);
      __Erase_Color(MIN_X, Vi, Color);
      __Erase_Color(MIN_X, Vi + 1, Color);

      __Erase_Color(MIN_X + 1, Vi, Color);

      __Erase_Color(MAX_X + 1, Vi - 2, Color); // erase the right triangle
      __Erase_Color(MAX_X + 1, Vi - 1, Color);
      __Erase_Color(MAX_X + 1, Vi, Color);
      __Erase_Color(MAX_X + 1, Vi + 1, Color);
      __Erase_Color(MAX_X + 1, Vi + 2, Color);

      __Erase_Color(MAX_X, Vi - 1, Color);
      __Erase_Color(MAX_X, Vi, Color);
      __Erase_Color(MAX_X, Vi + 1, Color);

      __Erase_Color(MAX_X - 1, Vi, Color);

      __Erase_Color(MIN_X - 1, Vi - 2, Color);
      __Erase_Color(MIN_X - 1, Vi - 1, Color);
      __Erase_Color(MIN_X - 1, Vi, Color);
      __Erase_Color(MIN_X - 1, Vi + 1, Color);
      __Erase_Color(MIN_X - 1, Vi + 2, Color);
   }
}
/***************************************************************************
Function Name : Draw_Ti_Mark
Description : draw or erase Time marker
*****************************************************************************/

void Draw_Ti_Mark(unsigned short Ti, char Mode, unsigned short Color)
{
   if (Mode == ADD)
   {
      __Add_Color(Ti - 2, MIN_Y - 1, Color);
      __Add_Color(Ti - 1, MIN_Y - 1, Color);
      __Add_Color(Ti, MIN_Y - 1, Color);
      __Add_Color(Ti + 1, MIN_Y - 1, Color);
      __Add_Color(Ti + 2, MIN_Y - 1, Color);

      __Add_Color(Ti - 1, MIN_Y, Color);
      __Add_Color(Ti, MIN_Y, Color);
      __Add_Color(Ti + 1, MIN_Y, Color);

      __Add_Color(Ti, MIN_Y + 1, Color);

      __Add_Color(Ti - 2, MAX_Y + 1, Color);
      __Add_Color(Ti - 1, MAX_Y + 1, Color);
      __Add_Color(Ti, MAX_Y + 1, Color);
      __Add_Color(Ti + 1, MAX_Y + 1, Color);
      __Add_Color(Ti + 2, MAX_Y + 1, Color);

      __Add_Color(Ti - 1, MAX_Y, Color);
      __Add_Color(Ti, MAX_Y, Color);
      __Add_Color(Ti + 1, MAX_Y, Color);

      __Add_Color(Ti, MAX_Y - 1, Color);
   } 
   else
   {
      __Erase_Color(Ti - 2, MIN_Y - 1, Color);
      __Erase_Color(Ti - 1, MIN_Y - 1, Color);
      __Erase_Color(Ti, MIN_Y - 1, Color);
      __Erase_Color(Ti + 1, MIN_Y - 1, Color);
      __Erase_Color(Ti + 2, MIN_Y - 1, Color);

      __Erase_Color(Ti - 1, MIN_Y, Color);
      __Erase_Color(Ti, MIN_Y, Color);
      __Erase_Color(Ti + 1, MIN_Y, Color);

      __Erase_Color(Ti, MIN_Y + 1, Color);

      __Erase_Color(Ti - 2, MAX_Y + 1, Color);
      __Erase_Color(Ti - 1, MAX_Y + 1, Color);
      __Erase_Color(Ti, MAX_Y + 1, Color);
      __Erase_Color(Ti + 1, MAX_Y + 1, Color);
      __Erase_Color(Ti + 2, MAX_Y + 1, Color);

      __Erase_Color(Ti - 1, MAX_Y, Color);
      __Erase_Color(Ti, MAX_Y, Color);
      __Erase_Color(Ti + 1, MAX_Y, Color);

      __Erase_Color(Ti, MAX_Y - 1, Color);

      __Erase_Color(Ti - 2, MIN_Y - 1, Color);
      __Erase_Color(Ti - 1, MIN_Y - 1, Color);
      __Erase_Color(Ti, MIN_Y - 1, Color);
      __Erase_Color(Ti + 1, MIN_Y - 1, Color);
      __Erase_Color(Ti + 2, MIN_Y - 1, Color);
   }
}
/*******************************************************************************
Function Name : Erase_Trig_Pos
Description : erase trigger position indicator
*******************************************************************************/
void      Erase_Trig_Pos(void)
{
   short  i;

   for (i = MIN_X + 2; i <= (MAX_X - 2); ++i)
   {
      __Erase_Color(i, MIN_Y + 2, CH2_COLOR);
      __Erase_Color(i, MIN_Y + 3, CH2_COLOR);
      __Erase_Color(i, MIN_Y + 4, CH2_COLOR);
      __Erase_Color(i, MIN_Y + 5, CH2_COLOR);

      __Erase_Color(i, MIN_Y + 3, LN2_COLOR);
      __Erase_Color(i, MIN_Y + 4, LN2_COLOR);
   }
}
/*******************************************************************************
Function Name : Draw_Trig_Pos
Description : draw trigger position indicator
*******************************************************************************/
void      Draw_Trig_Pos(void)
{
   short  i, j;

   for (i = MIN_X + 2; i <= (MAX_X - 2); i++)
   {
      __Add_Color(i, MIN_Y + 3, LN2_COLOR);;
      __Add_Color(i, MIN_Y + 4, LN2_COLOR);
   }

   j = MIN_X + 150 + (((Item_Index[TP] - BUFFER_SIZE) * X_SIZE) / (SEGMENT_SIZE*2));

   for (i = MIN_X + 2; i <= (MAX_X - 2); i++)
   {
      if ((i >= (j-10)) && (i <= (j + 10)))
      {
         __Add_Color(i, MIN_Y + 2, CH2_COLOR);
         __Add_Color(i, MIN_Y + 3, CH2_COLOR);
         __Add_Color(i, MIN_Y + 4, CH2_COLOR);
         __Add_Color(i, MIN_Y + 5, CH2_COLOR);
      } 
	  else
      {
         __Erase_Color(i, MIN_Y + 2, CH2_COLOR);
         __Erase_Color(i, MIN_Y + 3, CH2_COLOR);
         __Erase_Color(i, MIN_Y + 4, CH2_COLOR);
         __Erase_Color(i, MIN_Y + 5, CH2_COLOR);
      }
   }
}

void Fill_Rectangle(short x0, short y0, short width, short height, short Color)
{
  int i;

  LCD_SET_WINDOW(x0, x0 + width - 1, y0, y0 + height - 1); // limit write to rectangle

  for (i = 0; i < width * height; i++)
     Set_Pixel(Color);
    
  LCD_SET_WINDOW(LCD_X1, LCD_X2, LCD_Y1, LCD_Y2); // restore full screen
}

void Rounded_Rectangle(short x0, short y0, short width, short height, short Color)
{
  short i;

  Fill_Rectangle(x0 + 1,  y0 + 1, width - 2, height - 2, Color);
  
  // draw inside frame
  for (i = x0 + 2; i < (x0 + width - 2); i++) {
     Point_SCR(i, y0);
     Set_Pixel(WHITE);
     Point_SCR(i, y0 + height - 1);
     Set_Pixel(WHITE);
  }
  for (i = y0 + 2; i < (y0 + height - 2); i++) {
     Point_SCR(x0, i);
     Set_Pixel(WHITE);
     Point_SCR(x0 + width - 1, i);
     Set_Pixel(WHITE);
  }
  Point_SCR(x0 + 1, y0 + 1);
  Set_Pixel(WHITE);
  Point_SCR(x0 + width - 2, y0 + 1);
  Set_Pixel(WHITE);

  Point_SCR(x0 + 1, y0 + height - 2);
  Set_Pixel(WHITE);
  Point_SCR(x0 + width - 2, y0 + height - 2);
  Set_Pixel(WHITE);
}

/*******************************************************************************
Function Name : Display_Str
Description : print one string in the specific  position
Para : (x0,y0) is the coordinate of the start point of the string
       Color is the string color
       Mode=PRN Normal replace Display, Mode=INV Inverse replace Display
       s is the string
*******************************************************************************/
void      Display_Str(short x0, short y0, short Color, char Mode, unsigned const char *s)
{
   short     i, j, b;

   LCD_SET_WINDOW(x0, LCD_X2, y0, y0 + 13);
   while (*s)
   {
      b = 0;
      for (i = 0; i < 8; i++)
      {
         if (*s >= 0x22)
           b = pLib->Get_Font_8x14(*s, i);
         for (j = 0; j < 12; j++)       //means the char height pixel
         {
            if (b & 4)
              Set_Pixel(Mode?frm_col:Color);
	    else
              Set_Pixel(Mode?Color:frm_col);
            b >>= 1;
         }
         Set_Pixel(Mode?Color:frm_col);
         Set_Pixel(Mode?Color:frm_col);
      }
      ++s; // the pointer of the string will add one

   }
   LCD_SET_WINDOW(LCD_X1, LCD_X2, LCD_Y1, LCD_Y2); // Restore full screen

}

void Display_Info(unsigned short x0, unsigned short y0, char *Pre, long Num)
{
  char  buf[10], n = 0, k, str[2] = {'-', 0};
  
  if (Pre) {
    char *p = Pre;
    while (*p) p++;
    Display_Str(x0, y0, WHITE, PRN, (unsigned char const *)Pre);
    x0 += (p - Pre) * 8 + 8;
  }

  if (Num < 0) {
    Display_Str(x0, y0, WHITE, PRN, (unsigned char const *)str);
    x0 += 8;
    Num = -Num;
  }

  do {  
    buf[n++] = Num % 10;
    Num /= 10;
  } while (Num > 0);

  k = n;
  
  for (; n > 0; n--) {
    str[0] = '0' + buf[n - 1];
    Display_Str(x0, y0, WHITE, PRN, (unsigned char const *)str);
    x0 += 8;
  }
  
  str[0] = ' ';
  while (k < 10) {
    Display_Str(x0, y0, WHITE, PRN, (unsigned char const *)str);
    x0 += 8;
    k++;
  }
}
/******************************** END OF FILE *********************************/
