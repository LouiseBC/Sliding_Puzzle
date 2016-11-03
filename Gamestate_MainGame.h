#ifndef Gamestate_MainGame_h
#define Gamestate_MainGame_h
#include <SDL2_mixer/SDL_mixer.h>
#include "Gamestate.h"

class MainGame : public GameState {
    // Gamestate Functions
    bool init(Graphics* graphics, Game* game);
    ~MainGame();
    
    void handleEvents(SDL_Event& e);
    void update();
    void render();
    
private:
    Game* game = nullptr;
    Graphics* graphics = nullptr;
    
    // MainGame functions
    void makeTiles(std::vector<Tile>& tiles, const std::vector<SDL_Rect>& posiitons, const int& tiletype);
    void loadPositions(std::vector<SDL_Rect>& shadowpositions, const int& gridsize, const int& shiftpx = 0);
    void scrambleTiles(std::vector<Tile>& t, std::vector<Tile>& tshadow);
    bool loadAudio();
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
    std::vector<Tile> shadowTiles;
    std::vector<SDL_Rect> positions;
    std::vector<SDL_Rect> shadowPositions;
    
    Mix_Chunk* click = nullptr;
    Mix_Chunk* cat1  = nullptr;
    Mix_Chunk* cat2  = nullptr;
    Mix_Chunk* cat3  = nullptr;
};

#endif
