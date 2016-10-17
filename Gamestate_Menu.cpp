#include "Gamestate_Menu.h"
#include "Gamestate_MainGame.h"

bool MenuState::init(Graphics* graph, Game* g){
    graphics = graph;
    game = g;
    
    loadPositions(gridSize);
    makeTiles();
    
    // nonsense audio loading - to do
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    
    rollOver = Mix_LoadWAV("assets/hover.wav");
    if( rollOver == NULL ) {
        printf( "Failed to load 'rollover' SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }

    clickSound = Mix_LoadWAV("assets/click.wav");
    if( clickSound == NULL ) {
        printf( "Failed to load 'clicksound' SDL_mixer Error: %s\n", Mix_GetError() );
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
    
    if (e.type == SDL_KEYDOWN){
        if (e.key.keysym.sym == SDLK_ESCAPE)
            game->setQuit();
    }
    
    int x; int y;
    SDL_GetMouseState(&x, &y);
    mousePos = getActiveTile(x, y);
    
    if (e.type == SDL_MOUSEMOTION) {
        if (mousePos >= 0) {
            tiles[mousePos].setPosNumber(1);
            //prevMousePos = mousePos;
        }
        else if (prevMousePos >= 0) {
            tiles[prevMousePos].setPosNumber(0);
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
    if (mousePos >= 0 && prevMousePos == -1) {
        Mix_PlayChannel(-1, rollOver, 0);
        prevMousePos = mousePos;
    }
    if (click == true && mousePos >= 0) {
        switch(mousePos) {
            case 0: graphics->setGridSize(3);
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 1: graphics->setGridSize(4);
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 2: graphics->setGridSize(5);
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 3: graphics->setGridSize(6);
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 4: graphics->setGridSize(7);
                Mix_PlayChannel(-1, clickSound, 0);
                break;
            case 5: graphics->setGridSize(8);
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
    graphics->drawBoard(tiles, false);
    graphics->menuText(positions);
    graphics->update();
}

void MenuState::loadPositions(const int& n) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < n; ++i) {
        y = graphics->winPadding() + i*tileSize + i*tilePadding;
        for (int j = 0; j < n; ++j) {
            x = graphics->winPadding() + j*tileSize + j*tilePadding;
            positions.push_back( SDL_Rect{ x, y, tileSize, tileSize } );
        }
    }
}

void MenuState::makeTiles() {
    for (int i = 0; i < positions.size(); ++i) {
        tiles.push_back(Tile{positions[i], i, 0}); // 'current position' set to 1 on mouseover
    }
}