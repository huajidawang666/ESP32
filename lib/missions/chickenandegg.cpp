#include "system.h"

class ChickenAndEggClass {
public:
    ChickenAndEggClass() {
        init();
    }

    void init() {
        x = 64; y = 32;
        eggNum = 0;
    }

    void readInput() {
        if (readButton(DOWN, 4) && eggNum < 1000) {
            place();
        }
        if (readButton(ENTER, 5)) {
            finishMission = true;
        }
    }

    void place() {
        eggNum ++;
        // TODO: place egg
        x = random(0, 128); eggX[eggNum] = x;
        y = random(0, 64); eggY[eggNum] = y;
    }

    void print() {
        for (int i = 1; i <= eggNum; i ++) {
            u8g2.drawXBMP(eggX[i] - 4, eggY[i] - 4, 8, 8, gImage_egg);
        }
        u8g2.drawXBMP(x - 6, y - 6, 12, 12, gImage_chicken);
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 55, std::to_string(eggNum).length() * 6, 9);
        u8g2.setDrawColor(1);
        u8g2.setCursor(0, 64);
        u8g2.print(eggNum);
    }

private:
    short x, y;
    short eggNum;
    short eggX[1001], eggY[1001];
    // do not change the following code
    const unsigned char gImage_chicken[32] = {0xA0,0x02,0xE0,0x03,0x10,0x02,0x08,0x04,0x2E,0x08,0x0B,0x08,0x0C,0x08,0x08,0x04,0x10,0x0E,0xE0,0x0B,0x20,0x02,0x70,0x07};
    const unsigned char gImage_egg[8] = {0x18,0x24,0x42,0x42,0x81,0x81,0x42,0x3C};
};

void chickenAndEgg() {
    u8g2.setFont(u8g2_font_t0_11_tr);
    u8g2.setFontDirection(0);
    ChickenAndEggClass* chickenAndEgg = new ChickenAndEggClass();
    while (!finishMission) {
        u8g2.clearBuffer();
        chickenAndEgg->readInput();
        chickenAndEgg->print();
        u8g2.sendBuffer();
    }
    delete chickenAndEgg;
}