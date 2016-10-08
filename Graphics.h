#ifndef Graphics_h
#define Graphics_h
#include <SDL2/SDL.h>
#include <vector>
#include "Tile.h"

class Graphics {
public:
    Graphics();
    void setup();
    void destroy();
    
    void drawBoard(const std::vector<Tile>& t);
    void drawTile(const Tile& t);
    void winMessage();
    void updateClicks(int n);
    void update() { SDL_RenderPresent(renderer); }
    
    SDL_Texture* renderText(const std::string &message, const std::string &filepath, SDL_Color colour, int fontsize);
    void renderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip);
    
    int  tileSize()     { return TILE_SIZE; }
    int  tilePadding()  { return TILE_PADDING; }
    int  winPadding()   { return WINDOW_PADDING; }
    int  gridSize       { 8 };

private:
    const int WINDOW_WIDTH   { 500 };
    const int WINDOW_HEIGHT  { 500 };
    const int WINDOW_PADDING { 75 };
    
    int rawSize { (WINDOW_WIDTH - 2*WINDOW_PADDING) / gridSize };
    
    const int TILE_PADDING = rawSize/10;
    const int TILE_SIZE = rawSize-TILE_PADDING;
    
    SDL_Colour fontcolour {255,255,255, 1};
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* tilenumber;
    SDL_Texture* clicks;
    SDL_Texture* wintext;
};

#endif
