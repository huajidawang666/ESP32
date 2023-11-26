#include <Arduino.h>
#include <U8g2lib.h>
#include "system.h"
#include "settings.h"
#define LEFT 12
#define RIGHT 14
#define UP 27
#define DOWN 26
#define ENTER 25
int temperaryTime = 0;
int temperaryHr = 0, temperaryMin = 0;
String temperaryTimenow;
bool timeIndex = 0; // 0 - hr, 1 - min

void timeanddate_readInput();

void timeanddate_init() {
    u8g2.enableUTF8Print();
    u8g2.setFont(u8g2_font_7x14_tr);
    u8g2.setFontDirection(0);
    u8g2.setContrast(brightness * 255 / 20);
    temperaryTime = millis() + realTime;
    temperaryHr = (temperaryTime / 1000 / 60 / 60) % 24;
    temperaryMin = (temperaryTime / 1000 / 60) % 60;
    temperaryTimenow = String(temperaryHr < 10 ? "0" + String(temperaryHr) : String(temperaryHr)) + ":" + String(temperaryMin < 10 ? "0" + String(temperaryMin) : String(temperaryMin));
}

void timeanddate_loop() {
    u8g2.clearBuffer();
    u8g2.setCursor(0, 16);
    u8g2.print("    Time & Date   ");
    u8g2.setCursor(44, 40);
    u8g2.print(temperaryTimenow);
    u8g2.drawFrame(42 + timeIndex * 21, 28, 17, 14);
    u8g2.setCursor(0, 62);
    u8g2.print("       [OK]       ");
    timeanddate_readInput();
    u8g2.sendBuffer();
}

void timeanddate_readInput() {
    if (readButton(ENTER, 5)) {
        finishSettingMission = true;
        realTime = temperaryTime - millis();
        realTime += (temperaryHr - (temperaryTime / 1000 / 60 / 60) % 24) * 1000 * 60 * 60 + (temperaryMin - (temperaryTime / 1000 / 60) % 60) * 1000 * 60;
    } else if (readButton(LEFT, 1) || readButton(RIGHT, 2)) {
        timeIndex = !timeIndex;
    } else if (readButton(UP, 3)) {
        if (timeIndex == 0) {
            temperaryHr = (temperaryHr + 1) % 24;
        } else {
            temperaryMin = (temperaryMin + 1) % 60;
        }
        temperaryTimenow = String(temperaryHr < 10 ? "0" + String(temperaryHr) : String(temperaryHr)) + ":" + String(temperaryMin < 10 ? "0" + String(temperaryMin) : String(temperaryMin));
    } else if (readButton(DOWN, 4)) {
        if (timeIndex == 0) {
            temperaryHr = (temperaryHr + 23) % 24;
        } else {
            temperaryMin = (temperaryMin + 59) % 60;
        }
        temperaryTimenow = String(temperaryHr < 10 ? "0" + String(temperaryHr) : String(temperaryHr)) + ":" + String(temperaryMin < 10 ? "0" + String(temperaryMin) : String(temperaryMin));
    }
}