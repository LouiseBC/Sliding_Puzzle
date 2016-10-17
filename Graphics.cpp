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

void Graphics::drawBoard(const std::vector<Tile>& v, const bool& drawnumber) {
    SDL_SetRenderDrawColor(renderer, 172, 119, 120, 1); // bg colour
    SDL_RenderClear(renderer);
    
    for (int i = 0; i < v.size(); ++i)
        drawTile(v[i], drawnumber);
}

void Graphics::drawTile(const Tile& t, const bool& drawnumber) {
    SDL_SetRenderDrawColor(renderer, 238, 230, 230, 1); // tile colour
    
    // Draw Menu Tile
    if (drawnumber == false){
        if (t.posNumber() == 1) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1); // white - mouseover for menu
            SDL_Rect temp = t.position();
            SDL_RenderFillRect(renderer, &temp);
        }
        else {
            SDL_Rect temp = t.position();
            SDL_RenderFillRect(renderer, &temp);
        }
    }
    // Draw Main Game Tile
    else {
        // Keep last tile invisible
        if (t.number() == (grid*grid))
            SDL_SetRenderDrawColor(renderer, 172, 119, 120, 1); // bg colour
        
        // Render tile
        SDL_Rect temp = t.position();
        SDL_RenderFillRect(renderer, &temp);
        
        // Render tile number
        if (t.number() != (grid*grid)) {
            std::string msg = std::to_string(t.number());
            SDL_Colour colour {39, 134, 175, 1};
            tilenumber = renderText(msg, "assets/Calibrib.ttf", colour, 25);
            SDL_QueryTexture(tilenumber, NULL, NULL, &temp.w, &temp.h);
            renderTexture(tilenumber, temp.x+(TILE_SIZE/2 - temp.w/2), temp.y+(TILE_SIZE/2 - temp.h/2), nullptr);
        }
    }
}

void Graphics::setGridSize(const int& n) {
    grid = n;
    rawSize = (WINDOW_WIDTH - 2*WINDOW_PADDING) / grid;
    TILE_PADDING = rawSize / 10;
    TILE_SIZE    = rawSize - TILE_PADDING;
}

void Graphics::updateClicks(const int& n) {
    std::string count = std::to_string(n);
    std::string message = "Clicks: " + count;
    clicks = renderText(message, "assets/Calibrib.ttf", fontcolour, 17);
    int W; int H;
    SDL_QueryTexture(clicks, NULL, NULL, &W, &H);
    renderTexture(clicks, (WINDOW_PADDING), (WINDOW_HEIGHT - 0.75*WINDOW_PADDING - H), nullptr);
}

void Graphics::winMessage(bool win, int clicks) {
    if (win == true && clicks > 0){
        wintext = renderText("You win! Click anywhere to restart", "assets/Calibrib.ttf", fontcolour, 20);
        int W; int H;
        SDL_QueryTexture(wintext, NULL, NULL, &W, &H);
        renderTexture(wintext, (0.5*WINDOW_WIDTH - 0.5*W), (0.5*WINDOW_PADDING - 0.5*H), nullptr);
    }
}

void Graphics::menuText(std::vector<SDL_Rect> positions) {
    int number = 3;
    SDL_Color test {0, 0, 0, 1};
    int W; int H;
    
    for (int i = 0; i < 6; ++i){
        std::string numStr = std::to_string(number);
        std::string message = numStr + " x " + numStr;
        
        menutext = renderText(message, "assets/Calibrib.ttf", test, 25);
        SDL_QueryTexture(menutext, NULL, NULL, &W, &H);
        renderTexture(menutext, (positions[i].x + 0.5*positions[i].w - 0.5*W), (positions[i].y + 0.5*positions[i].h - 0.5*H), nullptr);
        
        number += 1;
    }
    
    menutext = renderText("START", "assets/Calibrib.ttf", test, 25);
    SDL_QueryTexture(menutext, NULL, NULL, &W, &H);
    renderTexture(menutext, (positions[7].x + 0.5*positions[7].w - 0.5*W), (positions[7].y + 0.5*positions[7].h - 0.5*H), nullptr);
    
    menutext = renderText("Select your grid size:", "assets/Calibrib.ttf", test, 25);
    SDL_QueryTexture(menutext, NULL, NULL, &W, &H);
    renderTexture(menutext, 0.5*WINDOW_WIDTH - 0.5*W, WINDOW_PADDING-1.5*H, nullptr);
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