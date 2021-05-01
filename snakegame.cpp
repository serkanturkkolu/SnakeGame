#include "snakegame.h"
#include <iostream>
#include <thread>
#include "snake.h"
#include "snakemap.h"

const int SnakeGame::SNAKE_SPEED_BASE_PERIOD=300;
const int SnakeGame::SNAKE_SPEED_BASE_DELTA=5;

SnakeGame::SnakeGame():exitSignal(false),map(SnakeMap{40,20}),restart(false),exceptionFlag(false),snakeSpeedDelta(0)
{
    system("stty -echo -icanon"); //stty echo icanon
    system("setterm -cursor off");
    map.setSnake(&snake);
    snake.setMap(&map);
    snake.setAddDotEventHandler(std::bind(&SnakeGame::addDotEventHandler,this));
    map.generateBait();
}

void SnakeGame::addDotEventHandler(void){
    if(snakeSpeedDelta<SNAKE_SPEED_BASE_PERIOD-SNAKE_SPEED_BASE_DELTA)
    snakeSpeedDelta+=SNAKE_SPEED_BASE_DELTA;
}

bool SnakeGame::killDirectionThread(char input){
//This function prevent to snake would speed up. If this function wouldn't be , whichever button the user pressed, the thread would be killed and the snake would speed up.

    Snake::Direction oppositeDirection;
    switch (input) {
        case Snake::DirectionUp:
            oppositeDirection=Snake::DirectionDown;
        break;
        case Snake::DirectionDown:
            oppositeDirection=Snake::DirectionUp;
        break;
        case Snake::DirectionRight:
            oppositeDirection=Snake::DirectionLeft;
        break;
        case Snake::DirectionLeft:
            oppositeDirection=Snake::DirectionRight;
        break;

    }
    if(snake.getLastDirect() != oppositeDirection && snake.getLastDirect() != input){ //this line for not speeding up the snake
        exitSignal=true;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    exitSignal=false;
    return snake.getLastDirect() != oppositeDirection && snake.getLastDirect() != input;

}


void SnakeGame::directionThread(Snake::Direction direct){
    try{
        while(!exitSignal){
            map.show();
            if(snake.directTo(direct)==false){
                snake.directTo(snake.getLastDirect());
            }
            auto start = std::chrono::high_resolution_clock::now();
            while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()-start).count()<(SNAKE_SPEED_BASE_PERIOD - snakeSpeedDelta) && !exitSignal);
        }
    }catch (std::exception&) {
           exceptionFlag=true; //this flag is for game ended and should decide continue or exit?
           std::system("clear");
           std::cout <<  "\n\tDo you want to exit? (y)(n)\n\n\n"
                        R"(
                 /-------------\
                 |  Game Over! |
                 \-------------/
                      \
                         \     ^__^
                            \  (oo)\_______
                               (__)\       )\/\
                                   ||----w |
                                   ||     ||)"
                       << std::endl;

       }
}

void SnakeGame::begin(){

    char input=(char)Snake::DirectionRight;
    while(!restart){
        switch (input){
            case Snake::DirectionUp: //Up
                if(killDirectionThread(input))//This block provides kill direction thread immediately. So It is written especially every valid case of switch statement.
                    std::thread(&SnakeGame::directionThread,this,Snake::DirectionUp).detach();
            break;
            case Snake::DirectionDown: //Down
                if(killDirectionThread(input))//This block provides kill direction thread immediately. So It is written especially every valid case of switch statement.
                   std::thread(&SnakeGame::directionThread,this,Snake::DirectionDown).detach();
            break;
            case Snake::DirectionRight: //Right
                if(killDirectionThread(input))//This block provides kill direction thread immediately. So It is written especially every valid case of switch statement.
                    std::thread(&SnakeGame::directionThread,this,Snake::DirectionRight).detach();
            break;
            case Snake::DirectionLeft: //Left
                if(killDirectionThread(input))//This block provides kill direction thread immediately. So It is written especially every valid case of switch statement.
                    std::thread(&SnakeGame::directionThread,this,Snake::DirectionLeft).detach();
            break;
            case 'q': //Quit the program
                killDirectionThread(input);//This block provides kill direction thread immediately. So It is written especially every valid case of switch statement.
                system("clear");
                std::cout << "\n\n\n\n\n\n\n\t\tDo you want to exit? (y)(n)\n\n\n\n\n\n" << std::endl;
                std::cin >> input;
                switch (input) {
                    case 'y':
                        system("clear");
                        std::cout << "\n\n\n\n\n\n\t\tGood Bye.\n\n\n\n\n\n" << std::endl;
                        exit(0);
                    break;
                    case 'n':
                        input = snake.getLastDirect();
                        continue;
                    break;
                    default:
                        std::cout << "\n\n\n\n\n\n\n\t\tDo you want to exit? (y)(n)\n\n\n\n\n\n" << std::endl;
                    break;

                }
            break;
        default:
            //nothing
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        query:
        std::cin >> input;
        if(exceptionFlag){ // this block is for answering the question what is continue or exit? // this block use last stdin character
            switch (input) {
                case 'y':
                system("clear");
                std::cout << "\n\n\n\n\n\n\t\tGood Bye.\n\n\n\n\n\n" << std::endl;
                exit(0);
                break;
                case 'n':
                restart = true;
                exceptionFlag=false;
                break;
                default:
                std::system("clear");
                std::cout << "\n\n\n\n\n\n\n\t\tDo you want to exit? (y)(n)\n\n\n\n\n\n" << std::endl;
                goto query;
                break;

            }
        }
    }
    //std::cout << "\n\nDo you want to play again? (y) (n)" <<std::endl;

}
