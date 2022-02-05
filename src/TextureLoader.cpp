#include "TextureLoader.hpp"
#include <iostream>

#define nl '\n'

using namespace std;

TextureLoader::TextureLoader() {};
TextureLoader::~TextureLoader() {};

SDL_Surface* TextureLoader::renderSurface = nullptr;

SDL_Texture* TextureLoader::generateTexture(const char* srcFilename, SDL_Renderer* renderer) {
    SDL_Texture* currentPieceTexture = nullptr;
    renderSurface = IMG_Load(srcFilename);
    currentPieceTexture = SDL_CreateTextureFromSurface(renderer, renderSurface);
    SDL_FreeSurface(renderSurface);
    return currentPieceTexture;
}
