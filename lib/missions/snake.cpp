#include "system.h"
#include <queue>

#define displayWidth 128
#define displayHeight 64
#define gridSize 4
#define gridWidth ((displayWidth - 8) / gridSize) // 30
#define gridHeight ((displayHeight - 8) / gridSize) // 15
#define frameTop 0
#define frameBottom (displayHeight - 1)
#define frameLeft 0
#define frameRight (displayWidth - 1)
#define gameSurfaceTop (frameTop + 1) // 1
#define gameSurfaceBottom (frameBottom - 1) // 62
#define gameSurfaceLeft (frameLeft + 1) // 1
#define gameSurfaceRight (frameRight - 1) // 126

class snakeGameClass{
public:
    snakeGameClass(){
        init();
    }

    void init() {
        headX = gridWidth / 2;
        headY = gridHeight / 2;
        foodX = random(1, gridWidth);
        foodY = random(1, gridHeight);
        isRunning = true;
        direction = NONE;
        segments = {std::make_pair(headX, headY)};
        score = 0;
        finishSelect = false;
        count = 0;
    }

    void render() {
        drawframe();

        // draw food
        drawFood();

        // draw snake
        drawSnake();
    }

    void drawframe() {
        u8g2.drawFrame(gameSurfaceLeft,
                       gameSurfaceTop,
                       gameSurfaceRight - gameSurfaceLeft + 1,
                       gameSurfaceBottom - gameSurfaceTop + 1);
    }

    void drawSnake() {
        for (auto& segment : segments) {
            u8g2.drawBox(segment.first * gridSize, segment.second * gridSize + 1, 4, 2);
            u8g2.drawBox(segment.first * gridSize + 1, segment.second * gridSize, 2, 4);
        }
    }

    void drawFood() {
        u8g2.drawBox(foodX * gridSize, foodY * gridSize, 4, 4);
    }
    
    void update() {
        if (millis() - lastmove > tickspeed) {
            lastmove = millis();
            if (direction == UPWARD) {
                headY --;
            } else if (direction == DOWNWARD) {
                headY ++;
            } else if (direction == LEFTWARD) {
                headX --;
            } else if (direction == RIGHTWARD) {
                headX ++;
            }
            if (headY < 1) {
                headY = gridHeight;
            } else if (headY > gridHeight) {
                headY = 1;
            } else if (headX < 1) {
                headX = gridWidth;
            } else if (headX > gridWidth) {
                headX = 1;
            }

            segments.push_back(std::make_pair(headX, headY));
            if (headX == foodX && headY == foodY) {
                count ++;
                foodX = random(1, gridWidth);
                foodY = random(1, gridHeight);
            } else {
                segments.erase(segments.begin());
            }
        }
    }

    void readInput() {
        if (readButton(ENTER, 5)) {
            finishMission = true;
        } 
        if (readButton(UP, 3)) {
            if (direction != DOWNWARD) {
                direction = UPWARD;
                lastmove = millis();
            }
        }
        if (readButton(DOWN, 4)) {
            if (direction != UPWARD) {
                direction = DOWNWARD;
                lastmove = millis();
            }
        }
        if (readButton(LEFT, 1)) {
            if (direction != RIGHTWARD) {
                direction = LEFTWARD;
                lastmove = millis();
            }
        }
        if (readButton(RIGHT, 2)) {
            if (direction != LEFTWARD) {
                direction = RIGHTWARD;
                lastmove = millis();
            }
        }
    }

    void check() {
        std::vector<std::pair<int, int>> subvector(segments.begin(), segments.end() - 1);
        for (auto& segment : subvector) {
            if (segment.first == headX && segment.second == headY) {
                score = 20.0 * pow( 10.0, pow(3.0, count / 100.0)) - 200.0;
                isRunning = false;
            }
        }
    }

    bool isRunning;
    float score;
    bool finishSelect;

private:
    enum Direction {NONE, UPWARD, DOWNWARD, LEFTWARD, RIGHTWARD};
    int count;
    const int tickspeed = 500; // miliseconds per tick
    int headX, headY;
    Direction direction;
    std::vector<std::pair<int, int>> segments;
    int foodX, foodY;
    long long lastmove = 0;
};

void snake() {
    finishMission = false;
    snakeGameClass* snakeGame = new snakeGameClass();
    u8g2.setContrast(brightness * 255 / 20);
    u8g2.setFont(u8g2_font_t0_11_tr);
    u8g2.setFontDirection(0);
    snakeGame->init();

    
    while (!finishMission){
        if (!snakeGame->isRunning) {
            u8g2.clearBuffer();
            u8g2.drawStr(0, 20, "Game Over");
            u8g2.drawStr(0, 30, "Score: ");
            u8g2.setCursor(40, 30);
            u8g2.print(snakeGame->score);
            u8g2.setCursor(0, 40);
            u8g2.print("Retry?");
            if (readButton (LEFT, 1) || readButton(RIGHT, 2)) {
                snakeGame->finishSelect = !snakeGame->finishSelect;
            }
            if (readButton(ENTER, 5)) {
                if (snakeGame->finishSelect) {
                    snakeGame->init();
                } else {
                    finishMission = true;
                }
            }
            u8g2.drawStr(0, 50,(snakeGame->finishSelect) ? "< Yes >" : "< No  >");
            u8g2.sendBuffer();
        } else {
            u8g2.clearBuffer();
            snakeGame->readInput();
            snakeGame->render();
            snakeGame->update();
            snakeGame->check();
            u8g2.sendBuffer();
        }
    }

    delete snakeGame;
}