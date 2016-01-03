#include <iostream>
#include <SDL2/SDL.h>
#include "res_path.h"
#include "cleanup.h"

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int, char**){
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // std::cout << "resource path: " << getResourcePath() << std::endl;

  SDL_Window *win = SDL_CreateWindow("Hello World!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  if(win == nullptr){
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if(ren == nullptr){
          cleanup(win);
          std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
          SDL_Quit();
          return 1;
  }

  std::string imagePath = getResourcePath("lesson1") + "hello.bmp";
  SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());

  if(bmp == nullptr){
          cleanup(ren, win);
          std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
          SDL_Quit();
          return 1;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
  SDL_FreeSurface(bmp);

  if(tex == nullptr){
          SDL_DestroyRenderer(ren);
          SDL_DestroyWindow(win);
          std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
          SDL_Quit();
          return 1;
  }

  SDL_RenderClear(ren);
  SDL_RenderCopy(ren, tex, NULL, NULL);
  SDL_RenderPresent(ren);

  SDL_Delay(3000);

  //Destroying things on quit
  cleanup(ren, tex, win);
  SDL_Quit();
  return 0;
}
