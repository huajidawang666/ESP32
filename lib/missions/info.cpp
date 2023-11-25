#include <Arduino.h>
#include <U8g2lib.h>
#include "missions.h"
#define LEFT 12
#define RIGHT 14
#define UP 27
#define DOWN 26
#define ENTER 25
#define INFOLINE 20

extern U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2;
extern int buttonStatus[6];
extern bool finishMission;
extern unsigned short int brightness;
int info_pageStart = 1;
int displayline;

String info[INFOLINE + 1] = {
                   "|-No more byd this.-|",
                   " My first ESP32 Proj ",
                   "  This is the first  ",
                   "Esp 32 Proj made by  ",
                   "huajidawang666, which",
                   "features wide range  ",
                   "of functions and     ",
                   "games.               ",
                   "  Project is under   ",
                   "MIT license. Github  ",
                   "repository: https://g",
                   "ithub.com/huajidawang",
                   "666/ESP32",
                   "",
                   "",
                   "",
                   "",
                   "",
                   "",
                   "",
                   ""
                   };

void info_init() {
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_t0_11_tr);
    u8g2.setContrast(brightness * 255 / 20);
    u8g2.setFontDirection(0);
    info_pageStart = 1; 
    displayline = INFOLINE + 1;
    while (info[displayline - 1] == "") {
        displayline --;
    }
}

void info_loop() {

    if (readButton(UP, 3) && info_pageStart > 1) {
        info_pageStart --;
    } else if (readButton(DOWN, 4) && info_pageStart < displayline - 5) {
        info_pageStart ++;
    }
    u8g2.clearBuffer();
    
    for (int i = 1; i <= 5; i++) {
        u8g2.setCursor(0, 10 * i);
        u8g2.print(info[i + info_pageStart - 1]);
    }

    u8g2.drawHLine(0, 51, 128);
    u8g2.setCursor(0, 63);
    u8g2.print("[<- Back]");
    u8g2.sendBuffer();
    if (readButton(ENTER, 5)) {
        finishMission = true;
    }
}