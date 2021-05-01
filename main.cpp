#include "snakegame.h"

using namespace std;

//TODO yemi bi önden yiyor arada bi boşluk varken, bu en öndeki dotu göstermediğinden olabilir.

int main()
{
    SnakeGame *game=new SnakeGame();
    while(true){
        game->begin();
        delete game;
        game= new SnakeGame();
    }
    return 0;
}
