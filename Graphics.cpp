#include "Graphics.h"
#include "Tile.h"
#include <iostream>
#include <SDL2_ttf/SDL_ttf.h>


Graphics::Graphics() {
    
}

void Graphics::setup() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cerr << "Error: init" << SDL_GetError() << std::endl;
    
    else {
        window = SDL_CreateWindow("Slider Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == nullptr)
            std::cerr << "Error: Create window" << SDL_GetError() << std::endl;
        
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (renderer == nullptr)
                std::cerr << "Error: Create renderer" << SDL_GetError() << std::endl;
            
            else {
                if( TTF_Init() == -1 )
                    std::cerr << "SDL_ttf could not initialize!" << TTF_GetError() << std::endl;
            }
        }
    }
}

void Graphics::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Graphics::drawBoard(const std::vector<Tile>& v) {
    SDL_SetRenderDrawColor(renderer, 153, 184, 152, 1); // bg colour
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < v.size(); ++i)
        drawTile(v[i]);
}

void Graphics::drawTile(const Tile& t) {
    // Keep tile0 invisible
    if (t.number() != 0)
        SDL_SetRenderDrawColor(renderer, 42, 54, 59, 1);
    
    // Render tile
    SDL_Rect temp = t.position();
    SDL_RenderFillRect(renderer, &temp);
    
    // Render tile number
    if (t.number() != 0) {
        std::string msg = std::to_string(t.number());
        SDL_Colour colour {232, 74, 95, 1};
        tilenumber = renderText(msg, "assets/Calibrib.ttf", colour, 25);
        SDL_QueryTexture(tilenumber, NULL, NULL, &temp.w, &temp.h);
        renderTexture(tilenumber, temp.x+(TILE_SIZE/2 - temp.w/2), temp.y+(TILE_SIZE/2 - temp.h/2), nullptr);
    }
}

void Graphics::updateClicks(int n) {
    std::string count = std::to_string(n);
    std::string message = "Clicks: " + count;
    clicks = renderText(message, "assets/Calibrib.ttf", fontcolour, 17);
    int W; int H;
    SDL_QueryTexture(clicks, NULL, NULL, &W, &H);
    renderTexture(clicks, (WINDOW_PADDING), (WINDOW_HEIGHT - 0.75*WINDOW_PADDING - H), nullptr);
}

void Graphics::winMessage() {
    wintext = renderText("You win! Click anywhere to restart", "assets/Calibrib.ttf", fontcolour, 20);
    int W; int H;
    SDL_QueryTexture(wintext, NULL, NULL, &W, &H);
    renderTexture(wintext, (0.5*WINDOW_WIDTH - 0.5*W), (0.5*WINDOW_PADDING - 0.5*H), nullptr);
}

void Graphics::renderTexture(SDL_Texture *tex, int x, int y, SDL_Rect *clip = nullptr) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    SDL_RenderCopy(renderer, tex, clip, &dst);
}

SDL_Texture* Graphics::renderText(const std::string &message, const std::string &filepath, SDL_Color colour, int fontsize) {
    //Open the font
    TTF_Font *font = TTF_OpenFont(filepath.c_str(), fontsize);
    if (font == nullptr){
        std::cerr << "TTF_OpenFont" << TTF_GetError;
        return nullptr;
    }
    //render to a surface as that's what TTF_RenderText returns
    SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), colour);
    if (surf == nullptr){
        TTF_CloseFont(font);
        std::cout << "TTF_RenderText" << TTF_GetError;
        return nullptr;
    }
    //then load that surface into a texture
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        std::cout << "CreateTexture" << SDL_GetError();
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}