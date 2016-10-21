#ifndef Graphics_h
#define Graphics_h
#include <SDL2/SDL.h>
#include <vector>
#include "Tile.h"

class Graphics {
public:
    void setup();
    void destroy();
    
    void setGridSize (const int& n);
    void drawBoard   (const std::vector<Tile>& t, const bool& drawnumber);
    void drawMenu    (const std::vector<Tile>& t);
    void drawTile    (const Tile& t, const bool& drawnumber);
    void winMessage  (const bool& win, const int& clicks, const std::string& message);
    void updateClicks(const int& n);
    void menuText(std::vector<Tile> tiles, const bool& catmode);
    void menuInstruction(const std::string& message);
    void renderCat(const std::vector<Tile>& tiles, const std::vector<SDL_Rect>& positions);
    
    void update()           { SDL_RenderPresent(renderer); }
    void renderClear()      { SDL_SetRenderDrawColor(renderer, 163, 118, 172, 1); SDL_RenderClear(renderer); }
    
    int  tileSize()     { return TILE_SIZE; }
    int  tilePadding()  { return TILE_PADDING; }
    int  winPadding()   { return WINDOW_PADDING; }
    int  gridSize()     { return grid; }

private:
    const int WINDOW_WIDTH   { 500 };
    const int WINDOW_HEIGHT  { 500 };
    const int WINDOW_PADDING { 75 };
    
    int grid       { 3 };
    int rawSize    { (WINDOW_WIDTH - 2*WINDOW_PADDING) / grid };
    
    int TILE_PADDING = rawSize / 10;
    int TILE_SIZE    = rawSize - TILE_PADDING;
    
    SDL_Colour fontcolour {0, 0, 0, 1};
    
    SDL_Window*   window     = NULL;
    SDL_Renderer* renderer   = NULL;
    
    SDL_Texture*  tilenumber = NULL;
    SDL_Texture*  clicks     = NULL;
    SDL_Texture*  wintext    = NULL;
    SDL_Texture*  menutext   = NULL;
    SDL_Texture*  menuinstr  = NULL;
    SDL_Texture*  cat        = NULL;
    
    SDL_Texture* renderText (const std::string &message, const std::string &filepath, SDL_Color colour, int fontsize);
    void         renderTexture  (SDL_Texture *tex, int x, int y, SDL_Rect *clip);
};

#endif
