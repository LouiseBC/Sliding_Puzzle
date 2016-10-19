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
    void makeTiles(std::vector<Tile>& tiles);
    void makeTiles(std::vector<Tile>& shifttiles, const std::vector<SDL_Rect>& shiftpositions);
    void loadPositions(std::vector<SDL_Rect>& positions, const int& gridsize);
    void loadPositions(std::vector<SDL_Rect>& shiftpositions, const int& gridsize, const int& shiftpx);
    void scrambleTiles(std::vector<Tile> t);
    void restart();
    
    bool isSolved();
    bool isNeighbour(const Tile& a, const Tile& b);
    int  getClickedTile(const int& x, const int& y);
    
    // MainGame data
    int  clickedTile { -1 };
    int  clicks      { 0 };
    bool gameExit    { false };
    bool gameWin     { true  };
    
    std::vector<Tile> tiles;
    std::vector<Tile> shiftTiles;
    std::vector<SDL_Rect> positions;
    std::vector<SDL_Rect> shiftPositions;
    
    Mix_Chunk* click = NULL;
    Mix_Chunk* cat1 = NULL;
    Mix_Chunk* cat2 = NULL;
    Mix_Chunk* cat3 = NULL;
};

#endif
