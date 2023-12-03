#include <Arduino.h>
#include <U8g2lib.h>
#include "system.h"

int gameBoard[10][10] = {0};
int subBoard[4][4] = {0};
int currentSelectx = 5, currentSelecty = 5;
int currentTurn = 1; //1 stands for 'x' while 2 stands for 'o'
int currentSubBoardIndexx = 0, currentSubBoardIndexy = 0;

int finishSelect = 0; //0 stands for finish while 1 stands for restart

bool isRunning = false;

bool isDisplaying = false;
int timeDisplayStart = 0;
String msg;

void printBoardStructure();
void printBoard();
void printCell(int label, int x, int y);
void printStatus();
void printSelect();

void utt_readInput();
void placeCell();
int getSubCell(int boardIndex, int relativeX, int relativeY);
void checkSubBoard(int boardIndex, int player);
bool checkWin(int player);
void Message();

void game_init();

void utt_readInput();
void placeCell();

// put function declarations here:

void utt_init() {
  isRunning = true;
  game_init();
  
  // gameBoard[5][4] = 2;
  // gameBoard[5][6] = 2;
  // gameBoard[2][1] = 2;
  // gameBoard[2][2] = 2;
  // gameBoard[2][3] = 2;
  // gameBoard[2][7] = 2;
  // gameBoard[2][8] = 2;
  // gameBoard[2][9] = 2;
  // gameBoard[2][4] = 2;
  // gameBoard[2][6] = 2;
  // put your setup code here, to run once:
  // Serial.begin(9600);
  u8g2.setContrast(brightness * 255 / 20);
}

