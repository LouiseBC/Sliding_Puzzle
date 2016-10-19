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
    
    // MenuState Functions
    void loadPositions (std::vector<SDL_Rect>& positions, const int& gridsize);
    void loadPositions (std::vector<SDL_Rect>& positions, const int& gridsize, const int& shiftPx);
    void makeTiles     (std::vector<Tile>& tiles, const std::vector<SDL_Rect>& positions, const int& tileType);
    int  getActiveTile (const int &x, const int &y);
    void pushTile      (const int& mousePos);
    void liftTile      (const int& buttonClicked);
    void pushButton    ();
    
    // MenuState Data
    const int gridSize    { 3 };
    const int tileSize    { 105 };
    const int tilePadding { 11 };
    const int shiftAmount { 7 };
    
    int  mousePos      { -1 };
    int  prevMousePos  { 0 };
    int  prevButtonClicked { -1 };
    bool click         { false };
    
    std::vector<Tile> tiles;
    std::vector<Tile> shiftTiles;
    std::vector<SDL_Rect> positions;
    std::vector<SDL_Rect> shiftPositions;
    
    Mix_Chunk* rollOver = NULL;
    Mix_Chunk* clickSound = NULL;
};

#endif