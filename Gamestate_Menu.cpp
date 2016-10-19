#include "Gamestate_Menu.h"
#include "Gamestate_MainGame.h"
#include <iostream>

namespace tileType {
    int button = 0;
    int rollover = 1;
    int shadow = 2;
}

bool MenuState::init(Graphics* graph, Game* g){
    graphics = graph;
    game = g;
    
    // Make menu tiles
    loadPositions(positions, gridSize);
    makeTiles(tiles, positions, tileType::button);
    // Make menu 'shadow' tiles
    loadPositions(shiftPositions, gridSize, shiftAmount);
    makeTiles(shiftTiles, shiftPositions, tileType::shadow);
    
    // Load sounds
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    rollOver = Mix_LoadWAV("assets/hover.wav");
    if( rollOver == NULL ) {
        std::cerr << "Failed to load beat 'rollover', error:" << Mix_GetError() << std::endl;
        return false;
    }
    clickSound = Mix_LoadWAV("assets/click.wav");
    if( clickSound == NULL ) {
        std::cerr << "Failed to load beat 'clicksound', error:" << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void MenuState::quit(){
    graphics   = NULL;
    game       = NULL;
    rollOver   = NULL;
    clickSound = NULL;
    Mix_Quit();
}

void MenuState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_QUIT)
        game->setQuit();
    
    if (e.type == SDLK_SPACE && game->statesSize() > 1)
        game->popState();
    
    int x; int y;
    SDL_GetMouseState(&x, &y);
    mousePos = getActiveTile(x, y);
    
    if (e.type == SDL_MOUSEMOTION) {
        if (mousePos >= 0) {
            tiles[mousePos].setPosNumber(tileType::rollover);
            //prevMousePos = mousePos;
        }
        else if (prevMousePos >= 0) { // if mouse is *no longer* inside button
            tiles[prevMousePos].setPosNumber(tileType::button);
            prevMousePos = -1;
        }
    }
    
    if (e.type == SDL_MOUSEBUTTONDOWN)
        click = true;
}

int MenuState::getActiveTile(const int &x, const int &y) {
    int tilenum = -1;
    for (int i = 0; i < tiles.size(); ++i) {
        if ( !(x < tiles[i].position().x || x > tiles[i].position().x + tiles[i].position().w ||
               y < tiles[i].position().y || y > tiles[i].position().y + tiles[i].position().h) )
            tilenum = i ;
    }
    return tilenum;
}

void MenuState::update() {
    if (mousePos >= 0 && prevMousePos == -1) { // only play when mouse first touches button
        Mix_PlayChannel(-1, rollOver, 0);
        prevMousePos = mousePos;
    }
    if (click == true && mousePos >= 0) {
        switch(mousePos) {
            case 0: graphics->setGridSize(3);
                pushButton();
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 1: graphics->setGridSize(4);
                pushButton();
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 2: graphics->setGridSize(5);
                pushButton();
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 3: graphics->setGridSize(6);
                pushButton();
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 4: graphics->setGridSize(7);
                pushButton();
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 5: graphics->setGridSize(8);
                pushButton();
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 6:
                pushButton();
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 7: game->pushState(new MainGame);
                Mix_PlayChannel(-1, clickSound, 0);
                break;
        }
    }
    click = false;
}

void MenuState::render(){
    graphics->renderClear();
    graphics->drawBoard(shiftTiles, false);
    graphics->drawBoard(tiles, false);
    graphics->menuText(positions, game->isCatMode());
    graphics->update();
}

void MenuState::pushButton() {
    // Catmode button can be turned on/off
    if (mousePos == 6 && game->isCatMode()) {
        liftTile(mousePos);
        game->toggleCatMode();
    }
    else if (mousePos == 6 && !game->isCatMode()) {
        pushTile(mousePos);
        game->toggleCatMode();
    }
    // all other tiles can only be turned on
    else if (prevButtonClicked != mousePos) {
        pushTile(mousePos);
        if (prevButtonClicked >= 0) {
            liftTile(prevButtonClicked);
        }
        prevButtonClicked = mousePos;
    }
}

void MenuState::pushTile(const int& mousepos) {
        tiles[mousepos].setTileShift(2, 2);
}
void MenuState::liftTile(const int& prevbuttonClicked) {
        tiles[prevbuttonClicked].setTileShift(-2, -2);
    // maybe just inline these functions? Unless more is added to do
}

void MenuState::loadPositions(std::vector<SDL_Rect>& positions, const int& gridsize) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < gridsize; ++i) {
        y = graphics->winPadding() + i*tileSize + i*tilePadding;
        for (int j = 0; j < gridsize; ++j) {
            x = graphics->winPadding() + j*tileSize + j*tilePadding;
            positions.push_back( SDL_Rect{ x, y, tileSize, tileSize } );
        }
    }
}

void MenuState::loadPositions(std::vector<SDL_Rect>& shiftPositions, const int& gridsize, const int& shiftPx) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < gridsize; ++i) {
        y = graphics->winPadding() + i*tileSize + i*tilePadding + shiftPx;
        for (int j = 0; j < gridsize; ++j) {
            x = graphics->winPadding() + j*tileSize + j*tilePadding + shiftPx;
            shiftPositions.push_back( SDL_Rect{ x, y, tileSize, tileSize } );
        }
    }
}

void MenuState::makeTiles(std::vector<Tile>& tiles, const std::vector<SDL_Rect>& positions, const int& tileType) {
    for (int i = 0; i < positions.size(); ++i) {
        tiles.push_back(Tile{positions[i], i, tileType});
    }
}