#include <Arduino.h>
#include <U8g2lib.h>
#include "system.h"

int row, col; // row <= 9, col <= 18
int nuclearChargeNum[10][19] = {{0},
                                {0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
                                {0, 3, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 6, 7, 8, 9,10},
                                {0,11,12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,13,14,15,16,17,18},
                                {0,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36},
                                {0,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54},
                                {0,55,56,57,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86},
                                {0,87,88,89,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118},
                                {0,0,0,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,0},
                                {0,0,0,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,0}
                                };
String elementName[119] = {"","H","He",
                          "Li","Be","B","C","N","O","F","Ne",
                          "Na","Mg","Al","Si","P","S","Cl","Ar",
                          "K","Ca","Sc","Ti","V","Cr","Mn","Fe","Ni","Co","Cu","Zn","Ga","Ge","As","Se","Br","Kr",
                          "Rb","Sr","Y","Zr","Nb","Mo","Tc","Ru","Rh","Pd","Ag","Cd","In","Sn","Sb","Te","I","Xe",
                          "Cs","Ba","La","Ce","Pr","Nd","Pm","Sm","Eu","Gd","Tb","Dy","Ho","Er","Tm","Yb","Lu","Hf","Ta","W","Re","Os","Ir","Pt","Au","Hg","Tl","Pb","Bi","Po","At","Rn",
                          "Fr","Ra","Ac","Th","Pa","U","Np","Pu","Am","Cm","Bk","Cf","Es","Fm","Md","No","Lr","Rf","Db","Sg","Bh","Hs","Mt","Ds","Rg","Cn","Nh","Fl","Mc","Lv","Ts","Og"};
float relativeAtomicMass[119] = {0,1.00,4.00,
                              6.94,9.01,10.81,12.01,14.01,16.00,19.00,20.18,
                              22.99,24.31,26.98,28.09,30.97,32.07,35.45,39.95,
                              39.10,40.08,44.96,47.87,50.94,52.00,54.94,55.85,58.93,58.69,63.55,65.38,69.72,72.63,74.92,78.96,79.90,83.80,
                              85.47,87.62,88.91,91.22,92.91,95.94,98.00,101.07,102.91,106.42,107.87,112.41,114.82,118.71,121.76,127.60,126.90,131.29,
                              132.91,137.33,138.91,140.12,140.91,144.24,145.00,150.36,151.96,157.25,158.93,162.50,164.93,167.26,168.93,173.05,174.97,178.49,180.95,183.84,186.21,190.23,192.22,195.08,196.97,200.59,204.38,207.20,208.98,209.00,210.00,222.00,
                              223.00,226.00,227.00,232.04,231.04,238.03,237.05,244.00,243.00,247.00,247.00,251.00,252.00,257.00,258.00,259.00,262.00,267.00,268.00,269.00,270.00,269.00,278.00,281.00,282.00,285.00,286.00,289.00,289.00,293.00,294.00,294.00};

void drawElement();
void chemistrytable_readInput();
void drawTable();
void drawSelect();

void chemistrytable_init() {
    row = 1;
    col = 1;
    u8g2.setContrast(brightness * 255 / 20);

}

void chemistrytable_loop() {
    u8g2.clearBuffer();
    drawTable();
    drawSelect();
    chemistrytable_readInput();
    drawElement();
    u8g2.sendBuffer();
}

void drawTable() {
    for (int i = 1; i <= 9; i ++) {
        if (i == 1) {
            u8g2.drawFrame(1, 1, 6, 6);
            u8g2.drawFrame(86, 1, 6, 6);
        } else if (i <= 3) {
            u8g2.drawFrame(1, 5 * i - 4, 6, 6);
            u8g2.drawFrame(6, 5 * i - 4, 6, 6);
            for (int j = 13; j <= 18; j++) {
                u8g2.drawFrame(j * 5 - 4, 5 * i - 4, 6, 6);
            }
        } else if (i <= 7) {
            for (int j = 1; j <= 18; j++) {
                u8g2.drawFrame(j * 5 - 4, 5 * i - 4, 6, 6);
            }
        } else {
            for (int j = 3; j <= 17; j++) {
                u8g2.drawFrame(j * 5 - 4, 5 * i + 1, 6, 6);
            }
        }
    }
}

void chemistrytable_readInput() {
    if (readButton(UP, 3) && ((row > 1 && (col == 1 || col == 18)) || (row > 2 && (col == 2 || (col >= 13 && col <= 17))) || (row > 4 && (col >= 3 && col <= 12)))) {
        row --;
    }
    if (readButton(DOWN, 4) && ((row < 7 && (col == 18 || col == 1 || col == 2)) || (row < 9 && (col >= 3 && col <= 17)))) {
        row ++;
    }
    if (readButton(LEFT, 1) && ((col > 1 && row <= 7) || (col > 3 && row > 7))) {
        if (col == 13 && (row == 3 || row == 2)) col = 2;
        else if (col == 18 && row == 1) col = 1;
        else col --;
    }
    if (readButton(RIGHT, 2) && ((col < 18 && row <= 7) || (col < 17 && row > 7))) {
        if (col == 1 && row == 1) col = 18;
        else if (col == 2 && (row == 2 || row == 3)) col = 13;
        else col ++;
    }
    if (readButton(ENTER, 5)) {
        finishMission = true;
    }
}

void drawSelect() {
    if (row > 7) u8g2.drawBox(col * 5 - 4, row * 5 + 1, 6, 6);
    else u8g2.drawBox(col * 5 - 4, row * 5 - 4, 6, 6);
}

void drawElement() {
    u8g2.setFont(u8g2_font_logisoso24_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(93, 36);
    u8g2.print(elementName[nuclearChargeNum[row][col]]);
    u8g2.setFont(u8g2_font_t0_11_tr);
    u8g2.setCursor(93, 10);
    u8g2.print(nuclearChargeNum[row][col]);
    u8g2.setCursor(93, 48);
    u8g2.print(relativeAtomicMass[nuclearChargeNum[row][col]]);
}