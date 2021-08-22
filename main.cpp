#include <SDL2/SDL.h>
#include <iostream>
#include <exception>
#include <cstdint>

#include <./constants.hh>
#include <./Board/TetrisBoard.hh>

SDL_Window* gWindow{NULL};
SDL_Renderer* gRenderer{NULL};

using BlockType = PieceEnums::BlockType;

void init() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error((std::string)"Initializing video failed: " + SDL_GetError());
  } else {
    gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, constants::SCREEN_WIDTH, constants::SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      throw std::runtime_error((std::string)"Initializing window failed: " + SDL_GetError());
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    }
  }
}

int main(int argc, char* args[]) {
  try {
    init();
    TetrisBoard tetrisBoard{gRenderer};
    SDL_Event e;
    uint32_t nextIterationTime = SDL_GetTicks() + constants::SCREEN_TICKS_PER_FRAME;

    bool quit{false};
    while(!quit) {
      tetrisBoard.render();
      SDL_RenderPresent(gRenderer);

      while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
          quit = true;
        } else if (e.type == SDL_KEYDOWN) {
          tetrisBoard.move(e.key.keysym.sym);
        }
      }

      uint32_t now { SDL_GetTicks() };
      if(nextIterationTime > now) {
        SDL_Delay(nextIterationTime - now);
      } else {
        nextIterationTime += constants::SCREEN_TICKS_PER_FRAME;
      }

      // TODO: fix delta t
      tetrisBoard.update(2);

    }
  } catch (std::runtime_error e) {
    std::cout<<e.what()<<std::endl;
  }

  return 0;
}
