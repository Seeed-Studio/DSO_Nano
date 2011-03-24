/*******************************************************************************
File Name: Menu.c   
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

// menu definitions
enum {
  MeFreq,
  MeDuty,
  MeRms,
  MeAvg,
  MeVpp,
  MeDCV,
  MeVmin,
  MeVmax,
  VDiv,
  TDiv,
  TrigPosition,
  T1Cursor,
  T2Cursor,
  V1Cursor,
  V2Cursor,
  GndPosition,
  TrigMode,
  TrigLevel,
  TrigSensitivity,
  TrigKind,
  SaveImage,
  SaveReference,
  LoadReference,
  ShowReference,
  SaveProfile,
  LoadProfile,
  OutFreq,
  ProbeAtt
} SubNames;

const SubMenuType Sub[] = {
  {"Freq", 1, MEASURE_KIND},
  {"Duty", 0, MEASURE_KIND},
  {"Vrms", 0, MEASURE_KIND},
  {"Vavg", 0, MEASURE_KIND},
  {"Vpp", 0, MEASURE_KIND},
  {"DC V", 0, MEASURE_KIND},
  {"Vmin", 0, MEASURE_KIND},
  {"Vmax", 0, MEASURE_KIND},
  {"V/Div", 1, Y_SENSITIVITY},
  {"T/Div", 1, X_SENSITIVITY},
  {"Trig Pos", 1, TRIG_POS},
  {"T1 Cursor", 0, T1_CURSOR},
  {"T2 Cursor", 0, T2_CURSOR},
  {"V1 Cursor", 1, V1_CURSOR},
  {"V2 Cursor", 0, V2_CURSOR},
  {"Gnd Pos", 0, GND_POSITION},
  {"Tr. Mode", 1, SYNC_MODE},
  {"Tr. Level", 0, TRIG_LEVEL},
  {"Tr. Sens.", 0, TRIG_SENSITIVITY},
  {"Tr. Kind", 0, TRIG_SLOPE},
  {"Save Img", 1, SAVE_WAVE_IMAGE},
  {"Save Ref", 0, SAVE_WAVE_CURVE},
  {"Load Ref", 0, LOAD_WAVE_CURVE},
  {"Show Ref", 0, SHOW_WAVE_CURVE},
  {"Save Pro", 0, SAVE_PROFILE},
  {"Load Pro", 0, LOAD_PROFILE},
  {"Out Freq", 1, OUTPUT_FREQUENCY},
  {"Probe Att", 1, INPUT_ATTENUATOR},
  {"Cal Offs", 0, CALIBRATE_OFFSET},
  {"Cal Range", 0, CALIBRATE_RANGE}
};

MainMenuType Menu[] = {
  {"VD", "V/Div", VDiv},
  {"TD", "T/Div", TDiv},
  {"YA", "Y-axis", V1Cursor},
  {"XA", "X-axis", TrigPosition},
  {"TR", "Trigger", TrigMode},
  {"ME", "Measure", MeFreq},
  {"FI", "File", SaveImage},
  {"FR", "Freq", OutFreq},
  {"OT", "Other", ProbeAtt}
};

enum {
  SyncModeF,
  TrigKindF,
  VoltageDivF,
  TimeDivF,
  MeasureF,
  BatteryInfoF,
  TrigLevelF,
  DeltaVoltageF,
  DeltaTimeF,
  InfoF
} FieldNames;

const FieldType Field[] = {
  {2, MAX_Y + 4, 40-2}, // SyncMode
  {40, MAX_Y + 4, 16}, // TriggerKind
  {56, MAX_Y + 4, 72},  // VoltageDiv
  {128, MAX_Y + 4, 80}, // TimeDiv
  {216, MAX_Y + 4, 79}, // Measure
  {295, MAX_Y + 4, 25}, // BatteryInfo
  {2, 3, 80-2}, // TrigLevel
  {80, 3, 80}, // DeltaVoltage
  {160, 3, 80}, // DeltaTime
  {240, 3, 80}  // Info
};

PopupType Popup;

unsigned char CurrentMenu;

//------------ voltage range related parameter definitions------------

unsigned const char Item_V[20][10] = // vertical sensitivity labels
// 0           1           2           3           4           5          6            7           8           9   probe attenuation x1
 {"10mV/Div", "20mV/Div", "50mV/Div", "0.1V/Div", "0.2V/Div", "0.5V/Div", " 1V/Div ", " 2V/Div ", " 5V/Div ", " 10V/Div",
//    10        11           12         13         14           15          16          17         18          19  probe attenuation x10    
  "0.2V/Div", "0.5V/Div", " 1V/Div ", " 2V/Div ", " 5V/Div ", " 10V/Div", " 20V/Div", " 50V/Div", "100V/Div", " -GND-  "};

unsigned const int V_Scale[20] = // vertical sensitivity factors
// 0    1     2     3     4     5      6      7      8       9   probe attenuation x1
 {400, 800, 2000, 4000, 8000, 20000, 40000, 80000, 200000, 400000,
// 10     11     12     13     14      15      16      17       18    19  probe attenuation x10
  8000, 20000, 40000, 80000, 200000, 400000, 800000, 2000000, 4000000, 0};

unsigned short  Km[20] =        // voltage compensating factor K = Km/4096 
// 0,    1,   2,   3,    4,   5,   6,    7,   8,   9,
 {2956, 1478, 591,  296, 1114, 446,  223, 1157, 463, 231,
//10,    11,   12,  13,   14,  15,   16,   17,  18,  19
  1452,  581, 290, 1082,  433, 216, 1048,  419, 210, 231};

//Y_POS calibration array 10mV 20mV 50mV  .1V  .2V  .5V   1V   2V   5V  10V  .2V  .5V   1V   2V   5V  10V  20V  50V 100V default
unsigned short  Y_POSm[20] = {20, 42, 104, 207, 55, 138, 275, 53, 132, 265, 42, 105, 212, 57, 142, 284, 59, 146, 294, 0};

//Y_POS calibration array 10mV 20mV 50mV  .1V  .2V  .5V   1V   2V   5V  10V  .2V  .5V   1V   2V   5V  10V  20V  50V 100V default
short           Y_POSn[20] = {881, 799, 566, 180, 750, 439, -75, 758, 461, -37, 799, 563, 161, 743, 424, -109, 735, 409, -146, 956};

//------------time base range related parameter definitions------------      

unsigned const char Item_T[22][10] = // time sensitivity labels
// 0           1           2            3           4           5           6         7           8          9           10
 {" 1us/Div ", " 2us/Div ", " 5us/Div ", " 10us/Div", " 20us/Div", " 50us/Div", "100us/Div", "200us/Div", "500us/Div", " 1ms/Div ", " 2ms/Div ",
//    11          12          13           14          15          16          17         18          19          20          21
" 5ms/Div ", " 10ms/Div", " 20ms/Div", " 50ms/Div", "100ms/Div", "200ms/Div", "500ms/Div", "  1s/Div ", "  2s/Div ", "  5s/Div ", " 10s/Div "};

unsigned const int T_Scale[22] = // time sensitivity factors
 {40, 80, 200, 400, 800, 2000, 4000, 8000, 20000, 40000, 80000,
200000, 400000, 800000, 2000000, 4000000, 8000000, 20000000, 40000000, 80000000, 200000000, 400000000};

unsigned const short Scan_PSC[22] = // prescale of horizontal scanning interval counter - 1
 {11, 11, 11, 11, 11, 15, 15, 15, 15, 15, 15, 31, 63, 63, 127, 255, 255, 255, 511, 511, 511, 1023 };

unsigned const short Scan_ARR[22] = // frequency division of horizontal scanning interval counter - 1 
 {6, 6, 6, 6, 6, 8, 17, 35, 89, 179, 359, 449, 449, 899, 1124, 1124, 2249, 5624, 5624, 11249, 28124, 28124 };

//------------ output base frequency related parameters definition------------      

unsigned const char Item_F[16][7] = // output frequency display labels

 {"1MHz", "500kHz", "200kHz", "100kHz", "50kHz", "20kHz", "10kHz", "5kHz", "2kHz", "1kHz", "500Hz", "200Hz", "100Hz", "50Hz", "20Hz", "10Hz"};

unsigned const int Fout_PSC[16] = // output frequency prescaler

{0, 0, 0, 0, 0, 0, 0, 0, 4 - 1, 4 - 1, 16 - 1, 16 - 1, 64 - 1, 64 - 1, 128 - 1, 128 - 1};

unsigned const int Fout_ARR[16] = // frequency division factors

{72 - 1, 144 - 1, 360 - 1, 720 - 1, 1440 - 1, 3600 - 1, 7200 - 1, 14400 - 1, 9000 - 1, 18000 - 1, 9000 - 1, 22500 - 1, 11250 - 1, 22500 - 1, 28125 - 1, 56250 - 1};

//------------------------------------------ initial value definition------------------------------------------------

unsigned short  Item_Index[27] = {0, 6, 7, 80, 0, 4, 8, 0, 0, 1, 1, 9, 233, 68, BUFFER_SIZE, 0, 0, 40, 199, 140, 0, 0, 1, 1, 1, 100, 100};

//hide or view the item, 1 means hide
unsigned char   Hide_Index[27] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

//if the item needs refresh, 1 means refresh 
volatile unsigned char  Update[27];

// ------------------------------------------------------------------------------------------------------------

unsigned const char V_Unit[4][3] = {"uV", "mV", "V ", "kV"};
unsigned const char T_Unit[4][3] = {"ns", "us", "ms", "s "};
unsigned const char F_Unit[4][4] = {"Hz ", "Hz ", "kHz", "MHz"};
unsigned const char Battery_Status[5][4] = {"~`'", "~`}", "~|}", "{|}", "USB"};
unsigned const short Battery_Color[5] = {RED, YEL, GRN, GRN, GRN};
unsigned const char MODE_Unit[5][5] = {"AUTO", "NORM", "SING", "SCAN", "FIT"};
enum {WriteErr, NoFile, SDErr, NoCard, SaveOk, Failed, ReadErr} SD_Enums;
unsigned const char *SD_Msgs[] = {"Write Err", "No File", "SD Err", "No Card", "Save Ok", "Failed", "Read Err"};

unsigned short Tp;

unsigned char   FileNum[4] = "000";
I32STR_RES      Num;

/*******************************************************************************
 Function Name : Update_Item
 Description :  update the items based on Update[x]
*******************************************************************************/
void     Update_Item(void)
{
   if (Update[SYNC_MODE])
   { 
      Update[SYNC_MODE] = 0;
      DisplayField(SyncModeF, (Item_Index[RUNNING_STATUS] == RUN)?GRN:RED, MODE_Unit[Item_Index[SYNC_MODE]]);
   }
   if (Update[Y_SENSITIVITY])
   { 
      Update[Y_SENSITIVITY] = 0;
      DisplayField(VoltageDivF, WHITE, Item_V[Item_Index[Y_SENSITIVITY]]);
      Set_Range(Item_Index[Y_SENSITIVITY]); //voltage range
      Set_Y_Pos(Item_Index[Y_SENSITIVITY], Item_Index[V0]);
   }
   if (Update[X_SENSITIVITY])
   { 
      Update[X_SENSITIVITY] = 0;
      DisplayField(TimeDivF, YEL, Item_T[Item_Index[X_SENSITIVITY]]);
      Set_Base(Item_Index[X_SENSITIVITY]);
   }
   if (Update[GND_POSITION])// update Y.Position
   { 
      Update[GND_POSITION] = 0;
      Set_Y_Pos(Item_Index[Y_SENSITIVITY], Item_Index[V0]);
   }
   if (Update[MEASURE_KIND])//measure kind
   {
       unsigned char UpdateKind = 1;
       Update[MEASURE_KIND] = 0;
       switch (Item_Index[4])
       {
       case 0: // frequency
          if (MeFr)
          {
             Int32String(&Num, Frequency, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str, F_Unit[Num.decPos]);
             UpdateKind = 0;
          }
          break;
       case 1: // duty
         if (MeFr)
         {
             Int32String(&Num, Duty, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str, "%");
             UpdateKind = 0;
          }
          break;
       case 2: // Vrms
          if (MeFr)
          {
             Int32String(&Num, Vrms, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str, V_Unit[Num.decPos]);
             UpdateKind = 0;
          }
          break;
       case 3: // Vavg
          if (MeFr)
          {
             Int32String_sign(&Num, Vavg, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str, V_Unit[Num.decPos]);
             UpdateKind = 0;
          }
          break;
       case 4: // Vpp
         if (MeDC) {
             Int32String(&Num, Vpp, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str,  V_Unit[Num.decPos]);
             UpdateKind = 0;
         }
         break;

       case 5: // DCV
          if (MeDC) {
             Int32String_sign(&Num, Vdc, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str, V_Unit[Num.decPos]);
             UpdateKind = 0;
          }
          break;
       case 6: // Vmin
          if (MeDC) {
             Int32String_sign(&Num, Vmin, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str, V_Unit[Num.decPos]);
             UpdateKind = 0;
          }
          break;
       case 7: // Vmax
          if (MeDC) {
             Int32String_sign(&Num, Vmax, 3);
             DisplayFieldEx(MeasureF, WHITE, 0, (unsigned const char *)Num.str, V_Unit[Num.decPos]);
             UpdateKind = 0;
          }
          break;
       }
      if (UpdateKind) DisplayField(MeasureF, WHITE, Sub[MeFreq + Item_Index[MEASURE_KIND]].Cmd);
      if (Popup.Active && (Popup.Sub1 == MeFreq)) {
        unsigned char Sub = Item_Index[MEASURE_KIND] + Popup.Sub1;
        if (Popup.Sub != Sub) SelectSub(Sub);
        else RefreshMeasure();
      }
      if (Item_Index[CI] == MEASURE_KIND)
        DisplayField(InfoF, WHITE, Sub[MeFreq + Item_Index[MEASURE_KIND]].Cmd);
   }
   if (Update[POWER_INFO])
   { 
      Update[POWER_INFO] = 0;
      DisplayField(BatteryInfoF, Battery_Color[Item_Index[POWER_INFO]], Battery_Status[Item_Index[POWER_INFO]]);
   }
   if (Update[TRIG_LEVEL])
   { 
      Update[TRIG_LEVEL] = 0;
      Int32String_sign(&Num, (Item_Index[VT] - Item_Index[V0]) * V_Scale[Item_Index[Y_SENSITIVITY]], 3);
      DisplayFieldEx(TrigLevelF, YEL, "Tr", (unsigned const char *)Num.str, V_Unit[Num.decPos]);
   }
   if (Update[TRIG_SENSITIVITY])
   {
      Update[TRIG_SENSITIVITY] = 0;
      if (Item_Index[CI] == TRIG_SENSITIVITY)
      {
         Int32String(&Num, Item_Index[TRIG_SENSITIVITY] * V_Scale[Item_Index[Y_SENSITIVITY]], 3);
         DisplayFieldEx(InfoF, WHITE, "[S", (unsigned const char *)Num.str, V_Unit[Num.decPos]); 
      }
   }
   if (Update[TRIG_SLOPE])
   { 
      Update[TRIG_SLOPE] = 0;
      DisplayField(TrigKindF, YEL, (unsigned const char *)((Item_Index[TRIG_SLOPE] == RISING)?"^":"_")); 
   }
   if (Update[INPUT_ATTENUATOR])
   { 
      Update[INPUT_ATTENUATOR] = 0;
      if (Item_Index[CI] == INPUT_ATTENUATOR)
        DisplayField(InfoF, WHITE, (unsigned const char *)((Item_Index[INPUT_ATTENUATOR] == 0)?"Att. x1": "Att. x10"));
   }
   if (Update[SAVE_WAVE_IMAGE])
   { 
      Update[SAVE_WAVE_IMAGE] = 0;
      if (Item_Index[CI] == SAVE_WAVE_IMAGE)
      {
         Char_to_Str(FileNum, Item_Index[SAVE_WAVE_IMAGE]);
         DisplayFieldEx(InfoF, WHITE, "S", FileNum, ".BMP");
      }
   }
   if (Update[SAVE_WAVE_CURVE])
   { 
      Update[SAVE_WAVE_CURVE] = 0;
      if (Item_Index[CI] == SAVE_WAVE_CURVE)
      {
         Char_to_Str(FileNum, Item_Index[SAVE_WAVE_CURVE]);
         DisplayFieldEx(InfoF, WHITE, "S", FileNum, ".DAT");
      }
   }
   if (Update[LOAD_WAVE_CURVE])
   { 
      Update[LOAD_WAVE_CURVE] = 0;
      if (Item_Index[CI] == LOAD_WAVE_CURVE)
      {
         Char_to_Str(FileNum, Item_Index[LOAD_WAVE_CURVE]);
         DisplayFieldEx(InfoF, WHITE, "L", FileNum, ".DAT");
      }
   }
   if (Update[SHOW_WAVE_CURVE])
   { 
      Update[SHOW_WAVE_CURVE] = 0;
      if (Item_Index[CI] == SHOW_WAVE_CURVE)
      {
         DisplayField(InfoF, WHITE, (unsigned const char *)(Hide_Index[REF]?"Hide Ref": "Show Ref"));
      }
   }
   if (Update[SAVE_PROFILE])
   { 
      Update[SAVE_PROFILE] = 0;
      if (Item_Index[CI] == SAVE_PROFILE)
      {
        if (Item_Index[SAVE_PROFILE]) {
         Char_to_Str(FileNum, Item_Index[SAVE_PROFILE]);
         DisplayFieldEx(InfoF, WHITE, "S", FileNum, ".CFG");
        } else
         DisplayFieldEx(InfoF, WHITE, "S", "Flash", "");
      }
   }
   if (Update[LOAD_PROFILE])
   { 
      Update[LOAD_PROFILE] = 0;
      if (Item_Index[CI] == LOAD_PROFILE)
      {
        if (Item_Index[LOAD_PROFILE]) {
         Char_to_Str(FileNum, Item_Index[LOAD_PROFILE]);
         DisplayFieldEx(InfoF, WHITE, "L", FileNum, ".CFG");
        } else 
         DisplayFieldEx(InfoF, WHITE, "L", "Flash", "");
      }
   }
   if (Update[OUTPUT_FREQUENCY])
   {
      Update[OUTPUT_FREQUENCY] = 0;
      TIM4_PSC = Fout_PSC[Item_Index[OUTPUT_FREQUENCY]];
      TIM4_ARR = Fout_ARR[Item_Index[OUTPUT_FREQUENCY]];
      TIM4_CCR1 = (Fout_ARR[Item_Index[OUTPUT_FREQUENCY]] + 1) / 2;
      if (Item_Index[CI] == OUTPUT_FREQUENCY)
         DisplayFieldEx(InfoF, WHITE, "Fr.", Item_F[Item_Index[OUTPUT_FREQUENCY]], ""); 
   }
   if (Update[T2_CURSOR])
   {
      Update[T2_CURSOR] = 0;
      if (Item_Index[CI] == T2_CURSOR)
      {
         unsigned char i = Item_Index[X_SENSITIVITY] / 9;
         unsigned char j = Item_Index[X_SENSITIVITY] % 9;
         Int32String_sign(&Num, (Item_Index[T2] - 150  + BUFFER_SIZE - Item_Index[TP]) * T_Scale[j], 3);
         DisplayFieldEx(InfoF, WHITE, "T2", (unsigned const char *)Num.str, T_Unit[Num.decPos+i]); 
         //Draw_Dot_Ti(Item_Index[T1], ADD, LN2_COLOR);
      }
   }
   if (Update[T1_CURSOR])
   { 
      Update[T1_CURSOR] = 0;
      if (Item_Index[CI] == T1_CURSOR)
      {
         unsigned char i = Item_Index[X_SENSITIVITY] / 9;
         unsigned char j = Item_Index[X_SENSITIVITY] % 9;
         Int32String_sign(&Num, (Item_Index[T1] - 150 + BUFFER_SIZE - Item_Index[TP]) * T_Scale[j], 3);
         DisplayFieldEx(InfoF, WHITE, "T1", (unsigned const char *)Num.str, T_Unit[Num.decPos+i]); 
         //Draw_Dot_Ti(Item_Index[T1], ADD, LN2_COLOR);
      }
   }
   if (Update[TRIG_POS])
   { 
      Update[TRIG_POS] = 0;
      if (Item_Index[CI] == TRIG_POS)
      {
         unsigned char i = Item_Index[X_SENSITIVITY] / 9;
         unsigned char j = Item_Index[X_SENSITIVITY] % 9;
         Int32String_sign(&Num, (Item_Index[TP] - BUFFER_SIZE) * T_Scale[j], 3);
         DisplayFieldEx(InfoF, WHITE, "TP", (unsigned const char *)Num.str, T_Unit[Num.decPos+i]); 
      }
   }
   if (Update[DELTA_T])
   {
      unsigned char i = Item_Index[X_SENSITIVITY] / 9;
      unsigned char j = Item_Index[X_SENSITIVITY] % 9;

      Update[DELTA_T] = 0;
      Int32String(&Num, (Item_Index[T2] - Item_Index[T1]) * T_Scale[j], 3);
      DisplayFieldEx(DeltaTimeF, YEL, "[T", (unsigned const char *)Num.str, T_Unit[Num.decPos+i]);
   }
   if (Update[CALIBRATE_OFFSET]) {
      Update[CALIBRATE_OFFSET] = 0;
      if (Item_Index[CI] == CALIBRATE_OFFSET) {
        Int32String_sign(&Num,  V_Scale[Item_Index[Y_SENSITIVITY]] * (Item_Index[CALIBRATE_OFFSET] - 100), 3);
        DisplayFieldEx(InfoF, WHITE, "Of", (unsigned const char *)Num.str, V_Unit[Num.decPos]); 
      }
   }
   if (Update[CALIBRATE_RANGE]) {
      Update[CALIBRATE_RANGE] = 0;
      if (Item_Index[CI] == CALIBRATE_RANGE) {
        PercentString(&Num, Item_Index[CALIBRATE_RANGE] - 100);
        DisplayFieldEx(InfoF, WHITE, "Ra", (unsigned const char *)Num.str, "%"); 
      }
   }
   if (Update[V2_CURSOR])
   {
      Update[V2_CURSOR] = 0;
      if (Item_Index[CI] == V2_CURSOR) {
        // V2 voltage
        Int32String_sign(&Num, (Item_Index[V2] - Item_Index[V0]) * V_Scale[Item_Index[Y_SENSITIVITY]], 3);
        DisplayFieldEx(InfoF, WHITE, "V2", (unsigned const char *)Num.str, V_Unit[Num.decPos]);
      }
      Int32String(&Num, (Item_Index[V1] - Item_Index[V2]) * V_Scale[Item_Index[Y_SENSITIVITY]], 3);
      DisplayFieldEx(DeltaVoltageF, WHITE, "[V", (unsigned const char *)Num.str, V_Unit[Num.decPos]);
   }
   if (Update[V1_CURSOR])
   { 
      Update[V1_CURSOR] = 0;
      if (Item_Index[CI] == V1_CURSOR) {
        // V1 voltage
        Int32String_sign(&Num, (Item_Index[V1] - Item_Index[V0]) * V_Scale[Item_Index[Y_SENSITIVITY]], 3);
        DisplayFieldEx(InfoF, WHITE, "V1", (unsigned const char *)Num.str, V_Unit[Num.decPos]);
      }
      Int32String(&Num, (Item_Index[V1] - Item_Index[V2]) * V_Scale[Item_Index[Y_SENSITIVITY]], 3);
      DisplayFieldEx(DeltaVoltageF, WHITE, "[V", (unsigned const char *)Num.str, V_Unit[Num.decPos]);
   }
   if (Update[CURSORS])
   {
      Update[CURSORS] = 0;
      Tp = MIN_X + 150 + Item_Index[TP] - BUFFER_SIZE;
      if (Tp >= MAX_X) Tp = MAX_X;
      else if (Tp <= MIN_X) Tp = MIN_X;
      else {
         Draw_Ti_Line(Tp, Hide_Index[TP]?ERASE:ADD, CH2_COLOR);
         Draw_Ti_Mark(Tp, ADD, CH2_COLOR);
      }
      if (Item_Index[CI] == TRIG_POS) 
        Draw_Trig_Pos();
      else
        Erase_Trig_Pos();
      // draw Gnd position
      Draw_Vi_Mark(Item_Index[V0], ADD, WAV_COLOR);
      // draw T cursors
      Draw_Ti_Line(Item_Index[T1], Hide_Index[T1]?ERASE:ADD, LN2_COLOR);
      Draw_Ti_Line(Item_Index[T2], Hide_Index[T2]?ERASE:ADD, LN2_COLOR);
      Draw_Ti_Mark(Item_Index[T1], ADD, LN2_COLOR);
      Draw_Ti_Mark(Item_Index[T2], ADD, LN2_COLOR);
      // draw V cursors
      Draw_Vi_Line(Item_Index[V1], Hide_Index[V1]?ERASE:ADD, LN2_COLOR);
      Draw_Vi_Line(Item_Index[V2], Hide_Index[V2]?ERASE:ADD, LN2_COLOR);
      Draw_Vi_Mark(Item_Index[V1], ADD, LN2_COLOR);
      Draw_Vi_Mark(Item_Index[V2], ADD, LN2_COLOR);
      // draw trigger level/sensitivity
      if (!Hide_Index[VT]) {
        Draw_Vt_Line(Item_Index[VT], ADD, LN1_COLOR);
        Draw_Vi_Mark(Item_Index[VT], ADD, LN1_COLOR);
      } else if (!Hide_Index[VS]) {
        Draw_Vt_Line(Item_Index[VT] + Item_Index[VS], ADD, LN1_COLOR);
        Draw_Vt_Line(Item_Index[VT] - Item_Index[VS], ADD, LN1_COLOR);
        Draw_Vi_Mark(Item_Index[VT] + Item_Index[VS], ADD, LN1_COLOR);
        Draw_Vi_Mark(Item_Index[VT] - Item_Index[VS], ADD, LN1_COLOR);
      }
   }
}

