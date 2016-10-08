#include "Game.h"
#include "Tile.h"
#include "Graphics.h"
#include <iostream>
#include <random>

Graphics graphics;

Game::Game() {
    // Set up SDL window & renderer
    graphics.setup();
    // Fill vector 'positions' with possible positions of n*n tiles
    loadPositions(graphics.gridSize);
    // Assign these starting positions to n*n tiles in vector 'tiles'.
    makeTiles(graphics.gridSize);
    // Draw the tiles and clicks
    graphics.drawBoard(tiles);
    graphics.updateClicks(clicks);
    graphics.update();
    // Main game loop
    gameLoop();
}

void Game::restart() {
    gameWin = false;
    clicks = 0;
    
    scrambleTiles(tiles);
    graphics.drawBoard(tiles);
    graphics.updateClicks(clicks);
    graphics.update();
    gameLoop();
}

void Game::loadPositions(const int& n) {
    int x = graphics.winPadding();
    int y = graphics.winPadding();
    
    for (int i = 0; i < n; ++i) {
        y = graphics.winPadding() + i*graphics.tileSize() + i*graphics.tilePadding();
        for (int j = 0; j < n; ++j) {
            x = graphics.winPadding() + j*graphics.tileSize() + j*graphics.tilePadding();
            positions.push_back( SDL_Rect{ x, y, graphics.tileSize(), graphics.tileSize() });
        }
    }
}

void Game::makeTiles(const int& n) {
    for (int i = 0; i < positions.size(); ++i) {
        tiles.push_back(Tile{positions[i], i, i});
    }
}

void Game::scrambleTiles(std::vector<Tile> t) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> range(1, tiles.size()-1);
    
    for (int i = 1; i < tiles.size(); ++i) {
        int n = range(rng);
        tiles[i].swap(tiles[n]);
    }
}

void Game::gameLoop() {
    SDL_Event event;
    
    while (gameExit == false) {
        while (SDL_PollEvent(&event)) {
            
            if (event.type == SDL_QUIT)
                gameExit = true;
            
            if (event.type == SDL_KEYDOWN){
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    gameExit = true;
            }
            
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (gameWin == true)
                    restart();
                
                int x; int y;
                SDL_GetMouseState(&x, &y);
                int clickedtile = clickedTile(x, y); // retreives clicked tile number or -1
                if (clickedtile > 0) { // if mouse clicked on a tile
                    if (isNeighbor(tiles[clickedtile], tiles[0])){
                        tiles[clickedtile].swap(tiles[0]);
                        clicks += 1;
                        graphics.drawBoard(tiles);
                        graphics.updateClicks(clicks);
                        graphics.update();
                        // update clicks text
                    }
                    
                }
            }
            if (gameWin == false && isSolved()){
                gameWin = true;
                graphics.winMessage();
                graphics.update();
            }
        }
    }
    graphics.destroy();
}

int Game::clickedTile(const int &x, const int &y) {
    int tilenum = -1;
    for (int i = 0; i < tiles.size(); ++i) {
        if (! (x < tiles[i].position().x || x > tiles[i].position().x + graphics.tileSize() ||
               y < tiles[i].position().y || y > tiles[i].position().y + graphics.tileSize() ))
            tilenum = i ;
    }
    return tilenum;
}

bool Game::isNeighbor(const Tile& a, const Tile& b) {
    if (a.posNumber() == b.posNumber() - 1 ||
        a.posNumber() == b.posNumber() + 1 ||
        a.posNumber() == b.posNumber() + graphics.gridSize ||
        a.posNumber() == b.posNumber() - graphics.gridSize)
        return true;
    return false;
}

bool Game::isSolved() {
    int correctiles = 0;
    
    for (int i = 0; i < tiles.size(); ++i) {
        if (tiles[i].position().x == positions[i].x &&
            tiles[i].position().y == positions[i].y)
            correctiles += 1;
    }
    if (correctiles == graphics.gridSize*graphics.gridSize)
        return true;
    return false;
}

