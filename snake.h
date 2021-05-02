#ifndef SNAKE_H
#define SNAKE_H
#include<iostream>
#include <deque>
#include "dot.h"
#include <functional>

class SnakeMap;

class Snake
{
public:
    enum Direction:char{
        DirectionRight='d',
        DirectionLeft='a',
        DirectionUp='w',
        DirectionDown='s',
    };
    Snake();
    bool addDot(Direction);
    bool directTo(Direction);
    void setMap(SnakeMap*);
    std::deque<Dot> getDots()const;
    int getDotCount()const;
    Direction getLastDirect();
    void setAddDotEventHandler(const std::function<void()>&);
    const std::function<void()>& getAddDotEventHandler();
private:
    std::deque<Dot> dots;
    int snakeSpeedDelta;
    SnakeMap* map;
    size_t dotLen;
    Direction lastDirect;
    std::function<void()> onAddDotSuccess;
};

#endif // SNAKE_H
