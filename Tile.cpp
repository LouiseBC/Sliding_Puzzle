#include "Tile.h"

void Tile::swap(Tile& t) {
    // Swap destination rect (current tile position)
    SDL_Rect tempRect;
    tempRect = tilePosition;
    tilePosition = t.tilePosition;
    t.tilePosition = tempRect;
    
    // Swap corresponding position number
    int tempNum;
    tempNum = currentNumber;
    currentNumber = t.currentNumber;
    t.currentNumber = tempNum;
}
