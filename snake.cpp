#include "snake.h"
#include <thread>
#include <chrono>
#include "snakemap.h"

Snake::Snake():dots({Dot(0,0)}),lastDirect(DirectionDown)
{

}
void Snake::setAddDotEventHandler(const std::function<void()>& handler){
    onAddDotSuccess = handler;
}

const std::function<void()>& Snake::getAddDotEventHandler(){
    return onAddDotSuccess;
}

void Snake::setMap(SnakeMap* gameMap){
    map=gameMap;
}

std::deque<Dot> Snake::getDots()const{
    return dots;
}

int Snake::getDotCount()const{
    return dots.size();
}
bool Snake::addDot(Snake::Direction direction){
    switch (direction) {
    case DirectionUp:
        if(dots.front().posY>0)
            dots.push_front(Dot(dots.front().posX,dots.front().posY-1));
        else
            return false;
        lastDirect=DirectionUp;
        break;
    case DirectionDown:
        if(dots.front().posY<map->getHeight())
            dots.push_front(Dot(dots.front().posX,dots.front().posY+1));
        else
            return false;
        lastDirect=DirectionDown;
        break;
    case DirectionLeft:
        if(dots.front().posX>0)
            dots.push_front(Dot(dots.front().posX-1,dots.front().posY));
        else
            return false;
        lastDirect=DirectionLeft;
        break;
    case DirectionRight:
        if(dots.front().posX<map->getWidth())
            dots.push_front(Dot(dots.front().posX+1,dots.front().posY));
        else
            return false;
        lastDirect=DirectionRight;
        break;
    }
    return true;

}

bool Snake::directTo(Snake::Direction direction){
    int samePoint=0;
    switch (direction) {
    case DirectionUp:
        if(lastDirect==DirectionDown)
            return false;
        else {
            dots.pop_back();
        }
        if(dots.front().posX==map->getBait().posX && dots.front().posY-1==map->getBait().posY){
            dots.emplace_front(map->getBait().posX, map->getBait().posY);
            map->generateBait();
        }
        dots.push_front(dots.front());
        dots.front().posY--;
        if(dots.front().posY<0)
            throw std::exception();

        lastDirect=DirectionUp;
        break;
    case DirectionDown:
        if(lastDirect==DirectionUp)
            return false;
        else {
            dots.pop_back();
        }
        if(dots.front().posX==map->getBait().posX && dots.front().posY+1==map->getBait().posY){
            dots.emplace_front(map->getBait().posX, map->getBait().posY);
            map->generateBait();
        }
        dots.push_front(dots.front());
        dots.front().posY++;
        if(dots.front().posY>map->getHeight()-1)
            throw std::exception();

        lastDirect=DirectionDown;
        break;
    case DirectionLeft:
        if(lastDirect==DirectionRight)
            return false;
        else {
            dots.pop_back();
        }
        if(dots.front().posX-1==map->getBait().posX && dots.front().posY==map->getBait().posY){
            dots.emplace_front(map->getBait().posX, map->getBait().posY);
            map->generateBait();
        }
        dots.push_front(dots.front());
        dots.front().posX--;
        if(dots.front().posX<0)
            throw std::exception();

        lastDirect=DirectionLeft;
        break;
    case DirectionRight:
        if(lastDirect==DirectionLeft)
            return false;
        else {
            dots.pop_back();
        }
        if(dots.front().posX+1==map->getBait().posX && dots.front().posY==map->getBait().posY){
            dots.emplace_front(map->getBait().posX, map->getBait().posY);
            map->generateBait();
        }
        dots.push_front(dots.front());
        dots.front().posX++;
        if(dots.front().posX>map->getWidth()-1)
            throw std::exception();

        lastDirect=DirectionRight;
        break;
    }

    /*START this block is for crash itself error.*/
    for(auto val:dots){
        if(dots.front().posX == val.posX && dots.front().posY == val.posY) {
            ++samePoint;
        }
    }
    if(samePoint>1){ // >1 because of itself
        throw std::exception();
    }
    /*END this block is for crash itself error.*/

    return true;
}

Snake::Direction Snake::getLastDirect(){
    return lastDirect;
}
