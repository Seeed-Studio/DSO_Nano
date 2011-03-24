/*******************************************************************************
 File name  : Files.h
 *******************************************************************************/
#ifndef __FILES_H
#define __FILES_H
//==================== Function declarations ===================================
char            FAT_Info(void);
char            Open_File(unsigned const char *name, unsigned char *num, unsigned const char *ext);

unsigned char   SetClusterNext(void);
char            DirTouch(void);
unsigned char   DirMatch(unsigned const char *p1, unsigned const char *p2, unsigned short n);

char            Read_File(void);
char            Write_File(void);
char            Writ_BMP_File(void);
void            Read_Parameter(void);
char            Write_Parameter(void);

#endif
/********************************* END OF FILE ********************************/
