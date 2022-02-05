
#ifndef Game_hpp
#define Game_hpp

#include <SDL.h>
#include <SDL_image.h>

class Game {

public:
    Game();
    ~Game();

    void initialiseGame(const char* title, int Xpos, int Ypos, int width, int height, bool isFullscreen);
    void renderBoard();
    void printCreatorSprite();
    void startGameLoop();
    // void render();       //Deprecated member Function
    void handleEvents();
    void update();
    void cleanGame();


protected:
    SDL_Renderer* renderer = nullptr;


private:
    SDL_Window* window = nullptr;
    // SDL_Renderer* bgRenderer;

    typedef struct Color {
        uint8_t red;
        uint8_t blue;
        uint8_t green;
        uint8_t alpha;
    } Color;

    Color PALE_GREY;
    Color SNOW_WHITE;
    Color WOODEN_BROWN;

    // PALE_GREY.red = 125;
    // PALE_GREY.blue = 135;
    // PALE_GREY.green = 150;
    // PALE_GREY.alpha = 255;

    // SNOW_WHITE.red = 255;
    // SNOW_WHITE.green = 255;
    // SNOW_WHITE.blue = 255;
    // SNOW_WHITE.alpha = 255;

    // WOODEN_BROWN.red = 159;
    // WOODEN_BROWN.green = 84;
    // WOODEN_BROWN.blue = 8;
    // WOODEN_BROWN.alpha = 255;

};



#endif          //For Game_hpp