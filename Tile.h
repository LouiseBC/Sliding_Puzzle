#ifndef Tile_h
#define Tile_h
#include <SDL2/SDL.h>

class Tile {
public:
    Tile(SDL_Rect r, int c, int t) :
    tilePosition{r}, currentNumber{c}, tiletype{t} {}
    
    void swap(Tile& t);
    void setTileType(int n)   { tiletype = n; }
    void setTileShift(const int& x, const int& y) { tilePosition.x += x; tilePosition.y += y; }
    
    SDL_Rect position() const { return tilePosition; }
    int posNumber()     const { return currentNumber; }
    int tileType()      const { return tiletype; }
    
    enum type { button = 0, rollover, shadow, invisible, buttonpressed};
    
private:
    SDL_Rect tilePosition;
    int currentNumber;
    int tiletype;
};

#endif
