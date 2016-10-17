#ifndef Gamestate_MainGame_h
#define Gamestate_MainGame_h
#include <SDL2_mixer/SDL_mixer.h>
#include "Gamestate.h"

class MainGame : public GameState {
    // Gamestate Functions
    bool init(Graphics* graphics, Game* game);
    void quit();
    
    void handleEvents(SDL_Event& e);
    void update();
    void render();
    
private:
    Game* game = NULL;
    Graphics* graphics = NULL;
    
    // MainGame functions
    void makeTiles();
    void loadPositions(const int & n);
    void scrambleTiles(std::vector<Tile> t);
    void restart();
    
    bool isSolved();
    bool isNeighbor(const Tile& a, const Tile& b);
    int  getClickedTile(const int& x, const int& y);
    
    // MainGame data
    int  clickedTile { -1 };
    int  clicks      { 0 };
    bool gameExit    { false };
    bool gameWin     { true  };
    bool catMode     { true };
    
    std::vector<Tile> tiles;
    std::vector<SDL_Rect> positions;
    
    Mix_Chunk* click = NULL;
};

#endif