void Erase_Sensitivity()
{
   Draw_Vt_Line(Item_Index[VT], ERASE, LN1_COLOR);
   Draw_Vi_Mark(Item_Index[VT], ERASE, LN1_COLOR);
   Draw_Vi_Mark(Item_Index[VT] + Item_Index[VS], ERASE, LN1_COLOR);
   Draw_Vi_Mark(Item_Index[VT] - Item_Index[VS], ERASE, LN1_COLOR);
   Draw_Vt_Line(Item_Index[VT] + Item_Index[VS], ERASE, LN1_COLOR);
   Draw_Vt_Line(Item_Index[VT] - Item_Index[VS], ERASE, LN1_COLOR);
}

void DrawMenu(void)
{
  unsigned char i;
  
  for (i = 0; i < (sizeof(Menu) / sizeof(Menu[0])); i++)
  {
    Display_Str(MAX_X + 3, MAX_Y - 17 - i * 20, YEL, PRN, Menu[i].Cmd);
  }
}

void HidePopup(void)
{
   unsigned short i;

   Popup.Active = 0;
   Display_Grid(); // draw grid
   for (i = 0; i < X_SIZE; i++)
   {
      Draw_SEG(i, Erase_Buffer[i], View_Buffer[i], WAV_COLOR);
   }
   if (!Hide_Index[REF])
     Draw_Reference();
}

