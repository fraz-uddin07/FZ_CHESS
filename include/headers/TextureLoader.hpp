#ifndef Texture_Loader_hpp
#define Texture_Loader_hpp

#include <SDL.h>
#include <SDL_image.h>

class TextureLoader {

public:
    TextureLoader();
    ~TextureLoader();

    static SDL_Texture* generateTexture(const char* srcFilename, SDL_Renderer* renderer);


private:

    static SDL_Surface* renderSurface;

};




#endif          //For Texture_Loader_hpp