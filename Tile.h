#ifndef Tile_h
#define Tile_h
#include <SDL2/SDL.h>

class Tile {
public:
    Tile(SDL_Rect r, int n, int c) :
    tilePosition{r}, originalNumber{n}, currentNumber{c} {}
    
    void swap(Tile& t);
    
    SDL_Rect position() const { return tilePosition; }
    int number() const { return originalNumber; }
    int posNumber() const { return currentNumber; }
    void setPosNumber(int n) { currentNumber = n; }
    
private:
    SDL_Rect tilePosition;
    int originalNumber;
    int currentNumber;
};

#endif