void ShowPopup(void)
{
   unsigned char i, t;
   short y;
  
   // find index of first sub option
   Popup.Sub = Popup.Sub1 = Popup.Sub2 = Menu[CurrentMenu].Sub;
   while ((Popup.Sub1 > 0) && (Sub[Popup.Sub1].Top != 1)) Popup.Sub1--;

   // find index of last sub option
   while ((Popup.Sub2 < (N_SUB - 1)) && (Sub[Popup.Sub2 + 1].Top != 1)) Popup.Sub2++;

   // find width of largest sub option cmd string   
   Popup.width = 0;
   for (i = Popup.Sub1; i <= Popup.Sub2; i++) {
     t = strlen((char const *)Sub[i].Cmd);
     if (t > Popup.width) Popup.width = t;
   }
   if (Popup.Sub1 == MeFreq) Popup.width += 9;  // space + 5 digits + 3 kind
   Popup.width = Popup.width * 8 + 8;
   
   // determine height
   Popup.height = (Popup.Sub2 - Popup.Sub1 + 1) * 18 + 8;
   
   // determine x1, y1 pos for popup
   Popup.x = LCD_WIDTH - 24 - Popup.width;
   Popup.y = MAX_Y - 17 + 7 - CurrentMenu * 20 - Popup.height / 2;
   if (Popup.y < 24) Popup.y = 24;
   while ((Popup.y + Popup.height) > (LCD_HEIGHT - 24)) Popup.y--;
   Popup.Sub = Menu[CurrentMenu].Sub;
   
   // draw popup
   Rounded_Rectangle(Popup.x, Popup.y, Popup.width, Popup.height, FRM_COLOR);
   y = Popup.y + Popup.height - 4 - 2 - 14;
   for (i = Popup.Sub1; i <= Popup.Sub2; i++) {
     unsigned char Typ = (Popup.Sub == i)?INV:PRN;
     Display_Str(Popup.x + 4, y, WHITE, Typ, Sub[i].Cmd);
     if (Typ == INV) {
       t = strlen((char const *)Sub[i].Cmd);
       Fill_Rectangle(Popup.x + 4 + t * 8, y, Popup.width - 8 - t * 8, 14, WHITE);
     }
     y -= 18;
   }
   Popup.Active = 1;
   if (Popup.Sub1 == MeFreq) RefreshMeasure();
}

