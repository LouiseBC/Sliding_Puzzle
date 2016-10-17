#ifndef Gamestate_Menu_h
#define Gamestate_Menu_h
#include "Gamestate.h"
#include <SDL2_mixer/SDL_mixer.h>

class MenuState : public GameState {
public:
    bool init(Graphics* graphics, Game* game);
    void quit();
    
    void handleEvents(SDL_Event& e);
    void update();
    void render();
private:
    Graphics* graphics = NULL;
    Game* game = NULL;
    
    // MainGame Functions
    void loadPositions(const int& gridsize);
    void makeTiles();
    int  getActiveTile(const int &x, const int &y);
    
    // MainGame Data
    const int gridSize    { 3 };
    const int tileSize    { 105 };
    const int tilePadding { 11 };
    
    int  mousePos     { -1 };
    int  prevMousePos { 0 };
    bool click        { false };
    
    std::vector<Tile> tiles;
    std::vector<SDL_Rect> positions;
    
    Mix_Chunk* rollOver = NULL;
    Mix_Chunk* clickSound = NULL;
};

#endif