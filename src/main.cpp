#define SDL_MAIN_HANDLED

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Game.hpp"
#include "Chess.hpp"

#define nl '\n'

using namespace std;

Chess* chessGame = nullptr;

int main (int argc, char* argv[]) {
    cout << "Starting Game...FZ_CHESS" << nl;

    // Window Title
    const char* title = "FZ_CHESS";

    //Initial Window position relative to the screen
    const int Xpos = SDL_WINDOWPOS_CENTERED;
    const int Ypos = SDL_WINDOWPOS_CENTERED;

    //Initial window Dimensions
    const int DEFAULT_WINDOW_WIDTH = 640;
    const int DEFAULT_WINDOW_HEIGHT = 640;

    //FullScreen Configuration
    bool isFullscreen = false;

    chessGame = new Chess();

    chessGame->initialiseGame(title, Xpos, Ypos, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, isFullscreen);

    chessGame->printLogo();

    chessGame->printCreatorSprite();

    chessGame->startGameLoop();

    // SDL_Delay(5000);

    chessGame->cleanGame();

    cout << "Quitting" << nl;

    delete chessGame;

    return 0;
}