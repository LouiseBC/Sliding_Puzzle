#include <iostream>
#include <random>
#include "Gamestate_MainGame.h"

bool MainGame::init(Graphics* graph, Game* g) {
    game     = g;
    graphics = graph;
    int shiftamount = 3;
    
    // Fill vector<SDL_Rect> 'positions' with possible positions of n*n tiles & make shadow positions
    loadPositions(positions, graphics->gridSize());
    loadPositions(shadowPositions, graphics->gridSize(), shiftamount);
    // Assign these starting positions to n*n tiles in vector<Tile> 'tiles' & make tile shadows
    makeTiles(tiles, positions, Tile::type::button);
    makeTiles(shadowTiles, shadowPositions, Tile::type::shadow);
    
    return loadAudio();
}

bool MainGame::loadAudio() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    click = Mix_LoadWAV("assets/hover.wav");
    if( click == nullptr ) {
        std::cerr << "Failed to load beat 'click', error:" << Mix_GetError() << std::endl;
        return false;
    }
    cat1 = Mix_LoadWAV("assets/meow1.wav");
    if( cat1 == nullptr ) {
        std::cerr << "Failed to load beat 'meow', error:" << Mix_GetError() << std::endl;
        return false;
    }
    cat2 = Mix_LoadWAV("assets/meow2.wav");
    if( cat2 == nullptr ) {
        std::cerr << "Failed to load beat 'meow', error:" << Mix_GetError() << std::endl;
        return false;
    }
    cat3 = Mix_LoadWAV("assets/meow3.wav");
    if( cat3 == nullptr ) {
        std::cerr << "Failed to load beat 'meow', error:" << Mix_GetError() << std::endl;
        return false;
    }
    return true;
}

void MainGame::loadPositions(std::vector<SDL_Rect>& positions, const int& gridsize, const int& shiftPx) {
    int x = graphics->winPadding();
    int y = graphics->winPadding();
    
    for (int i = 0; i < gridsize; ++i) {
        y = graphics->winPadding() + i*graphics->tileSize() + i*graphics->tilePadding() + shiftPx;
        for (int j = 0; j < gridsize; ++j) {
            x = graphics->winPadding() + j*graphics->tileSize() + j*graphics->tilePadding() + shiftPx;
            positions.push_back( SDL_Rect{ x, y, graphics->tileSize(), graphics->tileSize() } );
        }
    }
}

void MainGame::makeTiles(std::vector<Tile>& tiles, const std::vector<SDL_Rect>& positions, const int& tiletype) {
    for (int i = 0; i < positions.size(); ++i) {
        tiles.push_back(Tile{positions[i], i+1, tiletype}); // start counting from 1 for positions
    }
    tiles[tiles.size()-1].setTileType(Tile::type::invisible); // last tile should be invisible
}

void MainGame::handleEvents(SDL_Event& event) {
    if (event.type == SDL_QUIT)
        game->setQuit();
    
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game->setQuit();
    }
    
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

void MainGame::restart() {
    gameWin = false;
    clicks = 0;
    scrambleTiles(tiles, shadowTiles);
}

void MainGame::update() {
    if (isSolved()) {
        gameWin = true;
    }
    
    else if (clickedTile >= 0) { // if mouse clicked on a tile
        if (isNeighbour(tiles[clickedTile], tiles[tiles.size()-1])) {
            tiles[clickedTile].swap(tiles[tiles.size()-1]);
            shadowTiles[clickedTile].swap(shadowTiles[shadowTiles.size()-1]);
            clickedTile = -1; // reset to default
            
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

void MainGame::scrambleTiles(std::vector<Tile>& t, std::vector<Tile>& tshadow) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> tilerange(0, t.size()-2); // don't scramble last ('empty') tile
    
    for (int i = t.size()-2; i >= 0; --i) {
        int n = tilerange(rng);
        t[i].swap(t[n]);
        tshadow[i].swap(tshadow[n]);
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

void MainGame::render() {
    graphics->renderClear();
    graphics->drawBoard(shadowTiles);
    graphics->drawBoard(tiles, true);
    
    if (game->isCatMode()) {
        graphics->renderCat(tiles, positions);
    }
    
    graphics->updateClicks(clicks);
    graphics->winMessage(gameWin, clicks, "You win! Click anywhere to restart"); // make sure it's not the start of the game
    graphics->menuInstruction("Press spacebar for main menu");
    graphics->update();
}

MainGame::~MainGame() {
    Mix_Quit();
    game = nullptr;
    graphics = nullptr;
    click = nullptr;
    cat1 = nullptr;
    cat2 = nullptr;
    cat3 = nullptr;
}