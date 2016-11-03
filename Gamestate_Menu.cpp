#include "Gamestate_Menu.h"
#include "Gamestate_MainGame.h"
#include <iostream>

bool MenuState::init(Graphics* graph, Game* g){
    graphics = graph;
    game = g;
    int shiftAmount = 6;
    
    // Make menu tiles
    loadPositions(positions, gridSize);
    makeTiles(tiles, positions, Tile::type::button);
    // Make menu 'shadow' tiles
    loadPositions(shadowPositions, gridSize, shiftAmount);
    makeTiles(shadowTiles, shadowPositions, Tile::type::shadow);
    
    return loadAudio();
}

bool MenuState::loadAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    rollOver = Mix_LoadWAV("assets/hover.wav");
    if( rollOver == NULL ) {
        std::cerr << "Failed to load beat 'rollover', error:" << Mix_GetError() << std::endl;
        return false;
    }
    clickSound = Mix_LoadWAV("assets/press.wav");
    if( clickSound == NULL ) {
        std::cerr << "Failed to load beat 'clicksound', error:" << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void MenuState::loadPositions(std::vector<SDL_Rect>& shadowPositions, const int& gridsize, const int& shiftPx) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < gridsize; ++i) {
        y = graphics->winPadding() + i*tileSize + i*tilePadding + shiftPx;
        for (int j = 0; j < gridsize; ++j) {
            x = graphics->winPadding() + j*tileSize + j*tilePadding + shiftPx;
            shadowPositions.push_back( SDL_Rect{ x, y, tileSize, tileSize } );
        }
    }
}

void MenuState::makeTiles(std::vector<Tile>& tiles, const std::vector<SDL_Rect>& positions, const int& tileType) {
    for (int i = 0; i < positions.size(); ++i)
        tiles.push_back(Tile{positions[i], i, tileType});
}

void MenuState::handleEvents(SDL_Event& e) {
    if (e.type == SDL_QUIT)
        game->setQuit();
    
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE)
            game->setQuit();
    }
    
    if (e.type == SDL_MOUSEMOTION) {
        int x; int y;
        SDL_GetMouseState(&x, &y);
        mousePos = getActiveTile(x, y);
    }
    
    if (e.type == SDL_MOUSEBUTTONDOWN)
        click = true;
}

int MenuState::getActiveTile(const int &x, const int &y) {
    int tilenum = -1;
    for (int i = tiles.size(); i >= 0; --i) {
        if ( !(x < tiles[i].position().x || x > tiles[i].position().x + tiles[i].position().w ||
               y < tiles[i].position().y || y > tiles[i].position().y + tiles[i].position().h) )
            tilenum = i ;
    }
    return tilenum;
}

void MenuState::update() {
    // if cursor landed on a button
    if (mousePos >=0) {
        
        setRollOver();
        
        if (click == true) {
            switch(mousePos) {
                case 0: graphics->setGridSize(3);
                        pushButton();
                    break;
                case 1: graphics->setGridSize(4);
                        pushButton();
                    break;
                case 2: graphics->setGridSize(5);
                        pushButton();
                    break;
                case 3: graphics->setGridSize(6);
                        pushButton();
                    break;
                case 4: graphics->setGridSize(7);
                        pushButton();
                    break;
                case 5: graphics->setGridSize(8);
                        pushButton();
                    break;
                case 6: pushButton();
                    break;
                case 7: game->pushState(new MainGame);
                    break;
            }
            click = false;
        }
    }
    else if (prevMousePos >= 0) {// unset rollover
        if (tiles[prevMousePos].tileType() != Tile::type::buttonpressed)
            tiles[prevMousePos].setTileType(Tile::type::button);
    }
}

void MenuState::setRollOver() {
    // Play sound if cursor first landed on button
    if (tiles[mousePos].tileType() != Tile::type::rollover &&
        tiles[mousePos].tileType() != Tile::type::buttonpressed)
        Mix_PlayChannel(-1, rollOver, 0);
    
    // reset all tiles
    for (int i = tiles.size(); i >= 0; --i) {
        if (tiles[i].tileType() != Tile::type::buttonpressed)
            tiles[i].setTileType(Tile::type::button);
    }
    // set rollover tile
    if (tiles[mousePos].tileType() != Tile::type::buttonpressed)
        tiles[mousePos].setTileType(Tile::type::rollover);
    
    prevMousePos = mousePos;
}

void MenuState::pushButton() {
    // Catmode button can be turned on/off
    if (mousePos == 6) {
        Mix_PlayChannel(-1, clickSound, 0);
        if (game->isCatMode())
            liftTile(mousePos);
        else
            pushTile(mousePos);
        game->toggleCatMode();
    }
    // all other tiles can only be turned on
    else if (prevButtonClicked != mousePos) {
        Mix_PlayChannel(-1, clickSound, 0);
        pushTile(mousePos);
        if (prevButtonClicked >= 0) {
            liftTile(prevButtonClicked);
        }
        prevButtonClicked = mousePos;
    }
}

void MenuState::pushTile(const int& mousepos) {
    tiles[mousepos].setTileShift(2, 2);
    tiles[mousepos].setTileType(Tile::type::buttonpressed);
}
void MenuState::liftTile(const int& prevbuttonClicked) {
    tiles[prevbuttonClicked].setTileShift(-2, -2);
    tiles[prevbuttonClicked].setTileType(Tile::type::button);
}

void MenuState::render() {
    graphics->renderClear();
    graphics->drawBoard(shadowTiles);
    graphics->drawBoard(tiles);
    graphics->menuText(tiles, game->isCatMode());
    graphics->update();
}

MenuState::~MenuState() {
    Mix_Quit();
    graphics = nullptr;
    game = nullptr;
    rollOver = nullptr;
    clickSound = nullptr;
}