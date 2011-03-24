/*******************************************************************************
File Name: Function.c   
*******************************************************************************/

#include "Function.h"
#include "Menu.h"
#include "Lcd.h"
#include "Calculate.h"
#include "stm32f10x_lib.h"
#include "HW_V1_Config.h"
#include "ASM_Function.h"
#include "string.h"
#include "Files.h"

//-----------------------------------------------------------------------------

volatile unsigned short  Scan_Buffer[BUFFER_SIZE]; // sampling buffer
unsigned char   Signal_Buffer[300]; // signal data buffer
unsigned char   View_Buffer[300]; // view buffer
unsigned char   Erase_Buffer[300]; // erase buffer
unsigned char   Ref_Buffer[304]; // reference waveform buffer

// -----------------------------------------------------------------------------

volatile unsigned char   ScanSegment, ScanMode;

unsigned short  X1_Counter, X2_Counter,
                Wait_CNT, t0, t0_scan, tp_to_abs, tp_to_rel;
unsigned char   Toggle, Sync, SyncSegment;

unsigned char MeFr, MeDC;   // flag variable to indicate if frequency/DC related parameters are up to date
int      Frequency, Duty, Vpp, Vrms, Vavg, Vdc, Vmin, Vmax;

unsigned const short Ks[22] =   // interpolation coefficient of the horizontal scanning interval
 {29860, 14930, 5972, 2986, 1493, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024};


/*******************************************************************************
 Function Name : Mark_Trig
 Description : mark the trigger point and setup for post scan
 Para :     trigger point (absolute position within buffer)
*******************************************************************************/
void     Mark_Trig(unsigned short tp, unsigned char stop_scan)
{
    SyncSegment = tp / SEGMENT_SIZE;
    Sync = 2; // indicate trigger marked
    if (stop_scan) ScanMode = 3; // start DMA post fetch

    if (SyncSegment == 0) {
      tp_to_abs = SEGMENT_SIZE * 2;
      tp_to_rel = SEGMENT_SIZE;
    } else if (SyncSegment == 1) {
      tp_to_abs = 0;
      tp_to_rel = 0;
    } else {
      tp_to_abs = SEGMENT_SIZE;
      tp_to_rel = SEGMENT_SIZE * 2;
    }
    
    t0 = (tp + tp_to_rel);
    if (t0 >= BUFFER_SIZE) t0 -= BUFFER_SIZE;
    
    // reset display x pointers
    X1_Counter = X2_Counter = 0;
}

/*******************************************************************************
 Function Name : GetScanPos
 Description : find and reurn index of current point in scan buffer
*******************************************************************************/
unsigned short GetScanPos(void)
{
   unsigned short t;
   unsigned char  ss;

   do {
     ss = ScanSegment;
     t = (ss + 1) * SEGMENT_SIZE;
     t = t - DMA_CNDTR1;
   } while (ss != ScanSegment);
   if (t >= BUFFER_SIZE) t -= BUFFER_SIZE;
   return t;
}

/*******************************************************************************
 Function Name : Find_Trig
 Description :find the first point in sampling buffer which meets trigger condition 
*******************************************************************************/
void     Find_Trig(void)
{
   int            th1, th2;
   bool           trig = FALSE;
   unsigned short t = GetScanPos(); // get current absolute position in scan buffer

   // calculate trigger thresholds
   th1 = SigToAdc(Item_Index[VT] - Item_Index[TRIG_SENSITIVITY]);
   th2 = SigToAdc(Item_Index[VT] + Item_Index[TRIG_SENSITIVITY]);

   // search for trigger
   while (t0 != t) {
      if (Item_Index[TRIG_SLOPE] == 0)
      { 
         if ((Sync == 0) && (Scan_Buffer[t0] > th1))
           Sync = 1;  // below trigger threshold

         if ((Sync == 1) && (Scan_Buffer[t0] < th2))
           trig = TRUE; // above trigger threshold
      } else {  // trigger slope is descending edge
         if ((Sync == 0) && (Scan_Buffer[t0] <= th2))
           Sync = 1;  // above trigger threshold

         if ((Sync == 1) && (Scan_Buffer[t0] >= th1))
           trig = TRUE; // below trigger threshold
      }
 
      if (trig) {
        Mark_Trig(t0, 1);
        break;
      }
      
      t0 = (t0 + 1);
      if (t0 >= BUFFER_SIZE) t0 = 0;
   }
}

