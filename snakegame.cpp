#include "snakegame.h"
#include <iostream>
#include <thread>
#include "snake.h"
#include "snakemap.h"
#ifdef _WIN32
#include <conio.h>
#endif

const int SnakeGame::SNAKE_SPEED_BASE_PERIOD=300;
const int SnakeGame::SNAKE_SPEED_BASE_DELTA=5;
const char SnakeGame::EXIT_QUESTION[]="\n\n\n\n\n\n\n\t\tDo you want to exit? (y)(n)\n\n\n\n\n\n";
const char SnakeGame::SAY_GOOD_BYE[]="\n\n\n\n\n\n\t\tGood Bye.\n\n\n\n\n\n";
const char SnakeGame::SAY_GAME_OVER[]="\n\tDo you want to exit? (y)(n)\n\n\n"
                                      R"(
                               /-------------\
                               |  Game Over! |
                               \-------------/
                                    \
                                       \     ^__^
                                          \  (oo)\_______
                                             (__)\       )\/\
                                                 ||----w |
                                                 ||     ||)";

SnakeGame::SnakeGame():exitSignal(false),map(SnakeMap{40,20}),restart(false),exceptionFlag(false),snakeSpeedDelta(0)
{
#ifndef _WIN32
    system("stty -echo -icanon"); //stty echo icanon
    system("setterm -cursor off");
#endif
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

    Snake::Direction oppositeDirection=Snake::DirectionLeft;
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
        default:
            oppositeDirection=Snake::DirectionLeft;
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
           map.clearTerminal();
           std::cout <<  SAY_GAME_OVER << std::endl;

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
                map.clearTerminal();
                std::cout << EXIT_QUESTION << std::endl;
#ifdef _WIN32
                input=(char)_getch();
#else
                std::cin >> input;
#endif

                switch (input) {
                    case 'y':
                        map.clearTerminal();
                        std::cout << SAY_GOOD_BYE << std::endl;
                        exit(0);
                    break;
                    case 'n':
                        std::thread(&SnakeGame::directionThread,this,snake.getLastDirect()).detach();
                        continue;
                    break;
                    default:
                        input='q';
                        continue;
                    break;

                }

            break;
        default:
            //nothing
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        query:
#ifdef _WIN32
        input=(char)_getch();
#else
        std::cin >> input;
#endif
        if(exceptionFlag){ // this block is for answering the question what is continue or exit? // this block use last stdin character
            switch (input) {
                case 'y':
                map.clearTerminal();
                std::cout << "\n\n\n\n\n\n\t\tGood Bye.\n\n\n\n\n\n" << std::endl;
                exit(0);
                break;
                case 'n':
                restart = true;
                exceptionFlag=false;
                break;
                default:
                map.clearTerminal();
                std::cout << "\n\n\n\n\n\n\n\t\tDo you want to exit? (y)(n)\n\n\n\n\n\n" << std::endl;
                goto query;
                break;

            }
        }
    }
    //std::cout << "\n\nDo you want to play again? (y) (n)" <<std::endl;

}