void utt_loop() {
  // put your main code here, to run repeatedly:  
  
  // Serial.print(currentSelectx);
  // Serial.print(currentSelecty);
  // Serial.print(currentSubBoardIndexx);
  // Serial.println(currentSubBoardIndexy);

  u8g2.clearBuffer();
  printBoardStructure();
  printBoard();
  if (currentSelectx != 10) { printSelect();}
  printStatus();
  Message();

  // Serial.println(isRunning);
  if (isRunning) {
    utt_readInput();
    for (int index = 1; index <= 9; index++) {
      checkSubBoard(index, 1);
      checkSubBoard(index, 2);
    }
    if (checkWin(1)) {
      isRunning = false;
      msg = "X Wins!";
      isDisplaying = true;
      timeDisplayStart = millis();
    }
    if (checkWin(2)) {
      isRunning = false;
      msg = "O Wins!";
      isDisplaying = true;
      timeDisplayStart = millis();
    }
  } else {
    u8g2.setFont(u8g2_font_t0_11_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(64, 40);
    u8g2.print("Retry?");
    u8g2.setCursor(64, 50);
    u8g2.print(finishSelect ? "< Yes >" : "< No  >");
    if (readButton(ENTER, 5)) {
      if (finishSelect) {  
        game_init();
        // Serial.println("initialize");
        isRunning = true;
        msg = "";
      } else {
        finishMission = true;
      }
    }
    if (readButton(LEFT, 1) || readButton(RIGHT, 2)) {
      finishSelect = !finishSelect;
    }
  }

  u8g2.sendBuffer();
  
}

// put function definitions here:

void printBoardStructure() {
  for (int i = 1; i <= 8; i++) {
    if (i % 3 == 0) {
      u8g2.drawVLine(i * 6, 0, 53);
      u8g2.drawHLine(0, i * 6, 53);
    } else {
      for (int j = 1; j <= 26; j++) {
        u8g2.drawPixel(i * 6, j * 2);
        u8g2.drawPixel(j * 2, i * 6);
      }
    }
  }
}

void printBoard() {
  for (int i = 1; i <= 9; i++) {
    for (int j = 1; j <= 9; j++) {
      if (subBoard[(i - 1) / 3 + 1][(j - 1) / 3 + 1] == 0) {
        printCell(gameBoard[i][j], 6 * i - 4, 6 * j - 4);
      } 
    }
  }

  for (int i = 1; i <= 3; i++) {
    for (int j = 1; j <= 3; j++) {
      if (subBoard[i][j] == 1) {
        u8g2.drawLine(18 * i - 17, 18 * j - 17, 18 * i - 1, 18 * j - 1);
        u8g2.drawLine(18 * i - 17, 18 * j - 1, 18 * i - 1, 18 * j - 17);
      } 
      if (subBoard[i][j] == 2) {
        u8g2.drawCircle(18 * i - 9, 18 * j - 9, 8);
      } 
    }
  }
}

void printCell(int label, int x, int y) {
  // label = 1 stands for 'x' while label = 2 stands for 'o'
  if (label == 1) {
    u8g2.drawLine(x - 1, y - 1, x + 3, y + 3);
    u8g2.drawLine(x + 3, y - 1, x - 1, y + 3);
  } else if (label == 2) {
    u8g2.drawFrame(x, y, 3, 3);
  } else {
    return;
  }
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
  if (currentSelectx != 10) { u8g2.print(" Exit ->");} else { u8g2.print("[Exit ->]");}
}

void printSelect() {
  u8g2.drawFrame(6 * currentSelectx - 5, 6 * currentSelecty - 5, 5, 5);
}



extern int buttonStatus[6];

bool readButton(int pin, int direction) {
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

void utt_readInput() {
  if (readButton(LEFT, 1)) {
    currentSelectx--;
    if (currentSelectx == 0) {
      currentSelectx = 9;
    }
  }
  if (readButton(RIGHT, 2)) {
    currentSelectx++;
    if (currentSelectx == 11) {//10 stands for exit
      currentSelectx = 1;
    }
  }
  if (readButton(UP, 3) && currentSelectx != 10) {
    currentSelecty--;
    if (currentSelecty == 0) {
      currentSelecty = 9;
    }
  }
  if (readButton(DOWN, 4) && currentSelectx != 10) {
    currentSelecty++;
    if (currentSelecty == 10) {
      currentSelecty = 1;
    }
  }
  if (readButton(ENTER, 5)) {
    if(currentSelectx != 10) { placeCell();} else {
      finishMission = true;
    }
    
  }
}

void placeCell() {
  if (gameBoard[currentSelectx][currentSelecty] == 0 &&
      subBoard[(currentSelectx - 1) / 3 + 1][(currentSelecty - 1) / 3 + 1] == 0 &&
      ((currentSelectx - 1) / 3 + 1 == currentSubBoardIndexx || currentSubBoardIndexx == 0) &&
      ((currentSelecty - 1) / 3 + 1 == currentSubBoardIndexy || currentSubBoardIndexy == 0) &&
      currentSelectx != 10) {
    gameBoard[currentSelectx][currentSelecty] = currentTurn;
    
    if (currentTurn == 1) {
      currentTurn = 2;
    } else {
      currentTurn = 1;
    }

    currentSubBoardIndexx = (currentSelectx - 1) % 3 + 1;
    currentSubBoardIndexy = (currentSelecty - 1) % 3 + 1;
    if ((currentSubBoardIndexx == 0 && currentSubBoardIndexy == 0) || subBoard[currentSubBoardIndexx][currentSubBoardIndexy] != 0) {
      currentSubBoardIndexx = 0;
      currentSubBoardIndexy = 0;
    }
  }
}

int getSubCell (int boardIndex, int relativeX, int relativeY) {
  return gameBoard[(boardIndex - 1) % 3 * 3 + relativeX + 1][(boardIndex - 1) / 3 * 3 + relativeY + 1];
}

void checkSubBoard(int boardIndex, int player) {
  if (((getSubCell(boardIndex, 0 , 0) == player && getSubCell(boardIndex, 1 , 0) == player && getSubCell(boardIndex, 2 , 0) == player) ||
       (getSubCell(boardIndex, 0 , 0) == player && getSubCell(boardIndex, 0 , 1) == player && getSubCell(boardIndex, 0 , 2) == player) ||
       (getSubCell(boardIndex, 0 , 1) == player && getSubCell(boardIndex, 1 , 1) == player && getSubCell(boardIndex, 2 , 1) == player) ||
       (getSubCell(boardIndex, 1 , 0) == player && getSubCell(boardIndex, 1 , 1) == player && getSubCell(boardIndex, 1 , 2) == player) ||
       (getSubCell(boardIndex, 0 , 2) == player && getSubCell(boardIndex, 1 , 2) == player && getSubCell(boardIndex, 2 , 2) == player) ||
       (getSubCell(boardIndex, 2 , 0) == player && getSubCell(boardIndex, 2 , 1) == player && getSubCell(boardIndex, 2 , 2) == player) ||
       (getSubCell(boardIndex, 0 , 0) == player && getSubCell(boardIndex, 1 , 1) == player && getSubCell(boardIndex, 2 , 2) == player) ||
       (getSubCell(boardIndex, 0 , 2) == player && getSubCell(boardIndex, 1 , 1) == player && getSubCell(boardIndex, 2 , 0) == player)) &&
       subBoard[(boardIndex - 1) % 3 + 1][(boardIndex - 1) / 3 + 1] == 0) {
    subBoard[(boardIndex - 1) % 3 + 1][(boardIndex - 1) / 3 + 1] = player;
    currentSubBoardIndexx = 0;
    currentSubBoardIndexy = 0;
  }
}

bool checkWin(int player) {
  return ((subBoard[1][1] == player && subBoard[2][1] == player && subBoard[3][1] == player) ||
        (subBoard[1][1] == player && subBoard[1][2] == player && subBoard[1][3] == player) ||
        (subBoard[1][2] == player && subBoard[2][2] == player && subBoard[3][2] == player) ||
        (subBoard[2][1] == player && subBoard[2][2] == player && subBoard[2][3] == player) ||
        (subBoard[1][3] == player && subBoard[2][3] == player && subBoard[3][3] == player) ||
        (subBoard[3][1] == player && subBoard[3][2] == player && subBoard[3][3] == player) ||
        (subBoard[1][1] == player && subBoard[2][2] == player && subBoard[3][3] == player) ||
        (subBoard[1][3] == player && subBoard[2][2] == player && subBoard[3][1] == player));
}

void Message() {
  if (isDisplaying == true && millis() - timeDisplayStart < 5000) {
    u8g2.setFont(u8g2_font_t0_11_tr);
    u8g2.setFontDirection(0);
    u8g2.setCursor(64, 30);
    u8g2.print(msg);
  } else {
    isDisplaying = false;
  }
}

void game_init() {
  currentSelectx = 5, currentSelecty = 5;
  currentTurn = 1;
  currentSubBoardIndexx = 0, currentSubBoardIndexy = 0;
  
  memset(gameBoard, 0, sizeof(gameBoard));
  memset(subBoard, 0, sizeof(subBoard));
}