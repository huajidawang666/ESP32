#include <Arduino.h>
#include <U8g2lib.h>
#include "system.h"

class tictactoeClass {
    public:
    tictactoeClass() {
        init();
    }

    void init() {
        memset(gameBoard, 0, sizeof(gameBoard));
        x = 2;
        y = 2;
        currentTurn = 1;
        isRunning = 0;
        msgStart = 0;
        finishSelect = 0;
        cellFilled = 0;
        isTied = 0;
    }

    void printBoardStructure() {
        u8g2.drawHLine(1, 18, 52);
        u8g2.drawVLine(18, 1, 52);
        u8g2.drawHLine(1, 36, 52);
        u8g2.drawVLine(36, 1, 52);
    }
    void printCell() {
        for (int i = 1; i <= 3; i++) {
            for (int j = 1; j <= 3; j++) {
                if (gameBoard[i][j] == 1) {
                    u8g2.drawLine(18 * i - 17, 18 * j - 17, 18 * i - 1, 18 * j - 1);
                    u8g2.drawLine(18 * i - 17, 18 * j - 1, 18 * i - 1, 18 * j - 17);
                } 
                if (gameBoard[i][j] == 2) {
                    u8g2.drawCircle(18 * i - 9, 18 * j - 9, 8);
                } 
            }
        }
    }
    void printSelect() {
        if (x != 4) u8g2.drawFrame(18 * x - 16, 18 * y - 16, 15, 15);
    }
    void readInput (){
        if (readButton(LEFT, 1)) {
            if (x > 1) x --;
            else x = 3;
        }
        if (readButton(RIGHT, 2)) {
            if (x < 4) x ++;
            else x = 1;
        }
        if (readButton(UP, 3)) {
            if (x != 4) if (y > 1) y --;
                        else y = 3;
        }
        if (readButton(DOWN, 4)) {
            if (x != 4) if (y < 3) y ++;
                        else y = 1;
        }
        if (readButton(ENTER, 5)) {
            if (x != 4) placeCell();
                        else finishMission = true;
        }
    }
    void placeCell() {
        if (gameBoard[x][y] == 0 && x != 4) {
            gameBoard[x][y] = currentTurn;
            currentTurn = 3 - currentTurn;
            cellFilled ++;
        }
    }
    bool checkWin(int player) {
        return ((gameBoard[1][1] == player && gameBoard[2][1] == player && gameBoard[3][1] == player) ||
                (gameBoard[1][1] == player && gameBoard[1][2] == player && gameBoard[1][3] == player) ||
                (gameBoard[1][2] == player && gameBoard[2][2] == player && gameBoard[3][2] == player) ||
                (gameBoard[2][1] == player && gameBoard[2][2] == player && gameBoard[2][3] == player) ||
                (gameBoard[1][3] == player && gameBoard[2][3] == player && gameBoard[3][3] == player) ||
                (gameBoard[3][1] == player && gameBoard[3][2] == player && gameBoard[3][3] == player) ||
                (gameBoard[1][1] == player && gameBoard[2][2] == player && gameBoard[3][3] == player) ||
                (gameBoard[1][3] == player && gameBoard[2][2] == player && gameBoard[3][1] == player));
    }
    void printStatus() {
        u8g2.setFont(u8g2_font_t0_11_tr);
        u8g2.setFontDirection(0);
        u8g2.setCursor(64, 10);
        u8g2.print("Current");
        if (currentTurn == 1) {
            u8g2.setCursor(64, 20);
            u8g2.print("Turn: X");
        } else {
            u8g2.setCursor(64, 20);
            u8g2.print("Turn: O");
        }
    
        u8g2.setCursor(64, 60);
        if (x != 4) { u8g2.print(" Exit ->");} else { u8g2.print("[Exit ->]");}
    }
    
    bool isRunning;
    bool isTied;
    int currentTurn;
    int msgStart;
    int finishSelect;
    int cellFilled;

    private:

    int x, y;
    int gameBoard[4][4];
};

void tictactoe_loop(tictactoeClass* tictactoeGame);

void tictactoe() {
    tictactoeClass* tictactoeGame = new tictactoeClass();
    u8g2.setContrast(brightness * 255 / 20);
    u8g2.setFontDirection(0);
    tictactoeGame->init();
    tictactoeGame->isRunning = 1;
    
    tictactoe_loop(tictactoeGame);
    
}

void tictactoe_loop(tictactoeClass* tictactoeGame) {

while (!finishMission) {

    u8g2.clearBuffer();
    tictactoeGame->printBoardStructure();
    tictactoeGame->printCell();

    if (tictactoeGame->isRunning == 1) {
        tictactoeGame->printSelect();
        tictactoeGame->printStatus();
        tictactoeGame->readInput();
        if (tictactoeGame->checkWin(1) || tictactoeGame->checkWin(2)) {
            tictactoeGame->isRunning = 0;
            tictactoeGame->msgStart = millis();
        } else if (tictactoeGame->cellFilled == 9) {
            tictactoeGame->isRunning = 0;
            tictactoeGame->isTied = 1;
            tictactoeGame->msgStart = millis();
        }
    } else {
        u8g2.setFont(u8g2_font_t0_11_tr);
        u8g2.setCursor(64, 20);
        if (tictactoeGame->isTied == 0) {
            //turn is changed before the game is finished
            if (tictactoeGame->currentTurn == 1 && millis() - tictactoeGame->msgStart <= 5000) {
                u8g2.print("O wins!");
            } else if (tictactoeGame->currentTurn == 2 && millis() - tictactoeGame->msgStart <= 5000) {
                u8g2.print("X wins!");
            }
        } else if (millis() - tictactoeGame->msgStart <= 5000){
            u8g2.print("Tied.");
        }

        u8g2.setCursor(64, 40);
        u8g2.print("Retry?");
        u8g2.setCursor(64, 50);
        u8g2.print(tictactoeGame->finishSelect ? "< Yes >" : "< No  >");
        if (readButton(ENTER, 5)) {
            if (tictactoeGame->finishSelect) {  
                tictactoeGame->init();
                tictactoeGame->isRunning = 1;
                // Serial.println("initialize");
            } else {
                finishMission = true;
                delete tictactoeGame;
                }
            }
            if (readButton(LEFT, 1) || readButton(RIGHT, 2)) {
            tictactoeGame->finishSelect = !tictactoeGame->finishSelect;
        }
    }
    u8g2.sendBuffer();
}}