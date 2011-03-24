/*******************************************************************************
 File name  : Menu.h
 *******************************************************************************/
#ifndef __Menu_h
#define __Menu_h

#define KEYCODE_VOID    0
#define KEYCODE_PLAY    1
#define KEYCODE_M       2
#define KEYCODE_B       3

#define KEYCODE_UP      4
#define KEYCODE_DOWN    5
#define KEYCODE_LEFT    6
#define KEYCODE_RIGHT   7

// update index
#define SYNC_MODE         0
#define Y_SENSITIVITY     1
#define X_SENSITIVITY     2
#define GND_POSITION      3
#define MEASURE_KIND      4
#define POWER_INFO        5
#define TRIG_SENSITIVITY  6
#define TRIG_SLOPE        7
#define INPUT_ATTENUATOR  8
#define SAVE_WAVE_CURVE   9
#define LOAD_WAVE_CURVE   10
#define OUTPUT_FREQUENCY  11
#define T2_CURSOR         12
#define T1_CURSOR         13
#define TRIG_POS          14
#define RUNNING_STATUS    15
#define DELTA_T           16
#define V2_CURSOR         17
#define V1_CURSOR         18
#define TRIG_LEVEL        19
#define CURSORS           20
#define SHOW_WAVE_CURVE   21
#define SAVE_WAVE_IMAGE   22
#define SAVE_PROFILE      23
#define LOAD_PROFILE      24
#define CALIBRATE_OFFSET  25
#define CALIBRATE_RANGE   26

// item/hide index
#define REF                1    // reference wave
#define V0                 3    // Y axis ground position
#define VS                 6    // Y axsis trigger sensitivity
#define T2                12    // X cursor 2
#define T1                13    // X cursor 1
#define TP                14    // X axis trigger point
#define CI                16    // current item index
#define V2                17    // Y cursor 2
#define V1                18    // Y cursor 1
#define VT                19    // Y axis trigger level

#define N_MENU (sizeof(Menu) / sizeof(Menu[0]))
#define N_SUB (sizeof(Sub) / sizeof(Sub[0]))

typedef struct _SubMenuType {
  unsigned const char *Cmd;
  unsigned const char Top;    // set to 1 for topmost sub-item in group
  unsigned const char ci;     // current index equivalent
} SubMenuType;

typedef struct _MainMenuType {
  unsigned const char *Cmd;   // two letter command abbreviation
  unsigned const char *Info;  // command descriptive text
  unsigned char Sub;          // index to selected sub menu
} MainMenuType;

typedef struct _FieldType {
  unsigned const short x;     // x-position, pixels
  unsigned const short y;     // y-position, pixels 
  unsigned const char width;  // pixel width
} FieldType;

typedef struct _PopupType {
  short x;              // x-position, pixels
  short y;              // y-position pixels
  unsigned char width;  // pixel width
  unsigned char height; // pixel height
  unsigned char Sub;    // index of highlighted sub option
  unsigned char Sub1;   // index of first sub option
  unsigned char Sub2;   // index of last sub option
  unsigned char Active; // set to 1 when popup is active
} PopupType;

extern MainMenuType Menu[];
extern const SubMenuType Sub[];
extern const FieldType Field[];
extern PopupType Popup;
extern unsigned char CurrentMenu;

extern unsigned const char Item_V[20][10], Item_T[22][10];
extern unsigned const int V_Scale[20], T_Scale[22], Fout_ARR[16];
extern unsigned const char V_Unit[4][3], T_Unit[4][3];
extern unsigned const short Scan_PSC[22], Scan_ARR[22];
extern unsigned short Y_POSm[20], Km[20];
extern short    Y_POSn[20];
extern unsigned short Tp;
extern unsigned char F_Buff[512];
extern unsigned char FileNum[4];
extern volatile unsigned char Update[27];
extern unsigned short Item_Index[27];
extern unsigned char Hide_Index[27];

void Update_Item(void);
void Erase_Sensitivity(void);
void DrawMenu(void);
void ShowPopup(void);
void HidePopup(void);
void SelectMenu(unsigned char NewMenu);
void SelectSub(unsigned char pi);
unsigned char CheckSub(unsigned char SubOrg, unsigned char SubNew);
void RefreshMeasure(void);
void DisplayFieldEx(unsigned char fi, unsigned short Color, unsigned const char *pre, unsigned const char *str, unsigned const char *suf);
void DisplayField(unsigned char fi, unsigned short Color, unsigned const char *str);

void SaveWave(void);
void LoadWave(void);
void SaveWaveImage(void);
void SaveConfig(void);
void LoadConfig(void);
void ApplyConfig(void);
void PutConfig(void);
void RestoreConfig(void);

#endif
/******************************** END OF FILE *********************************/
