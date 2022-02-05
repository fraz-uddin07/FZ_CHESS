#include <iostream>
#include "Game.hpp"
#include "TextureLoader.hpp"

#define nl '\n'

using namespace std;

Game::Game() {};
Game::~Game() {};

void Game::initialiseGame(const char* title, int Xpos, int Ypos, int width, int height, bool isFullscreen) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        cout << "All Subsystems Initialised..." << nl;

        uint32_t windowConfiguration;
        if (isFullscreen == true) windowConfiguration = SDL_WINDOW_FULLSCREEN;
        else windowConfiguration = SDL_WINDOW_SHOWN;

        window = SDL_CreateWindow(title, Xpos, Ypos, width, height, windowConfiguration);
        SDL_SetWindowResizable(window, SDL_TRUE);

        if (window) cout << "Window Created..." << nl;

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // SDL_SetRenderDrawColor in R, G, B, Alpha Channels
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        cout << "Created Renderer" << nl;

    }
    else {
        cout << "Problem with subsystem Initialisatiion.EXITING" << nl;
    }
}


void Game::printCreatorSprite() {
    //Display Creators Sprite
    //SDL_Wait Event
    SDL_RenderFillRect(renderer, NULL);
    SDL_Texture* creatorSpriteTexture = TextureLoader::generateTexture("res/creator_sprite2.png", renderer);
    SDL_RenderCopy(renderer, creatorSpriteTexture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
    SDL_Delay(5000);

}


void Game::handleEvents() {

};



void Game::cleanGame() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    cout << "Game Cleared..." << nl;
}
