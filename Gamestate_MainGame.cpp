#include <iostream>
#include <random>
#include "Gamestate_MainGame.h"

// Main Gamestate Class Definition
///////////////////////////

bool MainGame::init(Graphics* graph, Game* g) {
    //Assign values to pointers
    game     = g;
    graphics = graph;
    
    // Fill vector<SDL_Rect> 'positions' with possible positions of n*n tiles
    loadPositions(graphics->gridSize());
    // Assign these starting positions to n*n tiles in vector<Tile> 'tiles'.
    makeTiles();
    
    // nonsense audio loading - to do
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    click = Mix_LoadWAV("assets/hover.wav");
    if( click == NULL ) {
        printf( "Failed to load beat 'click' SDL_mixer Error: %s\n", Mix_GetError() );
        return false;
    }
    
    return true;
}

void MainGame::quit() {
    game     = NULL;
    graphics = NULL;
    click    = NULL;
    Mix_Quit();
}

void MainGame::handleEvents(SDL_Event& event) {
    if (event.type == SDL_QUIT)
        game->setQuit();
    
    if (event.type == SDL_KEYDOWN){
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game->popState();
    }
    
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (gameWin == true)
            restart();
        
        int x; int y;
        SDL_GetMouseState(&x, &y);
        clickedTile = getClickedTile(x, y); // retreives clicked tile number or -1
        
    }
}

void MainGame::update() {
    if (gameWin == false && isSolved()){ // unnecessary 'gamewin == false'? to do
        gameWin = true;
    }
    
    else if (clickedTile >= 0) { // if mouse clicked on a tile
        if (isNeighbor(tiles[clickedTile], tiles[tiles.size()-1])){
            tiles[clickedTile].swap(tiles[tiles.size()-1]);
            clickedTile = -1; // reset to defaul
            
            clicks += 1;

            //if (catMode == true)
                Mix_PlayChannel(-1, click, 0);
        }
        
    }
}

void MainGame::render() {
    graphics->drawBoard(tiles, true);
    graphics->updateClicks(clicks);
    graphics->winMessage(gameWin, clicks); // make sure it's not the start of the game
    graphics->update();
}

void MainGame::restart() {
    gameWin = false;
    clicks = 0;
    
    scrambleTiles(tiles);
}

void MainGame::loadPositions(const int& n) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < n; ++i) {
        y = graphics->winPadding() + i*graphics->tileSize() + i*graphics->tilePadding();
        for (int j = 0; j < n; ++j) {
            x = graphics->winPadding() + j*graphics->tileSize() + j*graphics->tilePadding();
            positions.push_back( SDL_Rect{ x, y, graphics->tileSize(), graphics->tileSize() });
        }
    }
}

void MainGame::makeTiles() {
    for (int i = 0; i < positions.size(); ++i) {
        tiles.push_back(Tile{positions[i], i+1, i}); // start counting from 1
    }
}

void MainGame::scrambleTiles(std::vector<Tile> t) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> range(0, tiles.size()-2);
    
    for (int i = 0; i < tiles.size()-2; ++i) {
        int n = range(rng);
        tiles[i].swap(tiles[n]);
    }
}

int MainGame::getClickedTile(const int &x, const int &y) {
    int tilenum = -1;
    for (int i = 0; i < tiles.size(); ++i) {
        if (! (x < tiles[i].position().x || x > tiles[i].position().x + graphics->tileSize() ||
               y < tiles[i].position().y || y > tiles[i].position().y + graphics->tileSize() ))
            tilenum = i ;
    }
    return tilenum;
}

bool MainGame::isNeighbor(const Tile& a, const Tile& b) {
    if (a.posNumber() == b.posNumber() - 1 ||
        a.posNumber() == b.posNumber() + 1 ||
        a.posNumber() == b.posNumber() + graphics->gridSize() ||
        a.posNumber() == b.posNumber() - graphics->gridSize())
        return true;
    return false;
}

bool MainGame::isSolved() {
    int correctTilesN = 0;
    
    for (int i = 0; i < tiles.size(); ++i) {
        if (tiles[i].position().x == positions[i].x &&
            tiles[i].position().y == positions[i].y)
            correctTilesN += 1;
    }
    if (correctTilesN == graphics->gridSize()*graphics->gridSize())
        return true;
    return false;
}