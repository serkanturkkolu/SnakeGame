#ifndef SNAKEMAP_H
#define SNAKEMAP_H
#include <cstddef>
#include "dot.h"

#define DIRECTION_QUIT_CHAR 'q'


class Snake;

class SnakeMap
{
    struct Bait:public Dot{
        Bait(int x,int y):Dot(x,y){

        }
    };

public:
    SnakeMap();
    SnakeMap(int width,int height);
    void setSnake(Snake*);
    void setWidth(int width);
    void setHeight(int height);
    int getWidth()const;
    int getHeight()const;
    Bait getBait()const;
    void generateBait();
    void show();
    void clearTerminal();
private:
    Snake *snake;
    Bait bait;
    int width;
    int height;
};

#endif // GAMEMAP_H