void RefreshMeasure(void)
{
   unsigned char i;
   short y, x1, x2;
  
   x1 = Popup.x + Popup.width - 4 - 1 - 8 * 8;
   x2 = x1 + 8 * 5;
   y = Popup.y + Popup.height - 4 - 2 - 14;
   for (i = Popup.Sub1; i <= Popup.Sub2; i++) {
     unsigned char Typ = (Popup.Sub == i)?INV:PRN;
     if (MeFr || (MeDC && ((i - Popup.Sub1) >= 5))) {
       switch (i - Popup.Sub1) {
         case 0: // frequency
            Int32String(&Num, Frequency, 3);
            Display_Str(x1+8, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, F_Unit[Num.decPos]);
            break;
         case 1: // duty
            Int32String(&Num, Duty, 3);
            Display_Str(x1+8, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, "%");
            break;
         case 2: // Vrms
            Int32String(&Num, Vrms, 3);
            Display_Str(x1+8, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, V_Unit[Num.decPos]);
            break;
         case 3: // Vavg
            Int32String_sign(&Num, Vavg, 3);
            Display_Str(x1, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, V_Unit[Num.decPos]);
            break;
         case 4: // Vpp
            Int32String(&Num, Vpp, 3);
            Display_Str(x1+8, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, V_Unit[Num.decPos]);
           break;
         case 5: // DCV
            Int32String_sign(&Num, Vdc, 3);
            Display_Str(x1, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, V_Unit[Num.decPos]);
            break;
         case 6: // Vmin
            Int32String_sign(&Num, Vmin, 3);
            Display_Str(x1, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, V_Unit[Num.decPos]);
            break;
         case 7: // Vmax
            Int32String_sign(&Num, Vmax, 3);
            Display_Str(x1, y, WHITE, Typ, (unsigned const char *)Num.str);
            Display_Str(x2, y, WHITE, Typ, V_Unit[Num.decPos]);
            break;
       }
     } else {
       Fill_Rectangle(x1, y, Popup.width - (x1 - Popup.x) - 4, 14, (Typ == PRN)?FRM_COLOR:WHITE);
       Display_Str(x1 + 8 * 2, y, WHITE, Typ, "**");
     }
     y -= 18;
   }
}

