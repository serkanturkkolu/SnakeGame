#include "snakemap.h"
#include <random>
#include "snake.h"

SnakeMap::SnakeMap():/**/bait(0,0),width{10},height{10}
{

}


SnakeMap::SnakeMap(int pWidth,int pHeight):/**/bait(0,0),width{pWidth},height{pHeight}
{

}

void SnakeMap::setSnake(Snake *snake){
    this->snake=snake;
}


void SnakeMap::setWidth(int pWidth){
    width=pWidth;
}
void SnakeMap::setHeight(int pHeight){
    height=pHeight;
}

int SnakeMap::getWidth()const{
    return width;
}

int SnakeMap::getHeight()const{
    return height;
}

SnakeMap::Bait SnakeMap::getBait()const{
    return bait;
}

void SnakeMap::generateBait(){
    std::random_device randomDevice;
    std::mt19937 rand(randomDevice());
    std::uniform_int_distribution<int> uidWidth(0,width-1);
    std::uniform_int_distribution<int> uidHeight(0,height-1);
    int baitX=-1,baitY=-1;
    newBait:
    baitX=uidWidth(rand);
    baitY=uidHeight(rand);
    for(auto val : snake->getDots() ){
        if(val.posX == baitX && val.posY == baitY){
            goto newBait;
        }
    }
    bait.posX=baitX;
    bait.posY=baitY;

    if(snake->getAddDotEventHandler()!=nullptr)
        snake->getAddDotEventHandler()();
}


void SnakeMap::show(){ //this function can change according to displaying method
    std::system("clear");
    bool flag=false;
    for(int j=0;j<height;++j){
        for(int i=0;i<width;++i){
            for(auto val:snake->getDots()){
                if((val.posX==i && val.posY==j)){
                    std::cout << "*";
                    flag=true;
                    goto findDot;
                }else{
                    flag=false;
                }
            }
            if((bait.posX==i && bait.posY==j)){
                std::cout << "*";
                flag=true;
            }else{
                flag=false;
            }
            findDot:
            if(flag==false){
                std::cout << " ";
            }
        }
        std::cout << "|\n";//For frame right line
    }

    //For frame below line
    for(int i=0;i<width;++i){
       std::cout << "_";
    }
    //For frame below line

    std::cout << "|";//For frame right last line
    std::cout << std::flush;
}
