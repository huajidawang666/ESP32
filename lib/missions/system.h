#include <U8g2lib.h>
#ifndef MISSIONS_H_INCLUDED
#define MISSIONS_H_INCLUDED

#define LEFT 12
#define RIGHT 14
#define UP 27
#define DOWN 26
#define ENTER 25

//Ultimate Tictactoe
void utt_init(); void utt_loop();
//Info Line
void info_init(); void info_loop();
//Settings
void settings_init(); void settings_loop();
//Chemistry Table
void chemistrytable_init(); void chemistrytable_loop();


bool readButton(int pin, int direction);

extern long realTime;
extern bool finishMission;
extern unsigned short int brightness;
extern int buttonStatus[6];
extern U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;
#endif