/*******************************************************************************
 Function Name : AdcToSig
 Description : scale ADC reading to screen
*******************************************************************************/
int AdcToSig(int adc)
{
  int sig;

  sig = Km[Item_Index[Y_SENSITIVITY]] * (2048 - adc) / 4096 + 120 + (Item_Index[CALIBRATE_OFFSET] - 100);
  sig += (sig - Item_Index[V0]) * (Item_Index[CALIBRATE_RANGE] - 100) / 200;
  return sig;
}

/*******************************************************************************
 Function Name : SigToAdc
 Description : scale signal to ADC value
*******************************************************************************/
int SigToAdc(int sig)
{
  int adc, t = Item_Index[CALIBRATE_RANGE] - 100;
  
  sig = (sig * 200 + Item_Index[V0] * t) / (200 + t);
  adc = 2048 - (sig - 120 - (Item_Index[CALIBRATE_OFFSET] - 100)) * 4096 / Km[Item_Index[Y_SENSITIVITY]];
  return adc;
}

/*******************************************************************************
 Function Name : Process_Wave
 Description : process sampling buffer and put results in signal buffer
*******************************************************************************/
void    Process_Wave(void)
{
   int             p, q;
   int             Vs;
   unsigned short  t;  // relative position of last capture

   if (ScanMode == 0) { // capture complete
     t = BUFFER_SIZE;
     Sync = 4;  // no more data to process
   } else {
     t = GetScanPos() + tp_to_rel;  // get current relative position in scan buffer
     if (t >= BUFFER_SIZE) t -= BUFFER_SIZE;
   }

   p = t0;
   if ((ScanMode != 1) && (ScanMode != 2))  // ignore trigger/x offset when in continues scan mode
     p += BUFFER_SIZE - Item_Index[TP] - 150;

   for (; X2_Counter < X_SIZE; X2_Counter++)
   {
      q = p + (X2_Counter * 1024) / Ks[Item_Index[X_SENSITIVITY]];
      if (q < 0)
      {
        Erase_Wave(X1_Counter, X2_Counter + 1);
        X1_Counter = X2_Counter;
        continue;
      }

      if (q >= t)
         break;

      // find absolute q position in buffer      
      q = (q + tp_to_abs);
      if (q >= BUFFER_SIZE) q -= BUFFER_SIZE;

      Vs = AdcToSig(Scan_Buffer[q]);  // scale to screen
      if (Vs > MAX_Y) Vs = MAX_Y;
      else if (Vs < MIN_Y) Vs = MIN_Y;
      Signal_Buffer[X2_Counter] = Vs;
      
      Sync = 3; // new values in signal buffer
   }

   if ((Sync >= 3) && (X2_Counter < X_SIZE)) {
      if ((ScanMode == 1) || (ScanMode == 2))
        Erase_Wave(X2_Counter, X2_Counter + 25); // leave 1 div gap in scan mode
      else if ((Sync >= 4) || (Item_Index[X_SENSITIVITY] >= 12))
        Erase_Wave(X2_Counter, X_SIZE);
   }
}

/*******************************************************************************
 Function Name : Erase_Reference
 Description : Erase reference waveform
*******************************************************************************/
void        Erase_Reference(void)
{
   unsigned short j, i;
   
   for (j = i = 0; j < X_SIZE; j++) {
      Erase_SEG(j, Ref_Buffer[i], Ref_Buffer[j], REF_COLOR); // erase reference wave
      i = j;
   }
}

/*******************************************************************************
 Function Name : Draw_Reference
 Description : Draw reference waveform
*******************************************************************************/
void        Draw_Reference(void)
{
   unsigned short j, i;
   
   for (j = i = 0; j < X_SIZE; j++) {
      Draw_SEG(j, Ref_Buffer[i], Ref_Buffer[j], REF_COLOR); // erase reference wave
      i = j;
   }
}

