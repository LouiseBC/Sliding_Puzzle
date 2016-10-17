#ifndef Game_h
#define Game_h
#include <vector>
#include <SDL2/SDL.h>
#include "Graphics.h"

class GameState;

class Game {
public:
    Game();
    
    bool init(GameState* state);
    void loop();
    
    void pushState(GameState* state);
    void popState();
    void setQuit();
private:
    bool quit { false };
    
    Graphics graphics;
    SDL_Event event;
    std::vector<GameState*> states;
    
    //internal loop functions
    void update();
    void render();
    
    void quitGame(); //will free SDL resources and perform cleanup of states
};

#endif