void SelectSub(unsigned char pi)
{
   unsigned char t;
   short y;

   if ((pi == 255) || (pi < Popup.Sub1)) pi = Popup.Sub2;
   else if (pi > Popup.Sub2) pi = Popup.Sub1;
   // remove selection
   y = Popup.y + Popup.height - 4 - 2 - 14 - 18 * (Popup.Sub - Popup.Sub1);
   Display_Str(Popup.x + 4, y, WHITE, PRN, Sub[Popup.Sub].Cmd);
   t = strlen((char const *)Sub[Popup.Sub].Cmd);
   Fill_Rectangle(Popup.x + 4 + t * 8, y, Popup.width - 8 - t * 8, 14, FRM_COLOR);

   // add selection
   Popup.Sub = pi;
   y = Popup.y + Popup.height - 4 - 2 - 14 - 18 * (Popup.Sub - Popup.Sub1);
   Display_Str(Popup.x + 4, y, WHITE, INV, Sub[Popup.Sub].Cmd);
   t = strlen((char const *)Sub[Popup.Sub].Cmd);
   Fill_Rectangle(Popup.x + 4 + t * 8, y, Popup.width - 8 - t * 8, 14, WHITE);

   if (Popup.Sub1 == MeFreq) Item_Index[MEASURE_KIND] = Popup.Sub - Popup.Sub1;
   Menu[CurrentMenu].Sub = Popup.Sub;
   DisplayField(InfoF, WHITE, Sub[Menu[CurrentMenu].Sub].Cmd);

   Item_Index[CI] = Sub[Popup.Sub].ci;
}

