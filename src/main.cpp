/*
|--lib
|  |
|  |--missions
|  |  |- utt.cpp
|  |  |- info.cpp
|  |  |- settings.cpp
|  |  |- system.h
|  |  |--settings
|  |     |- brightness.cpp
|  |     |- settings.h
|  |     |- timeanddate.cpp
|  |
|  |- README
|
|- platformio.ini
|--src
    |- main.cpp --> THIS FILE
*/

#include <Arduino.h>
#include <U8g2lib.h>
#include "system.h"
#include "settings/settings.h"
#define LIST_AMOUNT 10
#define SPARETIME 30000 // milliseconds

//system
unsigned short int brightness = 10;

String list[LIST_AMOUNT + 1] = {"", "TicTacToe", "Ultimate TicTacToe", "Chicken And Eggs", "Chemistry Periodic Table", "Snake", "Settings", "Info", "Test8", "Test9", "Test10"};
int pageStart = 1;
int cursor = 1;
int showcaseIndex = 1;
int startDisplayTime = 0;
int currentTime = 0;
long realTime = 0;
int spareTime = 0; // no action in 30s turn to home page, showing time
bool mode = 1; // 0 stands for displayhome, 1 stands for displaylist

bool wifiStatus = false;

void home_readInput();
void startMission(int index);
void transition(bool direction);
void slide(bool direction);
void displayHome();
void displayList();

bool finishMission = true;

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/5, /* data=*/18, 
                                             /* cs=*/21, /* dc=*/19, /* reset=*/15);
int buttonStatus[6] = {0};






void setup() {
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setContrast(brightness * 255 / 20);
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_7x14_tr);
  u8g2.setFontDirection(0);
  pinMode(12, INPUT_PULLDOWN);
  pinMode(14, INPUT_PULLDOWN);
  pinMode(27, INPUT_PULLDOWN);
  pinMode(26, INPUT_PULLDOWN);
  pinMode(25, INPUT_PULLDOWN);
  startDisplayTime = millis();
  spareTime = startDisplayTime;
  
}

void loop() {
  realTime = realTime % 86400000;
  u8g2.setFont(u8g2_font_7x14_tr);
  u8g2.setFontDirection(0);
  currentTime = millis();
  u8g2.clearBuffer();
  if (mode) {
    displayList();
    if (millis() > spareTime + SPARETIME) {
        mode = !mode;
    }
  } else {
    displayHome();
  }
  home_readInput();
}

void displayHome() {
    int runday = millis() / 1000 / 60 / 60 / 24;
    int runhr = (millis() / 1000 / 60 / 60) % 24;
    int runmin = (millis() / 1000 / 60) % 60;

    int hr = ((realTime + millis()) / 1000 / 60 / 60) % 24;
    int min = ((realTime + millis()) / 1000 / 60) % 60;
    String timenow = String(hr < 10 ? "0" + String(hr) : String(hr)) + ":" + String(min < 10 ? "0" + String(min) : String(min))
                    //  + ":" + String(((realTime + millis()) / 1000) % 60)
                     ;
    String runtimenow = "Runtime: " + String(runday) + "d "
                         + String(runhr < 10 ? "0" + String(runhr) : String(runhr)) + "h "
                         + String(runmin < 10 ? "0" + String(runmin) : String(runmin)) + "m "
                         ;
                         
    u8g2.setFont(u8g2_font_t0_11_tr);
    u8g2.setCursor(2, 12);
    u8g2.print(runtimenow);
    u8g2.setFont(u8g2_font_logisoso32_tn);
    u8g2.setCursor(19, 48);
    // u8g2.setCursor(0, 48);
    u8g2.print(timenow);
    u8g2.sendBuffer();
}

void displayList() {
  for (int i = 1; i <= 4; i ++) {
    if (cursor == i + pageStart - 1) {
        u8g2.drawLine(3, i * 16 - 11, 6, i * 16 - 8);
        u8g2.drawLine(3, i * 16 - 4, 6, i * 16 - 7);
        u8g2.setCursor(9, 16 * i - 2);
        String showSelect = list[i + pageStart - 1];
        if (list[i + pageStart - 1].length() > 17) { // max display length with an arrow is 17
            if (currentTime - startDisplayTime > 500) {
                startDisplayTime = currentTime;
                showcaseIndex += 1;
                if (showcaseIndex > list[i + pageStart - 1].length()) {
                    showcaseIndex = 1;
                }
            }
            showSelect = list[i + pageStart - 1].substring(showcaseIndex - 1, (showcaseIndex + 16 < list[i + pageStart - 1].length()) ? (showcaseIndex + 16) : list[i + pageStart - 1].length());
        }
        u8g2.print(showSelect);
    } else {
        u8g2.setCursor(3, 16 * i - 2);
        u8g2.print(list[i + pageStart - 1]);
    }
  }
  u8g2.sendBuffer();
}

void home_readInput() {
    if (mode) {
        if (readButton(LEFT, 1)) {
            mode = !mode;
        }
        if (readButton(UP, 3)) {
            spareTime = millis();
            if (cursor == pageStart && pageStart > 1) {
                slide(1);
                pageStart--;
                cursor--;
                showcaseIndex = 1;
            } else if (cursor > pageStart) {
                transition(1);
                cursor--;
                showcaseIndex = 1;
            }
        }
        if (readButton(DOWN, 4)) {
            spareTime = millis();
            if (cursor == pageStart + 3 && pageStart + 3 < LIST_AMOUNT) {
                slide(0);
                pageStart++;
                cursor++;
                showcaseIndex = 1;
            } else if (cursor < pageStart + 3) {
                transition(0);
                cursor++;
                showcaseIndex = 1;
            }
        }
        if (readButton(ENTER, 5)) {
            startMission(cursor);
        }
    } else {
        if (readButton(LEFT, 1) || readButton(RIGHT, 2) || readButton(UP, 3) || readButton(DOWN, 4) || readButton(ENTER, 5)) {
            mode = !mode;
            spareTime = millis();
        }
    }
}

