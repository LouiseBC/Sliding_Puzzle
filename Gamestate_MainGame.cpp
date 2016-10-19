#include <iostream>
#include <random>
#include "Gamestate_MainGame.h"

bool MainGame::init(Graphics* graph, Game* g) {
    //Assign values to pointers
    game     = g;
    graphics = graph;
    int shiftamount = 3;
    
    // Fill vector<SDL_Rect> 'positions' with possible positions of n*n tiles & make shadow positions
    loadPositions(positions, graphics->gridSize());
    loadPositions(shiftPositions, graphics->gridSize(), shiftamount);
    // Assign these starting positions to n*n tiles in vector<Tile> 'tiles' & make shadow tiles
    makeTiles(tiles);
    makeTiles(shiftTiles, shiftPositions);
    
    // nonsense audio loading - to do
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    click = Mix_LoadWAV("assets/hover.wav");
    if( click == NULL ) {
        std::cerr << "Failed to load beat 'click', error:" << Mix_GetError() << std::endl;
        return false;
    }
    cat1 = Mix_LoadWAV("assets/meow1.wav");
    if( cat1 == NULL ) {
        std::cerr << "Failed to load beat 'meow', error:" << Mix_GetError() << std::endl;
        return false;
    }
    cat2 = Mix_LoadWAV("assets/meow2.wav");
    if( cat2 == NULL ) {
        std::cerr << "Failed to load beat 'meow', error:" << Mix_GetError() << std::endl;
        return false;
    }
    cat3 = Mix_LoadWAV("assets/meow3.wav");
    if( cat3 == NULL ) {
        std::cerr << "Failed to load beat 'meow', error:" << Mix_GetError() << std::endl;
        return false;
    }
    
    return true;
}

void MainGame::quit() {
    game     = NULL;
    graphics = NULL;
    click    = NULL;
    cat1     = NULL;
    cat2     = NULL;
    cat3     = NULL;
    Mix_Quit();
}

void MainGame::handleEvents(SDL_Event& event) {
    if (event.type == SDL_QUIT)
        game->setQuit();
    
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_SPACE)
            game->popState();
    }
    
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        if (gameWin == true)
            restart();
        
        else {
            int x; int y;
            SDL_GetMouseState(&x, &y);
            clickedTile = getClickedTile(x, y); // retreives clicked tile number or -1
        }
    }
}

void MainGame::update() {
    if (isSolved()) { // unnecessary 'gamewin == false'? to do
        gameWin = true;
    }
    
    else if (clickedTile >= 0) { // if mouse clicked on a tile
        if (isNeighbour(tiles[clickedTile], tiles[tiles.size()-1])){
            tiles[clickedTile].swap(tiles[tiles.size()-1]);
            shiftTiles[clickedTile].swap(shiftTiles[tiles.size()-1]);
            clickedTile = -1; // reset to defaul
            
            ++clicks;
            
            if (game->isCatMode()){
                std::random_device rd;
                std::mt19937 rng(rd());
                std::uniform_int_distribution<int> range(0, 2);
                
                int random = range(rd);
                switch(random) {
                    case 0: Mix_PlayChannel(-1, cat1, 0); break;
                    case 1: Mix_PlayChannel(-1, cat2, 0); break;
                    case 2: Mix_PlayChannel(-1, cat3, 0); break;
                }
            }
            else
                Mix_PlayChannel(-1, click, 0);
        }
        
    }
}

void MainGame::render() {
    graphics->renderClear();
    graphics->drawBoard(shiftTiles, false);
    graphics->drawBoard(tiles, true);
    
    if (game->isCatMode()) {
        graphics->renderCat(tiles, positions);
    }
    
    graphics->updateClicks(clicks);
    graphics->winMessage(gameWin, clicks, "You win! Click anywhere to restart"); // make sure it's not the start of the game
    graphics->menuInstruction("Press [spacebar] to return to main menu");
    graphics->update();
}

void MainGame::restart() {
    gameWin = false;
    clicks = 0;
    
    scrambleTiles(tiles);
}

void MainGame::loadPositions(std::vector<SDL_Rect>& positions, const int& gridsize) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < gridsize; ++i) {
        y = graphics->winPadding() + i*graphics->tileSize() + i*graphics->tilePadding();
        for (int j = 0; j < gridsize; ++j) {
            x = graphics->winPadding() + j*graphics->tileSize() + j*graphics->tilePadding();
            positions.push_back( SDL_Rect{ x, y, graphics->tileSize(), graphics->tileSize() });
        }
    }
}

void MainGame::loadPositions(std::vector<SDL_Rect>& shiftpositions, const int& gridsize, const int& shiftPx) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < gridsize; ++i) {
        y = graphics->winPadding() + i*graphics->tileSize() + i*graphics->tilePadding() + shiftPx;
        for (int j = 0; j < gridsize; ++j) {
            x = graphics->winPadding() + j*graphics->tileSize() + j*graphics->tilePadding() + shiftPx;
            shiftpositions.push_back( SDL_Rect{ x, y, graphics->tileSize(), graphics->tileSize() } );
        }
    }
}

void MainGame::makeTiles(std::vector<Tile>& tiles) {
    for (int i = 0; i < positions.size(); ++i) {
        tiles.push_back(Tile{positions[i], i+1, i+1}); // start counting from 1 for positions
    }
}
// Used for tile 'shadows'
void MainGame::makeTiles(std::vector<Tile>& shifttiles, const std::vector<SDL_Rect>& shiftpositions) {
    for (int i = 0; i < shiftpositions.size(); ++i) {
        shifttiles.push_back(Tile{shiftpositions[i], i, 2});
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

bool MainGame::isNeighbour(const Tile& a, const Tile& b) {
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