void SelectMenu(unsigned char mi)
{
    if (mi == 255) mi = N_MENU - 1;
    else if (mi >= N_MENU) mi = 0;
    Display_Str(MAX_X + 3, MAX_Y - 17 - CurrentMenu * 20, YEL, PRN, Menu[CurrentMenu].Cmd);
    Display_Str(MAX_X + 3, MAX_Y - 17 - mi * 20, WHITE, INV, Menu[mi].Cmd);
    CurrentMenu = mi;

    DisplayField(InfoF, WHITE, Sub[Menu[CurrentMenu].Sub].Cmd);
    Item_Index[CI] = Sub[Menu[CurrentMenu].Sub].ci;
}

void DisplayFieldEx(unsigned char fi, unsigned short Color, unsigned const char *pre, unsigned const char *str, unsigned const char *suf)
{
    unsigned short w1 = 0, w2 = 0, w3 = 0, x;

    if (pre) {
      w1 = strlen((char const *)pre) * 8;
      Display_Str(Field[fi].x, Field[fi].y, Color, PRN, pre);
      Fill_Rectangle(Field[fi].x + w1, Field[fi].y, 4, 14, FRM_COLOR);
      w1 += 4;
    }

    w2 = strlen((char const *)str) * 8;
    Display_Str(Field[fi].x + w1, Field[fi].y, Color, PRN, str);

    w3 = strlen((char const *)suf) * 8;
    Display_Str(Field[fi].x + w1 + w2, Field[fi].y, Color, PRN, suf);

    x = Field[fi].width - w1 - w2 - w3;
    Fill_Rectangle(Field[fi].x + w1 + w2 + w3, Field[fi].y, x, 14, FRM_COLOR);
}