void startMission(int index) {
    if (index == 1) {
        finishMission = false;
        tictactoe();
    } else if (index == 2) {
        utt_init();
        finishMission = false;
        while (!finishMission) {
            utt_loop();
        }
    } else if (index == 3) {
        finishMission = false;
        chickenAndEgg();
    } else if (index == 4) {
        chemistrytable_init();
        finishMission = false;
        while (!finishMission) {
            chemistrytable_loop();
        }
    } else if (index == 5) {
        snake();
    } else if (index == 6) {
        settings_init();
        finishMission = false;
        while (!finishMission) {
            settings_loop();
        }
    } else if (index == 7) {
        info_init();
        finishMission = false;
        while (!finishMission) {
            info_loop();
        }
    }
    spareTime = millis();
}

/*
DONT MIND THE FUCK INDEX THEY FUCKED MY BRAIN UP
DONT MIND THE FUCK INDEX THEY FUCKED MY BRAIN UP
DONT MIND THE FUCK INDEX THEY FUCKED MY BRAIN UP
DONT MIND THE FUCK INDEX THEY FUCKED MY BRAIN UP
DONT MIND THE FUCK INDEX THEY FUCKED MY BRAIN UP
*/

void transition(bool direction) {//1 stands for up, 0 stands for down
    int frame = 0; // max = 4
    while (frame <= 4) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_7x14_tr);
        u8g2.setFontDirection(0);
        if (direction == 0) {
            for (int i = 1; i <= 4; i ++) {
                if (cursor == i + pageStart - 1) {
                    u8g2.drawLine(3, i * 16 - 11 + 4 * frame, 6, i * 16 - 8 + 4 * frame);
                    u8g2.drawLine(3, i * 16 - 4 + 4 * frame, 6, i * 16 - 7 + 4 * frame);
                    u8g2.setCursor(9 - 6 * frame / 4, 16 * i - 2);
                    u8g2.print(list[i + pageStart - 1]);
                } else if (cursor == i + pageStart - 2) {
                    u8g2.setCursor(3 + 6 * frame / 4, 16 * i - 2);
                    u8g2.print(list[i + pageStart - 1]);    
                } else {
                    u8g2.setCursor(3, 16 * i - 2);
                    u8g2.print(list[i + pageStart - 1]);
                }
            }
            u8g2.sendBuffer();
        } else {
            for (int i = 1; i <= 4; i ++) {
                if (cursor == i + pageStart - 1) {
                    u8g2.drawLine(3, i * 16 - 11 - 4 * frame, 6, i * 16 - 8 - 4 * frame);
                    u8g2.drawLine(3, i * 16 - 4 - 4 * frame, 6, i * 16 - 7 - 4 * frame);
                    u8g2.setCursor(9 - 6 * frame / 4, 16 * i - 2);
                    u8g2.print(list[i + pageStart - 1]);
                } else if (cursor == i + pageStart) {
                    u8g2.setCursor(3 + 6 * frame / 4, 16 * i - 2);
                    u8g2.print(list[i + pageStart - 1]);    
                } else {
                    u8g2.setCursor(3, 16 * i - 2);
                    u8g2.print(list[i + pageStart - 1]);
                }
            }
            u8g2.sendBuffer();
        }
        frame++;
    }
}

void slide(bool direction) {
    int frame = 0; // max = 4
    while (frame <= 4) {
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_7x14_tr);
        u8g2.setFontDirection(0);
        if (direction == 0) {
            for (int i = 1; i <= 5; i ++) {
                if (i == 4) {
                    u8g2.setCursor(9 - 6 * frame / 4, 16 * i - 2 - 4 * frame);
                    u8g2.print(list[i + pageStart - 1]);
                } else if (i == 5) {
                    u8g2.setCursor(3 + 6 * frame / 4, 16 * i - 2 - 4 * frame);
                    u8g2.print(list[i + pageStart - 1]);
                } else {
                    u8g2.setCursor(3, 16 * i - 2 - 4 * frame);
                    u8g2.print(list[i + pageStart - 1]);
                }
            }
            u8g2.drawLine(3, 53, 6, 56);
            u8g2.drawLine(3, 60, 6, 57);
            u8g2.sendBuffer();
        } else {
            for (int i = 0; i <= 4; i ++) {
                if (i == 0) {
                    u8g2.setCursor(3 + 6 * frame / 4, 16 * i - 2 + 4 * frame);
                    u8g2.print(list[i + pageStart - 1]);
                } else if (i == 1) {
                    u8g2.setCursor(9 - 6 * frame / 4, 16 * i - 2 + 4 * frame);
                    u8g2.print(list[i + pageStart - 1]);
                } else {
                    u8g2.setCursor(3, 16 * i - 2 + 4 * frame);
                    u8g2.print(list[i + pageStart - 1]);
                }
            }
            u8g2.drawLine(3, 5, 6, 8);
            u8g2.drawLine(3, 12, 6, 9);
            u8g2.sendBuffer();
        }
        frame ++;
    }
}