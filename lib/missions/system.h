#include <U8g2lib.h>
#ifndef MISSIONS_H_INCLUDED
#define MISSIONS_H_INCLUDED

#define LEFT 12
#define RIGHT 14
#define UP 27
#define DOWN 26
#define ENTER 25

extern long realTime;
extern bool finishMission;
extern unsigned short int brightness;
extern int buttonStatus[6];
extern U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;

//TicTacToe
void tictactoe();
//Ultimate Tictactoe
void utt_init(); void utt_loop();
//Chemistry Table
void chemistrytable_init(); void chemistrytable_loop();
//Chicken and Eggs
void chickenAndEgg();
//Info Line
void info_init(); void info_loop();
//Settings
void settings_init(); void settings_loop();

inline bool readButton(int pin, int direction) {
  // direction: 1 for left, 2 for right, 3 for up, 4 for down
  if (digitalRead(pin)) {
    // Serial.println("button pressed1");
    delay(10);
    if (digitalRead(pin) && buttonStatus[direction] == 0) {
      // Serial.println("button pressed2");
      buttonStatus[direction] = 1;
      return true;
    } else {
      return false;
    }
  } else if (!digitalRead(pin)) {
    delay(10);
    if (!digitalRead(pin) && buttonStatus[direction] == 1) {
      buttonStatus[direction] = 0;
      return false;
    }
  }
  return false;
}




#endif