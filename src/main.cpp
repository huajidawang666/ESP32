/*
|--lib
|  |
|  |--utt
|  |  |- utt.c
|  |  |- utt.h
|  |
|  |- README
|
|- platformio.ini
|--src
    |- main.cpp --> THIS FILE
*/

#include <Arduino.h>
#include <U8g2lib.h>
#include "missions.h"
#define LIST_AMOUNT 10
#define LEFT 12
#define RIGHT 14
#define UP 27
#define DOWN 26
#define ENTER 25

String list[LIST_AMOUNT + 1] = {"", "Ultimate TicTacToe", "Info", "Test3", "Test4", "Test5", "Test6", "Test7", "Test8", "Test9", "Test10"};
int pageStart = 1;
int cursor = 1;
int showcaseIndex = 1;
int startDisplayTime = 0;
int currentTime = 0;

void home_readInput();
void startMission(int index);

bool finishMission = true;

U8G2_SSD1306_128X64_NONAME_F_4W_SW_SPI u8g2(U8G2_R0, /* clock=*/18, /* data=*/13, 
                                             /* cs=*/4, /* dc=*/2, /* reset=*/15);
int buttonStatus[6] = {0};

void setup() {
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_7x14_tr);
  u8g2.setFontDirection(0);
  pinMode(12, INPUT_PULLDOWN);
  pinMode(14, INPUT_PULLDOWN);
  pinMode(27, INPUT_PULLDOWN);
  pinMode(26, INPUT_PULLDOWN);
  pinMode(25, INPUT_PULLDOWN);
  startDisplayTime = millis();
}

void loop() {
  u8g2.setFont(u8g2_font_7x14_tr);
  u8g2.setFontDirection(0);
  currentTime = millis();
  u8g2.clearBuffer();
  home_readInput();
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
    if (readButton(UP, 3)) {
        if (cursor == pageStart && pageStart > 1) {
            pageStart--;
            cursor--;
            showcaseIndex = 0;
        } else if (cursor > pageStart) {
            cursor--;
            showcaseIndex = 0;
        }
    }
    if (readButton(DOWN, 4)) {
        if (cursor == pageStart + 3 && pageStart + 3 < LIST_AMOUNT) {
            pageStart++;
            cursor++;
            showcaseIndex = 0;
        } else if (cursor < pageStart + 3) {
            cursor++;
            showcaseIndex = 0;
        }
    }
    if (readButton(ENTER, 5)) {
        startMission(cursor);
    }
}

void startMission(int index) {
    if (index == 1) {
        utt_init();
        finishMission = false;
        while (!finishMission) {
            utt_loop();
        }
    }
    if (index == 2) {
        finishMission = false;
        while (!finishMission) {
            info_loop();
        }
    }
}