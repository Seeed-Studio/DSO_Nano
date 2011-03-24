/*******************************************************************************
 File name  : Function.h
 *******************************************************************************/
#ifndef __Function_h
#define __Function_h

#define SEGMENT_SIZE 1024
#define BUFFER_SIZE (SEGMENT_SIZE * 3)

#define RUN                0
#define HOLD               1
#define RISING             0

extern volatile unsigned short Scan_Buffer[BUFFER_SIZE];
extern unsigned char View_Buffer[300], Erase_Buffer[300], Ref_Buffer[304];
extern unsigned char Signal_Buffer[300];

extern volatile unsigned char ScanSegment, ScanMode;
extern unsigned char Sync;
extern unsigned char SyncSegment; 

extern unsigned char MeFr, MeDC;
extern int           Frequency, Duty, Vpp, Vrms, Vavg, Vdc, Vmin, Vmax;

int AdcToSig(int adc);
int SigToAdc(int sig);
unsigned short  GetScanPos(void);
void            Mark_Trig(unsigned short tp, unsigned char stop_scan);
void            Find_Trig(void);
void            Process_Wave(void);
void            Stop_Wave(void);
void            Scan_Wave(void);
void            Draw_Reference(void);
void            Erase_Reference(void);
void            Erase_Wave(unsigned short t1, unsigned short t2);
void            Redraw_Wave(void);
void            Draw_Wave(void);
void            Measure_Wave(void);

#endif
/******************************** END OF FILE *********************************/
