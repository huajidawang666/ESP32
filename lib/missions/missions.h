#include <U8g2lib.h>
#ifndef UTT_H_INCLUDED
#define UTT_H_INCLUDED

//Ultimate Tictactoe
void utt_init(); void utt_loop();
//Info Line
void info_loop();
bool readButton(int pin, int direction);
extern bool finishMission;
extern int buttonStatus[6];
extern U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;
#endif