/*******************************************************************************
File Name: Calculate.c  
*******************************************************************************/

#include "Calculate.h"

/*******************************************************************************
 Function Name : Int32String_sign
 Description : convert signed 32 bit number to string
*******************************************************************************/
void Int32String_sign(I32STR_RES * r, int n, unsigned char e)
{
   unsigned int   m, c;
   unsigned char  i, fixlen;
   char           *p = r->str;

   fixlen = e + 2;
   if (n == 0)
   {
      *p++ = ' ';
      *p++ = '0';
      *p++ = '.';
      *p++ = '0';
      *p++ = '0';
      *p = 0;
      r->decPos = 0;
      r->len = p - r->str;
      return;
   }
   if (n > 0)
      *p++ = ' ';
   else
   {
      *p++ = '-';
      n = -n;
   }
   m = n;
   i = 0;
   c = 5;
   while (m >= 10)
   {
      m /= 10;
      if (++i > e)
         c *= 10;
   }
   if (i >= e) {
      n += c;
      m = n;
      i = 0;
      while (m >= 10) m /= 10, i++;
   }
   r->decPos = i / 3;
   switch (i)
   {
   case 9:
      *p++ = '0' + n / 1000000000;
      if (--e == 0)
         break;
      n %= 1000000000;
      *p++ = '.', i = 0;
   case 8:
      *p++ = '0' + n / 100000000;
      if (--e == 0)
         break;
      n %= 100000000;
   case 7:
      *p++ = '0' + n / 10000000;
      if (--e == 0)
         break;
      n %= 10000000;
   case 6:
      *p++ = '0' + n / 1000000;
      if (--e == 0)
         break;
      n %= 1000000;
      if (i)
         *p++ = '.', i = 0;
   case 5:
      *p++ = '0' + n / 100000;
      if (--e == 0)
         break;
      n %= 100000;
   case 4:
      *p++ = '0' + n / 10000;
      if (--e == 0)
         break;
      n %= 10000;
   case 3:
      *p++ = '0' + n / 1000;
      if (--e == 0)
         break;
      n %= 1000;
      if (i)
         *p++ = '.', i = 0;
   case 2:
      *p++ = '0' + n / 100;
      if (--e == 0)
         break;
      n %= 100;
   case 1:
      *p++ = '0' + n / 10;
      if (--e == 0)
         break;
      n %= 10;
   case 0:
      *p++ = '0' + n;
   }
   
   while (p < r->str + fixlen) {
     char *q = p;
     while (q > r->str) {
       *q = *(q - 1);
       q--;
     }
     *q = ' ';
     p++;
   }
//   while (p < r->str + fixlen)
//      *p++ = ' ';
   *p = 0;
   r->len = p - r->str;
}

/*******************************************************************************
 Function Name : PercentString
 Description : convert signed 16 bit number to string with 1 decimal precision
*******************************************************************************/
void PercentString(I32STR_RES * r, short n)
{
   char *p = r->str;
   char fraction = (n & 1) ? '5':'0';

   if (n < 0) {
     *(p++) = '-';
     n = -n;
   }
   n = n / 2;
   if (n >= 10) *(p++) = '0' + n / 10;
   *(p++) = '0' + (n % 10);
   *(p++) = '.';
   *(p++) = fraction;
   *p = 0;
}

/*******************************************************************************
 Function Name : Int32String
 Description : convert unsigned 32 bit number to string
*******************************************************************************/
void Int32String(I32STR_RES * r, unsigned int n, unsigned char e)
{
   unsigned int   m, c;
   unsigned char  i, fixlen;
   char           *p = r->str;

   fixlen = e + 1;
   if (n == 0)
   {
      *p++ = '0';
      *p++ = '.';
      *p++ = '0';
      *p++ = '0';
      //*p++ = '0';
      *p = 0;
      r->decPos = 0;
      r->len = p - r->str;
      return;
   }
   m = n;
   i = 0;
   c = 5;
   while (m >= 10)
   {
      m /= 10;
      if (++i > e)
         c *= 10;
   }
   if (i >= e) {
      n += c;
      m = n;
      i = 0;
      while (m >= 10) m /= 10, i++;
   }
   r->decPos = i / 3;
   switch (i)
   {
   case 9:
      *p++ = '0' + n / 1000000000;
      if (--e == 0)
         break;
      n %= 1000000000;
      *p++ = '.', i = 0;
   case 8:
      *p++ = '0' + n / 100000000;
      if (--e == 0)
         break;
      n %= 100000000;
   case 7:
      *p++ = '0' + n / 10000000;
      if (--e == 0)
         break;
      n %= 10000000;
   case 6:
      *p++ = '0' + n / 1000000;
      if (--e == 0)
         break;
      n %= 1000000;
      if (i)
         *p++ = '.', i = 0;
   case 5:
      *p++ = '0' + n / 100000;
      if (--e == 0)
         break;
      n %= 100000;
   case 4:
      *p++ = '0' + n / 10000;
      if (--e == 0)
         break;
      n %= 10000;
   case 3:
      *p++ = '0' + n / 1000;
      if (--e == 0)
         break;
      n %= 1000;
      if (i)
         *p++ = '.', i = 0;
   case 2:
      *p++ = '0' + n / 100;
      if (--e == 0)
         break;
      n %= 100;
   case 1:
      *p++ = '0' + n / 10;
      if (--e == 0)
         break;
      n %= 10;
   case 0:
      *p++ = '0' + n;
   }
   while (p < r->str + fixlen) {
     char *q = p;
     while (q > r->str) {
       *q = *(q - 1);
       q--;
     }
     *q = ' ';
     p++;
   }
//   while (p < r->str + fixlen)
//      *p++ = ' ';
   *p = 0;
   r->len = p - r->str;
}

/*******************************************************************************
 Char_to_Str: convert byte to thre digit ascii number
*******************************************************************************/
void Char_to_Str(unsigned char *p, unsigned char n)
{
    *p++ = '0' + n / 100;
    n %= 100;
    *p++ = '0' + n / 10;
    n %= 10;
    *p++ = '0' + n;
    *p = 0;
}

/*******************************************************************************
  Calculate square root of unsigned long
********************************************************************************/
unsigned short sqrt32(unsigned long n)
{
    unsigned short c = 0x8000;
    unsigned short g = 0x8000;
    
    for(;;) {
      if(g*g > n)
      g ^= c;
      c >>= 1;
      if(c == 0)
      return g;
      g |= c;
    }
}

/********************************* END OF FILE ********************************/
