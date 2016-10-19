#include "Graphics.h"
#include <iostream>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>

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
                SDL_SetRenderDrawColor(renderer, 163, 118, 172, 1);
                if( TTF_Init() == -1 )
                    std::cerr << "SDL_ttf could not initialize!" << TTF_GetError() << std::endl;
                else {
                    cat = IMG_LoadTexture(renderer, "assets/cat2.png");
                    if (cat == nullptr)
                    std::cerr << "Error: Load texture";
                }
            }
        }
    }
}

void Graphics::destroy() {
    window     = NULL;
    renderer   = NULL;
    tilenumber = NULL;
    clicks     = NULL;
    wintext    = NULL;
    menutext   = NULL;
    menuinstr  = NULL;
    cat        = NULL;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void Graphics::drawBoard(const std::vector<Tile>& v, const bool& drawnumber) {
    SDL_SetRenderDrawColor(renderer, 163, 118, 172, 1); // bg colour
    
    for (int i = 0; i < v.size(); ++i)
        drawTile(v[i], drawnumber);
}

void Graphics::drawTile(const Tile& t, const bool& drawnumber) {
    SDL_SetRenderDrawColor(renderer, 237, 229, 239, 1); // tile colour
    SDL_Rect temp = t.position();
    
    // Draw Menu/Shadow Tile
    if (drawnumber == false){
        // Mouseover: Draw white tile
        if (t.posNumber() == 1) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
            SDL_RenderFillRect(renderer, &temp);
        }
        // 'Shadow' menu tile: Draw darker
        if (t.posNumber() == 2) {
            SDL_SetRenderDrawColor(renderer, 100, 65, 107, 1);
            SDL_RenderFillRect(renderer, &temp);
        }
        // No mouseover: Draw default
        else
            SDL_RenderFillRect(renderer, &temp);
    }
    // Draw Main Game Tile
    else {
        // Keep last tile invisible
        if (t.number() == (grid*grid))
            SDL_SetRenderDrawColor(renderer, 163, 118, 172, 1); // bg colour
        
        // Render tile
        SDL_RenderFillRect(renderer, &temp);
        
        // Render tile number
        if (t.number() != (grid*grid)) {
            std::string msg = std::to_string(t.number());
            tilenumber = renderText(msg, "assets/Calibrib.ttf", fontcolour, 25);
            SDL_QueryTexture(tilenumber, NULL, NULL, &temp.w, &temp.h);
            renderTexture(tilenumber, temp.x+(TILE_SIZE/2 - temp.w/2), temp.y+(TILE_SIZE/2 - temp.h/2), nullptr);
        }
    }
}

void Graphics::renderCat(const std::vector<Tile>& tiles, const std::vector<SDL_Rect>& positions) {
    int position = 1;
    for (int i = 0; i < tiles.size()-1; ++i) {
        for (int j = 0; j < tiles.size()-1; ++j) {
            // 'attach' tile n to position n of cat photo
            if (tiles[j].number() == position) {
                SDL_Rect temp = positions[position-1];
                renderTexture(cat, tiles[j].position().x, tiles[j].position().y, &temp);
                position += 1;
            }
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
    SDL_Rect clickBanner       { WINDOW_PADDING + TILE_SIZE - 5, WINDOW_PADDING/2 };
    SDL_Rect clickBannerShadow { WINDOW_PADDING + TILE_SIZE - 5 + 2, WINDOW_PADDING/2 + 2};
    SDL_SetRenderDrawColor  (renderer, 100, 65, 107, 1);
    SDL_RenderFillRect      (renderer, &clickBannerShadow);
    SDL_SetRenderDrawColor  (renderer, 237, 229, 239, 1);
    SDL_RenderFillRect      (renderer, &clickBanner);
    
    std::string count = std::to_string(n);
    std::string message = "Clicks: " + count;
    clicks = renderText(message, "assets/GreenFlame.ttf", fontcolour, 15);
    int W; int H;
    SDL_QueryTexture(clicks, NULL, NULL, &W, &H);
    renderTexture(clicks, (WINDOW_PADDING), (WINDOW_PADDING - 1.2*H), nullptr);
}

void Graphics::winMessage(const bool& win, const int& clicks, const std::string& message) {
    if (win == true && clicks > 0){
        wintext = renderText(message, "assets/Calibrib.ttf", fontcolour, 20);
        int W; int H;
        SDL_QueryTexture(wintext, NULL, NULL, &W, &H);
        renderTexture(wintext, (0.5*WINDOW_WIDTH - 0.5*W), (0.5*WINDOW_PADDING - 0.5*H), nullptr);
    }
}

void Graphics::menuInstruction(const std::string& message) {
    SDL_Color darkpink { 100, 65, 107, 1 };
    menuinstr = renderText(message, "assets/GreenFlame.ttf", darkpink, 20);
    int W; int H;
    SDL_QueryTexture(wintext, NULL, NULL, &W, &H);
    renderTexture(menuinstr, (0.5*WINDOW_WIDTH - 0.5*W), (WINDOW_HEIGHT - 2*H), nullptr);
}

void Graphics::menuText(std::vector<SDL_Rect> positions, const bool& catmode) {
    int number = 3;
    int W; int H;
    
    for (int i = 0; i < 6; ++i){
        std::string numStr = std::to_string(number);
        std::string message = numStr + " x " + numStr;
        
        menutext = renderText(message, "assets/Calibrib.ttf", fontcolour, 25);
        SDL_QueryTexture(menutext, NULL, NULL, &W, &H);
        renderTexture(menutext, (positions[i].x + 0.5*positions[i].w - 0.5*W), (positions[i].y + 0.5*positions[i].h - 0.5*H), nullptr);
        
        number += 1;
    }
    if (catmode){
        SDL_Color tilecolour { 237, 229, 239, 1 };
        menutext = renderText("Cat Mode", "assets/Calibrib.ttf", tilecolour, 20);
        SDL_QueryTexture(menutext, NULL, NULL, &W, &H);
        renderTexture(menutext, (positions[6].x + 0.5*positions[6].w - 0.5*W + 2), (positions[6].y + 0.5*positions[6].h - 0.5*H + 2), nullptr);
    }
    
    menutext = renderText("START", "assets/Calibrib.ttf", fontcolour, 25);
    SDL_QueryTexture(menutext, NULL, NULL, &W, &H);
    renderTexture(menutext, (positions[7].x + 0.5*positions[7].w - 0.5*W), (positions[7].y + 0.5*positions[7].h - 0.5*H), nullptr);
    
    menutext = renderText("Select your grid size:", "assets/GreenFlame.ttf", fontcolour, 20);
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