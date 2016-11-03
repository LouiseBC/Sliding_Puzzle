#ifndef Gamestate_Menu_h
#define Gamestate_Menu_h
#include "Gamestate.h"
#include <SDL2_mixer/SDL_mixer.h>

class MenuState : public GameState {
public:
    bool init(Graphics* graphics, Game* game);
    ~MenuState();
    
    void handleEvents(SDL_Event& e);
    void update();
    void render();
private:
    Graphics* graphics = nullptr;
    Game* game = nullptr;
    
    // MenuState Functions
    void loadPositions (std::vector<SDL_Rect>& positions, const int& gridsize, const int& shiftPx = 0);
    void makeTiles     (std::vector<Tile>& tiles, const std::vector<SDL_Rect>& positions, const int& tileType);
    bool loadAudio     ();
    int  getActiveTile (const int &x, const int &y);
    void setRollOver   ();
    void pushButton    ();
    void pushTile      (const int& mousePos);
    void liftTile      (const int& buttonClicked);
    
    // MenuState Data
    const int gridSize     { 3 };
    const int tileSize     { 105 };
    const int tilePadding  { 11 };
    
    int  mousePos          { -1 };
    int  prevMousePos      { -1 };
    int  prevButtonClicked { -1 };
    bool click             { false };
    
    std::vector<Tile> tiles;
    std::vector<Tile> shadowTiles;
    std::vector<SDL_Rect> positions;
    std::vector<SDL_Rect> shadowPositions;
    
    Mix_Chunk* rollOver = nullptr;
    Mix_Chunk* clickSound = nullptr;
};

#endif