void DisplayField(unsigned char fi, unsigned short Color, unsigned const char *str)
{
    DisplayFieldEx(fi, Color, 0, str, "");
}


void LoadWave(void)
{
  Update[LOAD_WAVE_CURVE] = 0;
  if (SD_Card_ON())
  {
    Hide_Index[REF] = 1; // hide reference waveform
    Erase_Reference();
    if (FAT_Info() == 0)
    {
       Char_to_Str(FileNum, Item_Index[LOAD_WAVE_CURVE]);
       if (Open_File("FILE",FileNum,"DAT") == 0)
       {
          if (Read_File() == 0)
          {
             memcpy(Ref_Buffer, F_Buff + 2, 300);
             Hide_Index[REF] = 0; // show new reference waveform
             Draw_Reference();
             Update[LOAD_WAVE_CURVE] = 1;
          } else
             DisplayField(InfoF, WHITE, SD_Msgs[ReadErr]);
       } else
          DisplayField(InfoF, WHITE, SD_Msgs[NoFile]);
    } else
       DisplayField(InfoF, WHITE, SD_Msgs[SDErr]);
  } else
     DisplayField(InfoF, WHITE, SD_Msgs[NoCard]);
}

void SaveWave(void)
{
  Update[SAVE_WAVE_CURVE] = 0;
  if (SD_Card_ON())
  {
      if (FAT_Info() == 0)
      {
         Char_to_Str(FileNum, Item_Index[SAVE_WAVE_CURVE]);
         if (Open_File("FILE",FileNum,"DAT") == 0)
         {
            F_Buff[0] = 0;
            F_Buff[1] = 0;
            memcpy(F_Buff + 2, View_Buffer, 300);
            if (Write_File() == 0)
            {
               if (Item_Index[SAVE_WAVE_CURVE] < 255)
                  Item_Index[SAVE_WAVE_CURVE]++;
               Update[SAVE_WAVE_CURVE] = 1;
            } else
               DisplayField(InfoF, WHITE, SD_Msgs[WriteErr]);
         } else
            DisplayField(InfoF, WHITE, SD_Msgs[NoFile]);
      } else
         DisplayField(InfoF, WHITE, SD_Msgs[SDErr]);
  } else
     DisplayField(InfoF, WHITE, SD_Msgs[NoCard]);
}

