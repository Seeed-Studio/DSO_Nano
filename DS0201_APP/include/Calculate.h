/*******************************************************************************
 File name  : Calculate.h
 *******************************************************************************/
#ifndef __Calculate_H__
#define __Calculate_H__

typedef struct _T_i32str_res I32STR_RES;

struct _T_i32str_res
{
   unsigned char   decPos;
   unsigned char   len;
   char            str[13];
};

void            Int32String_sign(I32STR_RES * r, int n, unsigned char e);
void            Int32String(I32STR_RES * r, unsigned int n, unsigned char e);
void            Char_to_Str(unsigned char *p, unsigned char n);
void            PercentString(I32STR_RES * r, short n);
unsigned short  sqrt32(unsigned long n);

#endif
/********************************* END OF FILE ********************************/
