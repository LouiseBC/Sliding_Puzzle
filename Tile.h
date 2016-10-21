#ifndef Tile_h
#define Tile_h
#include <SDL2/SDL.h>

class Tile {
public:
    Tile(SDL_Rect r, int n, int c) :
    tilePosition{r}, originalNumber{n}, currentNumber{c} {}
    
    Tile(SDL_Rect r, int n, int c, int t) :
    tilePosition{r}, originalNumber{n}, currentNumber{c}, tiletype{t} {}
    
    void swap(Tile& t);
    
    SDL_Rect position() const { return tilePosition; }
    int number()        const { return originalNumber; }
    int posNumber()     const { return currentNumber; }
    int tileType()      const { return tiletype; }
    void setTileType(int n)   { tiletype = n; }
    void setTileShift(const int& x, const int& y) { tilePosition.x += x; tilePosition.y += y; }
    enum type { button = 0, rollover, shadow, invisible, buttonpressed};
    
private:
    SDL_Rect tilePosition;
    int originalNumber;
    int currentNumber;
    int tiletype;
};

#endif
