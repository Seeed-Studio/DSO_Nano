/*******************************************************************************
File Name: main.c  
********************************************************************************/

#include "Function.h"
#include "Menu.h"
#include "Lcd.h"
#include "Calculate.h"
#include "stm32f10x_lib.h"
#include "HW_V1_Config.h"
#include "Files.h"
#include "string.h"
#include "ASM_Function.h"

void   main(void)
{
   unsigned short i;

/*--------------initialize the hardware-----------*/
   pLib = (LIB_Interface *)*(u32 *)(LIB_VECTORS + 7 * 4);
   if (pLib->Signature != LIB_SIGNATURE)  // incompatible library
     while (1); // halt, we can not display an error as we have no font table

   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0xC000);
   NVIC_Configuration();

/*----------display APP version ----------*/
   Display_Str(168, 23, YEL, PRN, "APP ver 3.13");
   frm_col = FRM_COLOR;
   
   Delayms(2000);
   //WaitForKey();
 
   for (i = 0; i < sizeof(Ref_Buffer); i++)
      Ref_Buffer[i] = pLib->Get_Ref_Wave(i);
   
/*--------initialize the display --------*/

   Display_Frame();
   DrawMenu();
   Read_Parameter();
   RestoreConfig();
   SelectMenu(0);

/*--------application main loop --------*/

   while (1) {
     Update_Item();
     Scan_Wave();

     if (Key_Buffer) {

       if (Key_Buffer == KEYCODE_UP) {
         if (Popup.Active) 
          SelectSub(Popup.Sub - 1);
         else
          SelectMenu(CurrentMenu - 1);
       }
       if (Key_Buffer == KEYCODE_DOWN) {
         if (Popup.Active)
           SelectSub(Popup.Sub + 1);
         else
           SelectMenu(CurrentMenu + 1);
       }
       if (Key_Buffer == KEYCODE_M) {
         if (Popup.Active)
           HidePopup();
         else
           ShowPopup();
       }
       if (Key_Buffer == KEYCODE_PLAY)
       {
          Update[SYNC_MODE] = 1;
          if (Item_Index[RUNNING_STATUS] == RUN) {
             Item_Index[RUNNING_STATUS] = HOLD;
          } else {
             Stop_Wave();
             Item_Index[RUNNING_STATUS] = RUN;
          }
       }

       Update[Item_Index[CI]] = 1;
       Update[CURSORS] = 1;

       if (Key_Buffer == KEYCODE_B) {
         if (Popup.Active) {
           HidePopup();
           Update_Item();
           Update[Item_Index[CI]] = 1;
           Update[CURSORS] = 1;
         }
       switch (Item_Index[CI]) {
         case SYNC_MODE:
            if (Item_Index[SYNC_MODE] != 4) // FIT mode
            {
              Erase_Sensitivity();
              Item_Index[TRIG_SENSITIVITY] = 3;
              if (Item_Index[X_SENSITIVITY] >= 15) {
                Item_Index[X_SENSITIVITY] = 14;
                Update[X_SENSITIVITY] = 1;
              }
              Item_Index[SYNC_MODE] = 4;
            } else {
              Item_Index[SYNC_MODE] = 0;
            }
            Item_Index[RUNNING_STATUS] = RUN;
            Update[RUNNING_STATUS] = 1;
            break;
            
         case TRIG_LEVEL:
            Erase_Sensitivity();
            Hide_Index[VT] = (Hide_Index[VT] + 1) & 1; 
            if (!Hide_Index[VT]) Hide_Index[VS] = 1;  // VT and VS are mutually exclusive
            break;
            
         case TRIG_SENSITIVITY:
            Erase_Sensitivity();
            Hide_Index[VS] = (Hide_Index[VS] + 1) & 1; // show or hide trigger sensitivity lines
            if (!Hide_Index[VS]) Hide_Index[VT] = 1;  // VT and VS are mutually exclusive
            break;
            
         case T2_CURSOR:
            Hide_Index[T2] = (Hide_Index[T2] + 1) & 1;
            break;
            
         case T1_CURSOR:
            Hide_Index[T1] = (Hide_Index[T1] + 1) & 1; 
            break;
            
         case TRIG_POS:
            if (Item_Index[TP] == BUFFER_SIZE) {
              Hide_Index[TP] = (Hide_Index[TP] + 1) & 1;
              Draw_Ti_Line(Tp, ERASE, CH2_COLOR);
            } else {
              Draw_Ti_Line(Tp, ERASE, CH2_COLOR);
              Draw_Ti_Mark(Tp, ERASE, CH2_COLOR);
              Item_Index[TP] = BUFFER_SIZE;
              Redraw_Wave();
            }
            break;
            
         case V2_CURSOR:   // bottom cursor
            Hide_Index[V2] = (Hide_Index[V2] + 1) & 1; // show or hide V2
            break;
            
         case V1_CURSOR:   // top cursor
            Hide_Index[V1] = (Hide_Index[V1] + 1) & 1;  // show or hide V1
            break;

         case SHOW_WAVE_CURVE:
            Hide_Index[REF] = (Hide_Index[REF] + 1) & 1;  // show or hide reference wave
            if (Hide_Index[REF])
              Erase_Reference();
            else
              Draw_Reference();
            break;

         case SAVE_WAVE_IMAGE:
            SaveWaveImage();
            break;

         case SAVE_WAVE_CURVE:
            SaveWave();
            break;

         case LOAD_WAVE_CURVE:
            LoadWave();
            break;
 
         case SAVE_PROFILE:
            SaveConfig();
            break;

         case LOAD_PROFILE:
            LoadConfig();
            break;

         case CALIBRATE_OFFSET:
            Item_Index[CALIBRATE_OFFSET] = 100;
            break;
            
         case CALIBRATE_RANGE:
            Item_Index[CALIBRATE_RANGE] = 100;
            break;

//         case Y_SENSITIVITY:
//         case X_SENSITIVITY:
//         case TRIG_SLOPE:
//         case OUTPUT_FREQUENCY:
//         case GND_POSITION:
//         case MEASURE_KIND:
//         case INPUT_ATTENUATOR:
  
       }/*switch*/
       }/*if*/
       

       if ((Key_Buffer == KEYCODE_LEFT) || (Key_Buffer == KEYCODE_RIGHT)) {
       switch (Item_Index[CI]) {
         case Y_SENSITIVITY:
            Item_Index[TP] = BUFFER_SIZE;
            Update[TRIG_POS] = 1;
            Draw_Ti_Mark(Tp, ERASE, CH2_COLOR);
            if (Key_Buffer == KEYCODE_RIGHT)
            {
               if ((Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] < 9))
                  Item_Index[Y_SENSITIVITY]++; // attenuation X1
               if ((Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] < 18))
                  Item_Index[Y_SENSITIVITY]++; // attentuation X10
            }
            if (Key_Buffer == KEYCODE_LEFT)
            {
               if ((Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] > 0))
                  Item_Index[Y_SENSITIVITY]--; // X1
               if ((Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] > 11))
                  Item_Index[Y_SENSITIVITY]--; // X10
            }
            Update[TRIG_LEVEL] = 1; // trigger voltage
            Update[V1_CURSOR] = 1; // V1-V2
            Stop_Wave();
            break;

         case X_SENSITIVITY:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[X_SENSITIVITY] < 21))
               Item_Index[X_SENSITIVITY]++;
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[X_SENSITIVITY] > 0))
               Item_Index[X_SENSITIVITY]--;
            Item_Index[TP] = BUFFER_SIZE;
            Draw_Ti_Mark(Tp, ERASE, CH2_COLOR);
            Update[DELTA_T] = 1;
            Stop_Wave();
            if ((Item_Index[SYNC_MODE] == 0) && (Item_Index[X_SENSITIVITY] >= 15)) {
               Item_Index[SYNC_MODE] = 3;
               Update[SYNC_MODE] = 1;
            }
            if ((Item_Index[SYNC_MODE] == 3) && (Item_Index[X_SENSITIVITY] < 15)) {
               Item_Index[SYNC_MODE] = 0;
               Update[SYNC_MODE] = 1;
            }
            break;

          case SYNC_MODE:
            if (Item_Index[SYNC_MODE] >= 3) // 3 = SCAN, 4 = FIT
              Item_Index[SYNC_MODE] = 0;
            if (Key_Buffer == KEYCODE_LEFT)
            { // next mode
               if (Item_Index[SYNC_MODE] > 0)
                  Item_Index[SYNC_MODE]--;
               else
                  Item_Index[SYNC_MODE] = 2;
            }
            if (Key_Buffer == KEYCODE_RIGHT)
            { 
               if (Item_Index[SYNC_MODE] < 2)
                  Item_Index[SYNC_MODE]++;
               else
                  Item_Index[SYNC_MODE] = 0;
            }
            Stop_Wave();
            Item_Index[RUNNING_STATUS] = RUN;
            if ((Item_Index[SYNC_MODE] == 0) && (Item_Index[X_SENSITIVITY] >= 15)) {
               Item_Index[SYNC_MODE] = 3;
            }
            break;
            
         case TRIG_LEVEL:
            Erase_Sensitivity();
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[VT] < (MAX_Y - Item_Index[VS] - 1)))
               Item_Index[VT]++; 
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[VT] > (MIN_Y + Item_Index[VS] + 1)))
               Item_Index[VT]--; 
            Hide_Index[VT] = 0;
            Hide_Index[VS] = 1;
            break;
            
          case TRIG_SENSITIVITY:
            Erase_Sensitivity();
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[VT] < (MAX_Y - Item_Index[VS] - 1)) && (Item_Index[VT] > (MIN_Y + Item_Index[VS] + 1)))
               Item_Index[VS]++; // reduce trigger sensitivity
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[VS] > 0))
               Item_Index[VS]--; // increase trigger sensitivity
            Hide_Index[VS] = 0;
            Hide_Index[VT] = 1;
            break;
            
         case TRIG_SLOPE:
            Item_Index[TRIG_SLOPE] = (Item_Index[TRIG_SLOPE] + 1) & 1; // rising or falling edge
            break;
            
         case OUTPUT_FREQUENCY:
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[OUTPUT_FREQUENCY] < 15))
               Item_Index[OUTPUT_FREQUENCY]++; 
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[OUTPUT_FREQUENCY] > 0))
               Item_Index[OUTPUT_FREQUENCY]--; 
            break;
            
         case T2_CURSOR:
            Draw_Ti_Mark(Item_Index[T2], ERASE, LN2_COLOR);
            Draw_Ti_Line(Item_Index[T2], ERASE, LN2_COLOR);
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[T2] < MAX_X - 1))
               Item_Index[T2]++;
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[T2] > Item_Index[T1] + 1))
               Item_Index[T2]--;
            Hide_Index[T2] = 0;
            Update[DELTA_T] = 1;
            break;
            
         case T1_CURSOR:
            Draw_Ti_Mark(Item_Index[T1], ERASE, LN2_COLOR);
            Draw_Ti_Line(Item_Index[T1], ERASE, LN2_COLOR);
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[T1] < Item_Index[T2] - 1))
               Item_Index[T1]++;
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[T1] > MIN_X + 1))
               Item_Index[T1]--;
            Hide_Index[T1] = 0;
            Update[DELTA_T] = 1;
            break;
            
         case TRIG_POS:
            Draw_Ti_Line(Tp, ERASE, CH2_COLOR);
            Draw_Ti_Mark(Tp, ERASE, CH2_COLOR);
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[TP] <= (BUFFER_SIZE+SEGMENT_SIZE-25)))
                 Item_Index[TP] += 25; 
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[TP] >= (BUFFER_SIZE-SEGMENT_SIZE+25)))
                 Item_Index[TP] -= 25; 
            Redraw_Wave();
            Hide_Index[TP] = 0;
            break;
            
         case V2_CURSOR:   // bottom cursor
            Draw_Vi_Mark(Item_Index[V2], ERASE, LN2_COLOR);
            Draw_Vi_Line(Item_Index[V2], ERASE, LN2_COLOR);
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[V2] < Item_Index[V1] - 1))
               Item_Index[V2]++; // Vertical cursor rise
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[V2] > MIN_Y + 1))
               Item_Index[V2]--; 
            Hide_Index[V2] = 0;
            break;
            
         case V1_CURSOR:   // top cursor
            Draw_Vi_Mark(Item_Index[V1], ERASE, LN2_COLOR);
            Draw_Vi_Line(Item_Index[V1], ERASE, LN2_COLOR);
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[V1] < MAX_Y - 1))
               Item_Index[V1]++; 
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[V1] > Item_Index[V2] + 1))
               Item_Index[V1]--; 
            Hide_Index[V1] = 0;
            break;

         case GND_POSITION:
            Draw_Vi_Mark(Item_Index[V0], ERASE, WAV_COLOR);
            Erase_Sensitivity();
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[V0] < MAX_Y - 1) && (Item_Index[VT] < (MAX_Y - Item_Index[TRIG_SENSITIVITY] - 1)))
            {
               Item_Index[V0]++;
               Item_Index[VT]++; 
            } 
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[V0] > (MIN_Y + 1)) && (Item_Index[VT] > (MIN_Y + Item_Index[TRIG_SENSITIVITY] - 1)))
            {
               Item_Index[V0]--;
               Item_Index[VT]--; 
            } 
            break;
            
         case MEASURE_KIND:
            if (Key_Buffer == KEYCODE_RIGHT)
            {
               if (Item_Index[MEASURE_KIND] < 7)
                  Item_Index[MEASURE_KIND]++; // next measure kind
               else
                  Item_Index[MEASURE_KIND] = 0;
            }
            if (Key_Buffer == KEYCODE_LEFT)
            {
               if (Item_Index[MEASURE_KIND] > 0)
                  Item_Index[MEASURE_KIND]--; // previous measure kind
               else
                  Item_Index[MEASURE_KIND] = 7;
            }
            break; 
            
         case INPUT_ATTENUATOR:
            Item_Index[INPUT_ATTENUATOR] = (Item_Index[INPUT_ATTENUATOR] + 1) & 1; // toggle bteween 1x and 10x
            if ((Item_Index[INPUT_ATTENUATOR] == 0) && (Item_Index[Y_SENSITIVITY] > 9))
                Item_Index[Y_SENSITIVITY] -= 9;
            if ((Item_Index[INPUT_ATTENUATOR] == 1) && (Item_Index[Y_SENSITIVITY] < 10))
                Item_Index[Y_SENSITIVITY] += 9;
            Erase_Wave(0, X_SIZE);
            Update[Y_SENSITIVITY] = 1;   // reconfigures ADC
            Update[TRIG_LEVEL] = 1;      // trigger voltage
            Update[V1_CURSOR] = 1;      // V1-V2
            break;

         case SHOW_WAVE_CURVE:
            Hide_Index[REF] = (Hide_Index[REF] + 1) & 1;  // show or hide reference wave
            if (Hide_Index[REF])
              Erase_Reference();
            else
              Draw_Reference();
            break;

         case SAVE_WAVE_IMAGE:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[SAVE_WAVE_IMAGE] < 255))
                Item_Index[SAVE_WAVE_IMAGE]++; 
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[SAVE_WAVE_IMAGE] > 1))
                Item_Index[SAVE_WAVE_IMAGE]--; 
            break;

         case SAVE_WAVE_CURVE:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[SAVE_WAVE_CURVE] < 255))
                Item_Index[SAVE_WAVE_CURVE]++; 
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[SAVE_WAVE_CURVE] > 1))
                Item_Index[SAVE_WAVE_CURVE]--; 
            break;

         case LOAD_WAVE_CURVE:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[LOAD_WAVE_CURVE] < 255))
                 Item_Index[LOAD_WAVE_CURVE]++; // file number + 1
            if ((Key_Buffer == KEYCODE_LEFT) && ((Item_Index[LOAD_WAVE_CURVE]) > 1))
                 Item_Index[LOAD_WAVE_CURVE]--; // file number - 1
            break;

         case SAVE_PROFILE:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[SAVE_PROFILE] < 255))
                Item_Index[SAVE_PROFILE]++; 
            if ((Key_Buffer == KEYCODE_LEFT) && (Item_Index[SAVE_PROFILE] >= 1))
                Item_Index[SAVE_PROFILE]--; 
            break;

         case LOAD_PROFILE:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[LOAD_PROFILE] < 255))
                 Item_Index[LOAD_PROFILE]++; // file number + 1
            if ((Key_Buffer == KEYCODE_LEFT) && ((Item_Index[LOAD_PROFILE]) >= 1))
                 Item_Index[LOAD_PROFILE]--; // file number - 1
            break;

        case CALIBRATE_OFFSET:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[CALIBRATE_OFFSET] < 200))
                 Item_Index[CALIBRATE_OFFSET]++; // 1 pix increase
            if ((Key_Buffer == KEYCODE_LEFT) && ((Item_Index[CALIBRATE_OFFSET]) >= 1))
                 Item_Index[CALIBRATE_OFFSET]--; // 1 pix decrease
            break;

        case CALIBRATE_RANGE:
            if ((Key_Buffer == KEYCODE_RIGHT) && (Item_Index[CALIBRATE_RANGE] < 200))
                 Item_Index[CALIBRATE_RANGE]++; // 1% increase
            if ((Key_Buffer == KEYCODE_LEFT) && ((Item_Index[CALIBRATE_RANGE]) >= 1))
                 Item_Index[CALIBRATE_RANGE]--; // 1% decrease
            break;
            
       }/*switch*/
       }/*if*/
       Key_Buffer = 0;
     }
   }
   
}/*main*/
/********************************* END OF FILE ********************************/
