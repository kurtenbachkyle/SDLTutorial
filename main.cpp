#include <iostream>
#include <SDL2/SDL.h>
#include "res_path.h"

/*
 * Lesson 0: Test to make sure SDL is setup properly
 */
int main(int, char**){
  if (SDL_Init(SDL_INIT_VIDEO) != 0){
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return 1;
  }

  // std::cout << "resource path: " << getResourcePath() << std::endl;

  SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 100, 100, SDL_WINDOW_SHOWN);
  if(win == nullptr){
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
  }

  SDL_Quit();
  return 0;
}