/*******************************************************************************
 Function Name : Erase_Vawe
 Description : Erase waveform from t1 to t2 - 1 inclusive
*******************************************************************************/
void        Erase_Wave(unsigned short t1, unsigned short t2)
{
   unsigned short j;
   
   if (t2 > X_SIZE) t2 = X_SIZE;
   for (j = t1; j < t2; j++) {
      Erase_SEG(j, Erase_Buffer[j], View_Buffer[j], WAV_COLOR); // erase previous signal
      View_Buffer[j] = 0xff;
      Erase_Buffer[j] = 0xff;
   }
   MeFr = MeDC = 0;
}

/*******************************************************************************
 Function Name : Redraw_Vawe
 Description : Process signal buffer and redraw waveform
*******************************************************************************/
void        Redraw_Wave(void)
{
  if ((ScanMode == 0) || (Sync >= 2)) {
    X1_Counter = X2_Counter = 0;
    Sync = 2; // process wave
  } else Erase_Wave(0, X_SIZE);
}

/*******************************************************************************
 Function Name : Draw_Wave
 Description :erase the reference and view wave then draw new waveform
*******************************************************************************/
void        Draw_Wave(void)
{
   unsigned short  i = (X1_Counter > 0) ? X1_Counter - 1 : 0;

   for (; X1_Counter < X2_Counter; X1_Counter++)
   {
      Erase_SEG(X1_Counter, Erase_Buffer[X1_Counter], View_Buffer[X1_Counter], WAV_COLOR); // erase previous signal
      Draw_SEG(X1_Counter, Signal_Buffer[i], Signal_Buffer[X1_Counter], WAV_COLOR); // draw new signal
      View_Buffer[X1_Counter] = Signal_Buffer[X1_Counter];
      Erase_Buffer[X1_Counter] = Signal_Buffer[i];
      i = X1_Counter;
   }

   if (Sync == 4) // scan complete
   { 
      Measure_Wave();   // do waveform measurements
      Sync = 0;
   } else (Sync = 2); // further processing needed
}


/*******************************************************************************
 Function Name : Stop_Wave
 Description : stop DMA ADC sampling forcefully and clear screen
               use for mode change
*******************************************************************************/
void    Stop_Wave(void)
{
  ADC_Stop();
  Erase_Wave(0, X_SIZE);
  Sync = 0;
}

/*******************************************************************************
 Function Name : Scan_Wave
 Description : process the synchronous scan,display waveform based on the synchronous mode
 NOTE: there are four modes, AUTO, NORM, SING, FIT 
*******************************************************************************/
void    Scan_Wave(void) 
{
    //--------------------Common-------------------------
    if (Item_Index[RUNNING_STATUS] == RUN)
    {
       if ((Sync <= 1) && (ScanMode == 0)) { // we must restart sampling
          Sync = 0;
          t0 = SEGMENT_SIZE; // start to look for trigger in first position, second segment
          t0_scan = 0;
          ADC_Start();
          Refresh_Counter = 100;  // keep waveform for 100ms
       }
    }
    
   //--------------------SCAN-----------------------------
    if (Item_Index[SYNC_MODE] == 3) { // 3:SCAN
      if ((Sync <= 1) && (ScanMode >= 1)) {
         Mark_Trig(t0_scan, 0);
      }
      if (Sync == 2)
         Process_Wave(); 
  
      if (Sync >= 3) {
         Draw_Wave(); 
         if ((ScanMode == 1) || (ScanMode == 2)) {
           if (X2_Counter >= X_SIZE) {
             t0_scan += X_SIZE;   // advance one frame
             if (t0_scan >= BUFFER_SIZE) t0_scan -= BUFFER_SIZE;
             Sync = 0;
           }
         }
      }

      if ((Item_Index[RUNNING_STATUS] != RUN) && ((ScanMode == 1) || (ScanMode == 2))) {
        t0_scan += 150;
        if (t0_scan >= BUFFER_SIZE) t0_scan -= BUFFER_SIZE;
        Mark_Trig(t0_scan, 1); // force trig at screen center position
        Draw_Ti_Line(Tp, ERASE, CH2_COLOR);
        Draw_Ti_Mark(Tp, ERASE, CH2_COLOR);
        Erase_Trig_Pos();
        Item_Index[TP] = BUFFER_SIZE; // reset X position to center of screen
        Update[CURSORS] = 1;
     }
   }
   
   //--------------------AUTO, FIT------------------------
   if ((Item_Index[SYNC_MODE] == 0) || (Item_Index[SYNC_MODE] == 4))// 0:AUTO, 5:FIT
   {
      if ((Sync <= 1) && (ScanMode >= 2)) {
       if (Refresh_Counter == 0) {  // force trigger after 100ms
         Mark_Trig(GetScanPos(), 1); // trig at current scan position
       } else
         Find_Trig(); 
      }
      if (Sync == 2)
         Process_Wave(); 
  
      if ((Sync >= 3) && (Refresh_Counter == 0))
         Draw_Wave(); 
   }

   //--------------------NORM-----------------------------
   if (Item_Index[SYNC_MODE] == 1)//1 :NORM
   {
      if ((Sync <= 1) && (ScanMode >= 2))
        Find_Trig(); 

      if (Sync == 2)
       Process_Wave(); 

      if ((Sync >= 3) && (Refresh_Counter == 0))
         Draw_Wave(); 
   }

   //-------------------- SING--------------------------
   if (Item_Index[SYNC_MODE] == 2)//2 :SING
   {
      if ((Sync <= 1) && (ScanMode >= 2))
         Find_Trig(); 

      if (Sync == 2)
         Process_Wave();

      if ((Sync >= 3) && (Refresh_Counter == 0)) {
         Draw_Wave(); 
         Item_Index[RUNNING_STATUS] = HOLD;
         Update[SYNC_MODE] = 1;
      }
   }
}

