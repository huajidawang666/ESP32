#include <Arduino.h>
#include <U8g2lib.h>
#include "system.h"
#include "settings/settings.h"
#define SETTINGS_AMOUNT 10

bool finishSettingMission = true;

int settings_cursor = 1;
int settings_pageStart = 1;
int settings_displayline;


String settings_list[SETTINGS_AMOUNT + 1] = {
                                    "|-No more byd this.-|",
                                    " Time & Date      -> ",
                                    " Brightness       -> ",
                                    " WIFI             -> ",
                                    " Test2               ",
                                    " Test3               ",
                                    " Test4               ",
                                    "",
                                    "",
                                    "",
                                    ""
                                    };

void settings_readInput();


void settings_init() {
    int settings_cursor = 1;
    int settings_pageStart = 1;
    settings_displayline = SETTINGS_AMOUNT + 1;
    while (settings_list[settings_displayline - 1] == "") {
        settings_displayline --;
    }
    settings_displayline --;
    u8g2.setContrast(brightness * 255 / 20);
}

void settings_loop() {
  u8g2.setFont(u8g2_font_t0_11_tr);
  u8g2.setFontDirection(0);
  u8g2.clearBuffer();
  u8g2.setCursor(3, 10);
  u8g2.print("<-Back Settings      ");
  settings_readInput();
  for (int i = 1; i <= 5; i ++) {
    if (settings_cursor == i + settings_pageStart - 1) {
        String showSelect = settings_list[i + settings_pageStart - 1];
        showSelect[0] = '[';
        showSelect[20] = ']';
        u8g2.setCursor(3, 10 + 10 * i);
        u8g2.print(showSelect);
    } else {
        u8g2.setCursor(3, 10 + 10 * i);
        u8g2.print(settings_list[i + settings_pageStart - 1]);
    }
  }
  u8g2.sendBuffer();
}

void settings_readInput() {
    if (readButton(UP, 3)) {
        if (settings_cursor == settings_pageStart && settings_pageStart > 1) {
            settings_pageStart--;
            settings_cursor--;
        } else if (settings_cursor > settings_pageStart) {
            settings_cursor--;
        }
    }
    if (readButton(DOWN, 4)) {
        if (settings_cursor == settings_pageStart + 4 && settings_pageStart + 4 < settings_displayline) {
            settings_pageStart++;
            settings_cursor++;
        } else if (settings_cursor < settings_pageStart + 4) {
            settings_cursor++;
        }
    }
    if (readButton(LEFT, 1)) {
        finishMission = true;
    }
    if (readButton(ENTER, 5)) {
        if (settings_cursor == 1) {
            finishSettingMission = false;
            timeanddate_init();
            while (!finishSettingMission) {
                timeanddate_loop();
            }
        } else if (settings_cursor == 2) {
            finishSettingMission = false;
            brightness_init();
            while (!finishSettingMission) {
                brightness_loop();
            }
        } else if (settings_cursor == 3) {
            finishSettingMission = false;
            wifi();
        }
    }
}