#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <exception>
#include <cstdint>

const int SCREEN_WIDTH{ 640 };
const int SCREEN_HEIGHT{ 480 };

SDL_Window* gWindow{NULL};
SDL_Renderer* gRenderer{NULL};

void init();

enum BlockType {
  EMTPY,
  RED,
  GREEN,
  BLUE
};

struct Piece {
  BlockType type{BlockType::RED};
  std::vector<std::vector<bool>> shape {
    {0, 0, 0},
    {1, 1, 0},
    {1, 1, 0}
  };
  // std::pair<int, int> rotationalCenter{}
};

class TetrisBoard {
  const int width{10};
  const int height{20};
  const int playFieldHeight{SCREEN_HEIGHT};
  const int squareSize{playFieldHeight / height};
  const int playFieldWidth{(width * squareSize) + 1};
  const int playFieldX{(SCREEN_WIDTH / 3) - 1};
  std::vector<std::vector<BlockType>> board {};
  SDL_Rect playField{playFieldX, 0, playFieldWidth, playFieldHeight};

  public:
  TetrisBoard() {
    for(auto i = 0; i < width; i++) {
      board.emplace_back(std::vector<BlockType>{});
      for(auto j = 0; j < height; j++) {
        board[i].emplace_back(BlockType::EMTPY);
      }
    }

    board[2][18] = BlockType::RED;
    board[2][19] = BlockType::RED;
    board[3][19] = BlockType::RED;
    board[4][19] = BlockType::RED;

    board[5][5] = BlockType::BLUE;
    board[6][5] = BlockType::BLUE;
    board[7][5] = BlockType::BLUE;
    board[8][5] = BlockType::BLUE;
  };

  void renderSquares() {
    for(auto i = 0; i < width; i++) {
      for(auto j = 0; j < height; j++) {
        SDL_Rect rect{};
        rect.x = (squareSize * i) + playFieldX;
        rect.y = squareSize * j;
        rect.w = squareSize;
        rect.h = squareSize;

        auto block = board[i][j];
        switch(block) {
          case BlockType::RED:
            SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
            break;
          case BlockType::GREEN:
            SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
            break;
          case BlockType::BLUE:
            SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
            break;
          default:
            SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 100);
        }

        if(block == BlockType::EMTPY) {
          SDL_RenderDrawRect(gRenderer, &rect);
        } else {
          SDL_RenderFillRect(gRenderer, &rect);
        }
      }
    }
  }

  void render() {
    // Render playfield
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    renderSquares();
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(gRenderer, &playField);
  }
};

void init() {
  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error((std::string)"Initializing video failed: " + SDL_GetError());
  } else {
    gWindow = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == NULL) {
      throw std::runtime_error((std::string)"Initializing window failed: " + SDL_GetError());
    } else {
      gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    }
  }
}

int main(int argc, char* args[]) {
  TetrisBoard tetrisBoard{};
  try {
    init();
    SDL_Event e;

    uint32_t mTicksCount = SDL_GetTicks();
    float dt = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if(dt > 0.05f) dt = 0.05f;
    bool quit{false};

    while(!quit) {
      tetrisBoard.render();
      SDL_RenderPresent(gRenderer);

      while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
          quit = true;
        }
      }
    }
  } catch (std::runtime_error e) {
    std::cout<<e.what()<<std::endl;
  }

  return 0;
}