/*******************************************************************************
 Function Name : Measure_Wave
 Description :  calculate the frequency,cycle,duty, Vpp(peak-to-peak value),Vavg(average of alternating voltage),
                Vrms (effective value of alternating voltage), DC voltage
*******************************************************************************/
void      Measure_Wave(void)
{
   unsigned short  i, j, t_max = 0xffff, t_min = 0, Trig = 0;
   unsigned int    Threshold0, Threshold1, Threshold2, Threshold3;
   int             Vk = 0, Vn, Vm, Vp, Vq, Tmp1, Tmp2;
   unsigned short  Edge, First_Edge, Last_Edge;
   
   Edge = 0,
   First_Edge = 0;
   Last_Edge = 0;
   Threshold0 = SigToAdc(Item_Index[V0]);
   Threshold1 = SigToAdc(Item_Index[VT] - Item_Index[TRIG_SENSITIVITY]);
   Threshold2 = SigToAdc(Item_Index[VT] + Item_Index[TRIG_SENSITIVITY]);
   Threshold3 = SigToAdc(Item_Index[VT]);
   
   for (i = 0; i < BUFFER_SIZE; i++)
   {
      j = (i + tp_to_abs);
      if (j >= BUFFER_SIZE) j -= BUFFER_SIZE;

      Vk += Scan_Buffer[j];
      if ((i >= t0) && (i < t0 + 300))
      {
         if (Scan_Buffer[j] < t_max)
            t_max = Scan_Buffer[j];
         if (Scan_Buffer[j] > t_min)
            t_min = Scan_Buffer[j];
      }
      if ((Trig == 0) && (Scan_Buffer[j] > Threshold1))
         Trig = 1; 

      if ((Trig == 1) && (Scan_Buffer[j] < Threshold2))
      {
         Trig = 0;
         if (First_Edge == 0)
         {
            First_Edge = i;
            Last_Edge = i;
            Edge = 0;
         } else {
            Last_Edge = i;
            Edge++;
         }
      }
   }
   Vk = Vk / BUFFER_SIZE;

   MeFr = 0;
   if (Edge != 0)
   {
      Vm = 0;
      Vq = 0;
      Vn = 0;
      for (i = First_Edge; i < Last_Edge; i++)
      { 
         j = (i + tp_to_abs);
         if (j >= BUFFER_SIZE) j -= BUFFER_SIZE;
      
         if (Scan_Buffer[j] < Threshold3) Vm++;
         
         Vp = (4096 - Scan_Buffer[j]) - Threshold0;
         Vn += (Vp * Vp) / 8; 

         if (Scan_Buffer[j] < Threshold0)
            Vq += (Threshold0 - Scan_Buffer[j]);
         else
            Vq += (Scan_Buffer[j] - Threshold0);
      }
      if (Item_Index[X_SENSITIVITY] < 5)
        Frequency = (Edge * (1000000000 / 1167) / (Last_Edge - First_Edge)) * 1000; // ??? suspicious, check
      else
        Frequency = (Edge * (1000000000 / T_Scale[Item_Index[X_SENSITIVITY]]) / (Last_Edge - First_Edge)) * 1000;
      
      //Cycle = ((Last_Edge - First_Edge) * T_Scale[Item_Index[X_SENSITIVITY]]) / Edge;
      //Tlow = ((Last_Edge - First_Edge - Vm) * T_Scale[Item_Index[X_SENSITIVITY]]) / Edge;
      //Thigh = (Vm * T_Scale[Item_Index[X_SENSITIVITY]]) / Edge;
      Duty = 100000 * Vm / (Last_Edge - First_Edge);
      
      Vrms = ((Km[Item_Index[Y_SENSITIVITY]] * sqrt32(Vn / (Last_Edge - First_Edge) * 8)) / 4096)
              * V_Scale[Item_Index[Y_SENSITIVITY]];
      Vrms = Vrms + Vrms * (Item_Index[CALIBRATE_RANGE] - 100) / 200;
      Vavg = ((Km[Item_Index[Y_SENSITIVITY]] * (Vq / (Last_Edge - First_Edge))) / 4096)
              * V_Scale[Item_Index[Y_SENSITIVITY]];
      Vavg = Vavg + Vavg * (Item_Index[CALIBRATE_RANGE] - 100) / 200;
      
      MeFr = 1;
   }
    
   if (t_min < t_max) t_min = t_max;

   Tmp1 = AdcToSig(t_min);
   Vmin = (Tmp1 - Item_Index[V0]) * V_Scale[Item_Index[Y_SENSITIVITY]];
   Tmp2 = AdcToSig(t_max);
   Vmax = (Tmp2 - Item_Index[V0]) * V_Scale[Item_Index[Y_SENSITIVITY]];
   Tmp1 = Tmp2 - Tmp1;
   Vpp = Tmp1 * V_Scale[Item_Index[Y_SENSITIVITY]];
   Tmp2 = AdcToSig(Vk);
   Vdc = (Tmp2 - Item_Index[V0]) * V_Scale[Item_Index[Y_SENSITIVITY]];

   MeDC = 1;
   Update[MEASURE_KIND] = 1;
 
   if ((Item_Index[SYNC_MODE] == 4) && (Wait_CNT == 0))
   {
      if ((Edge < 20) && (Item_Index[X_SENSITIVITY] < 14))
      {
         Item_Index[X_SENSITIVITY]++;
         Update[X_SENSITIVITY] = 1;
      }
      if ((Edge > 60) && (Item_Index[X_SENSITIVITY] > 0))
      {
         Item_Index[X_SENSITIVITY]--;
         Update[X_SENSITIVITY] = 1;

      }
      if ((Tmp1 < 50) && (Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] > 0))
      {
         Item_Index[Y_SENSITIVITY]--;
         Update[Y_SENSITIVITY] = 1;
      }
      if ((Tmp1 < 50) && (Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] > 11))
      {
         Item_Index[Y_SENSITIVITY]--;
         Update[Y_SENSITIVITY] = 1;
      }
      if ((Tmp1 > 150) && (Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] < 9))
      {
         Item_Index[Y_SENSITIVITY]++;
         Update[Y_SENSITIVITY] = 1;
      }
      if ((Tmp1 > 150) && (Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] < 18))
      {
         Item_Index[Y_SENSITIVITY]++;
         Update[Y_SENSITIVITY] = 1;
      }
      Erase_Sensitivity();
      Item_Index[VT] = Tmp2 - Item_Index[VS] * 2;
      Update[TRIG_LEVEL] = 1;
      Update[CURSORS] = 1;
   }
   if (Wait_CNT > 5) Wait_CNT = 0;
   else Wait_CNT++;
}

/******************************** END OF FILE *********************************/
