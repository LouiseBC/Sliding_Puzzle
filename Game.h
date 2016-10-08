#ifndef Game_hpp
#define Game_hpp
#include <vector>
#include "Tile.h"

class Game {
public:
    Game();
    
    void gameLoop();
    void makeTiles(const int& n);
    void loadPositions(const int& n);
    void scrambleTiles(std::vector<Tile> t);
    bool isSolved();
    void restart();
    
    bool isNeighbor(const Tile& a, const Tile& b);
    int clickedTile(const int& x, const int& y);
    
private:
    int  clicks     { 0 };
    int  gameState  { 0 };
    bool gameExit   { false };
    bool gameWin    { true  };
    bool catMode    { false };
    
    std::vector<Tile> tiles;
    std::vector<SDL_Rect> positions;
};

#endif /* Game_hpp */
