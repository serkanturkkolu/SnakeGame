#ifndef SNAKEGAME_H
#define SNAKEGAME_H
#include "snake.h"
#include "snakemap.h"

class SnakeGame
{
public:
    SnakeGame();
    void begin();
private:
    void addDotEventHandler(void);
    static const int SNAKE_SPEED_BASE_PERIOD;
    static const int SNAKE_SPEED_BASE_DELTA;
    static const char EXIT_QUESTION[];
    static const char SAY_GOOD_BYE[];
    static const char SAY_GAME_OVER[];
    void directionThread(Snake::Direction direct);
    bool killDirectionThread(char);
    bool exitSignal;
    Snake::Direction direction;
    SnakeMap map;
    Snake snake;
    bool restart;
    bool exceptionFlag;
    int snakeSpeedDelta;
};

#endif // SNAKEGAME_H