void SaveWaveImage(void)
{
  Update[SAVE_WAVE_IMAGE] = 0;
  if (SD_Card_ON())
  {
      if (FAT_Info() == 0)
      {
         Char_to_Str(FileNum, Item_Index[SAVE_WAVE_IMAGE]);
         if (Open_File("IMAGE",FileNum,"BMP") == 0)
         {
            if (Writ_BMP_File() == 0)
            {
               if (Item_Index[SAVE_WAVE_IMAGE] < 255)
                  Item_Index[SAVE_WAVE_IMAGE]++;
               Update[SAVE_WAVE_IMAGE] = 1;
            } else
               DisplayField(InfoF, WHITE, SD_Msgs[WriteErr]);
         } else
            DisplayField(InfoF, WHITE, SD_Msgs[NoFile]);
      } else
         DisplayField(InfoF, WHITE, SD_Msgs[SDErr]);
  } else
     DisplayField(InfoF, WHITE, SD_Msgs[NoCard]);
}

void SaveConfig(void)
{
  Update[SAVE_PROFILE] = 0;
  if (Item_Index[SAVE_PROFILE]) {
    if (SD_Card_ON())
    {
        if (FAT_Info() == 0)
        {
           Char_to_Str(FileNum, Item_Index[SAVE_PROFILE]);
           if (Open_File("FILE",FileNum,"CFG") == 0)
           {
              PutConfig();
              if (Write_File() != 0)
                 DisplayField(InfoF, WHITE, SD_Msgs[WriteErr]);
           } else
              DisplayField(InfoF, WHITE, SD_Msgs[NoFile]);
        } else
           DisplayField(InfoF, WHITE, SD_Msgs[SDErr]);
    } else
       DisplayField(InfoF, WHITE, SD_Msgs[NoCard]);
  } else {
    PutConfig();
    if (Write_Parameter() == FLASH_COMPLETE)
       DisplayField(InfoF, WHITE, SD_Msgs[SaveOk]);
    else
       DisplayField(InfoF, WHITE, SD_Msgs[Failed]);
  }
}

void LoadConfig(void)
{
  if (Item_Index[LOAD_PROFILE]) {
    Update[LOAD_PROFILE] = 0;
    if (SD_Card_ON())
    {
      if (FAT_Info() == 0)
      {
         Char_to_Str(FileNum, Item_Index[LOAD_PROFILE]);
         if (Open_File("FILE",FileNum,"CFG") == 0)
         {
            if (Read_File() == 0)
              RestoreConfig();
            else
               DisplayField(InfoF, WHITE, SD_Msgs[ReadErr]);
         } else
            DisplayField(InfoF, WHITE, SD_Msgs[NoFile]);
      } else
         DisplayField(InfoF, WHITE, SD_Msgs[SDErr]);
    } else
       DisplayField(InfoF, WHITE, SD_Msgs[NoCard]);
  } else {
    Read_Parameter();
    RestoreConfig();
  }
}

unsigned char CheckSub(unsigned char SubOrg, unsigned char SubNew)
{
   unsigned char Sub1, Sub2;
  
   // find index of first sub option
   Sub1 = Sub2 = SubOrg;
   while ((Sub1 > 0) && (Sub[Sub1].Top != 1)) Sub1--;

   // find index of last sub option
   while ((Sub2 < (N_SUB - 1)) && (Sub[Sub2 + 1].Top != 1)) Sub2++;
   
   if (SubNew ==  SaveProfile) SubNew = SaveImage;
   if ((SubNew >= Sub1) && (SubNew <= Sub2)) return SubNew;
   
   return SubOrg;
}

void PutConfig(void)
{
  unsigned short i;

  F_Buff[0] = 0x30;
  F_Buff[1] = 0x00;
  memcpy(F_Buff + 2, Item_Index, sizeof(Item_Index));
  memcpy(F_Buff + 64, Hide_Index, sizeof(Hide_Index));
  for (i = 0; i < 9; i ++)
    F_Buff[2 + 96 + i] = Menu[i].Sub;
}

void RestoreConfig(void)
{
   unsigned char t = Menu[CurrentMenu].Sub;    // preserve active menu option
   unsigned short i;
   
   if ((F_Buff[0] == 0x30) && (F_Buff[1] == 0x00)) {
      Erase_Sensitivity();
      memcpy(Item_Index, F_Buff + 2, sizeof(Item_Index));
      memcpy(Hide_Index, F_Buff + 64, sizeof(Hide_Index));
      for (i = 0; i < 9; i ++)
        Menu[i].Sub = CheckSub(Menu[i].Sub,F_Buff[2 + 96 + i]);
      Menu[CurrentMenu].Sub = t;  // restore active menu
   }
   ApplyConfig();
}

void ApplyConfig(void)
{
   memset(Signal_Buffer, 0xff, sizeof(Signal_Buffer));
   memset(View_Buffer, 0xff, sizeof(View_Buffer));
   memset(Erase_Buffer, 0xff, sizeof(Erase_Buffer));
   memset((void *)Update, 1, sizeof(Update));
   Item_Index[RUNNING_STATUS] = RUN;
   Item_Index[POWER_INFO] = 3;
   if (Item_Index[TP] > BUFFER_SIZE) Item_Index[TP] = BUFFER_SIZE;
   Popup.Active = 0;
   Item_Index[CI] = Sub[Menu[CurrentMenu].Sub].ci;
   Display_Grid();
   if (!Hide_Index[REF])
     Draw_Reference();
}

/******************************** END OF FILE *********************************/
