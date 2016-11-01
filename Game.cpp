#include <iostream>
#include <random>
#include "Game.h"
#include "Tile.h"
#include "Gamestate.h"


Game::Game() {
    quit = false;
}

bool Game::init(GameState* state) {
    graphics.setup();
    pushState(state);
    return true;
}

void Game::loop()
{
    while(quit == false)
    {
        update();
        render();
    }
    quitGame();
}

void Game::update()
{
    while(SDL_PollEvent(&event))
    {
        if(states.size() > 0){
            states.back()->handleEvents(event);
        }
        if(states.size() > 0){
            states.back()->update();
        }
    }
}

void Game::render()
{
    if(states.size() > 0)
        states.back()->render();
}

void Game::setQuit() {
    quit = true;
}

void Game::toggleCatMode() {
    if (catMode == false)
        catMode = true;
    else if (catMode == true)
        catMode = false;
}

void Game::pushState(GameState* state) {
    states.push_back(state);
    if(state->init(&graphics, this) == false)
        quit = true;
}

void Game::popState() {
    delete states.back();
    states.pop_back();
    
    if(states.size() == 0)
        quit = true;
}

void Game::quitGame() {
    while(states.size() > 0)
    {
        delete states.back();
        states.pop_back();
    }
    graphics.destroy();
    SDL_Quit();
}
