#include <Arduino.h>
#include <U8g2lib.h>
#include "settings.h"
#include "missions.h"

#define LEFT 12
#define RIGHT 14
#define UP 27
#define DOWN 26
#define ENTER 25

extern unsigned short int brightness;
extern U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;
extern int buttonStatus[6];
extern bool finishSettingMission;

void brightness_readInput();

void brightness_init() {
    u8g2.begin();
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_7x14_tr);
    u8g2.setFontDirection(0);
}

void brightness_loop() {
    u8g2.clearBuffer();
    u8g2.setContrast(brightness * 255 / 20);
    u8g2.setCursor(0, 16);
    u8g2.print("    Brightness    ");
    brightness_readInput();
    u8g2.drawFrame(16, 24, 96, 8);
    u8g2.drawBox(16, 24, brightness * 96 / 20, 8);
    u8g2.setCursor(0, 48);
    u8g2.print("<-0           20->");
    u8g2.setCursor(58, 48);
    u8g2.print(brightness);
    u8g2.setCursor(0, 62);
    u8g2.print("       [OK]       ");
    /*
        Brightness   
      [=====      ]  
    <-0     5    10->
           [OK]      
    */
   u8g2.sendBuffer();
}

void brightness_readInput() {
    if (readButton(LEFT, 1) && brightness > 0){
        brightness --;
    } else if (readButton(RIGHT, 2) && brightness < 20) {
        brightness ++;
    } else if (readButton(ENTER, 5)) {
        finishSettingMission = true;
    }
}