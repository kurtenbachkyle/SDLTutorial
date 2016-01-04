#include <iostream>
#include <array>
#include <SDL2/SDL.h>
#include "res_path.h"
#include "cleanup.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg + " : " << SDL_GetError() << std::endl;
}

SDL_Texture* loadTexture(SDL_Renderer *ren, const std::string &filepath){
    SDL_Texture *tex = nullptr;
    SDL_Surface *bmp = nullptr;
    bmp = SDL_LoadBMP(filepath.c_str());
    if(bmp == nullptr){
        return nullptr;
    } else {
        tex = SDL_CreateTextureFromSurface(ren, bmp);
        SDL_FreeSurface(bmp);
        if(tex == nullptr){
            logSDLError(std::cout, "loadTexture");
        }
    }
    return tex;
}

/**
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height
 * @param tex The source texture we want to draw
 * @param ren The renderer we want to draw to
 * @param x The x coordinate to draw to
 * @param y The y coordinate to draw to
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderAtMiddlePoint(SDL_Texture *tex, SDL_Renderer *ren, int x, int y){
    int width, height;
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
    renderTexture(tex, ren, (x-width/2), (y-height/2));
}

void tileTexture(SDL_Texture *tex, SDL_Renderer *ren, int startx, int starty, int stageWidth, int stageHeight){
    int width, height;
    SDL_QueryTexture(tex, NULL, NULL, &width, &height);
    int fillX = (stageWidth % width == 0) ? (stageWidth/width) : (stageWidth/width)+1;
    int fillY = (stageHeight % height == 0) ? (stageHeight/height) : (stageHeight/height)+1;
    for(int i = 0; i < fillX; i++){
        for(int j = 0; j < fillY; j++){
            renderTexture(tex, ren, i*width, j*height);
        }
    }
}

int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        logSDLError(std::cout , "SDL_Init Error");
        return 1;
    }

    // std::cout << "resource path: " << getResourcePath() << std::endl;

    SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(win == nullptr){
        logSDLError(std::cout , "SDL_CreateWindow Error");
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(ren == nullptr){
        cleanup(win);
        logSDLError(std::cout , "SDL_CreateRenderer Error");
        SDL_Quit();
        return 1;
    }

    std::string resourcePath = getResourcePath("lesson2");
    SDL_Texture* background= loadTexture(ren, resourcePath+"background.bmp");
    SDL_Texture* image = loadTexture(ren, resourcePath+"image.bmp");

    if(background == nullptr || image == nullptr){
        cleanup(ren, win, background, image);
        logSDLError(std::cout , "SDL_CreateTextureFromSurface Error");
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    int x = SCREEN_WIDTH/2; 
    int y = SCREEN_HEIGHT/2;
    while(!quit){
        SDL_PollEvent(&e);
        
        if(e.type == SDL_QUIT){
            quit = true;
        }
        const Uint8* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_RIGHT] ) {
               x = x+10; 
        }
        if (state[SDL_SCANCODE_LEFT] ) {
               x = x-10; 
        }
        if (state[SDL_SCANCODE_UP] ) {
               y = y-10; 
        }
        if (state[SDL_SCANCODE_DOWN] ) {
               y = y+10; 
        }
        SDL_RenderClear(ren);
        // SDL_RenderCopy(ren, image, NULL, NULL);
        tileTexture(background, ren, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        renderAtMiddlePoint(image, ren, x, y);
        // renderAtMiddlePoint(image, ren, 0, 0);
        SDL_RenderPresent(ren);
    }

    //Destroying things on quit
    cleanup(ren, image, background, win);
    SDL_Quit();
    return 0;
}
