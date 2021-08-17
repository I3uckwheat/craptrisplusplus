#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <exception>
#include <cstdint>
#include <memory>

const int SCREEN_WIDTH{ 640 };
const int SCREEN_HEIGHT{ 480 };
const int SCREEN_FPS{ 24 };
const int SCREEN_TICKS_PER_FRAME{ 1000 / SCREEN_FPS };

SDL_Window* gWindow{NULL};
SDL_Renderer* gRenderer{NULL};

void init();

enum BlockType {
  EMTPY,
  RED,
  GREEN,
  BLUE,
  PURPLE
};

struct Piece {
  virtual ~Piece() {};
  BlockType type{BlockType::EMTPY};
  std::vector<std::vector<bool>> shape {
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
  };
  static const int canvasSize{4};
  int x {0};
  int y {0};
};

struct SquarePiece : public Piece {
  SquarePiece() {
    type = BlockType::GREEN;
    shape = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 1, 0},
      {0, 1, 1, 0}
    };

    x = 0;
    y = 0;
  }
};

struct LinePiece : public Piece {
  LinePiece() {
    type = BlockType::BLUE;
    shape = {
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0},
      {0, 1, 0, 0}
    };

    x = 0;
    y = 0;
  }
};

struct TPiece : public Piece {
  TPiece() {
    type = BlockType::PURPLE;
    shape = {
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 1, 0, 0},
      {1, 1, 1, 0}
    };

    x = 0;
    y = 0;
  }
};

class TetrisBoard {
  const int width{10};
  const int height{20};
  const int playFieldHeight{SCREEN_HEIGHT};
  const int squareSize{playFieldHeight / height};
  const int playFieldWidth{(width * squareSize) + 1};
  const int playFieldX{(SCREEN_WIDTH / 3) - 1};

  uint32_t nextUpdate{0};
  uint32_t currentTime{0};

  std::vector<std::vector<BlockType>> board {};
  SDL_Rect playField{playFieldX, 0, playFieldWidth, playFieldHeight};

  std::unique_ptr<Piece> activePiece;

  public:
  TetrisBoard() {
    activePiece.reset(new LinePiece{});
    for(auto i = 0; i < width; i++) {
      board.emplace_back(std::vector<BlockType>{});
      for(auto j = 0; j < height; j++) {
        board[i].emplace_back(BlockType::EMTPY);
      }
    }
  }
  ~TetrisBoard() {}

  bool hasBottomCollided() {
    // Find bottom coords of piece
    std::vector<std::pair<int, int>> pieceBottomCoords{};
    for(auto i = activePiece->canvasSize - 1; i > -1; i--) {
      for(auto j = activePiece->canvasSize - 1; j > -1; j--) {
        if(activePiece->shape[j][i]) {
          pieceBottomCoords.emplace_back(std::pair<int, int>{i, j});
          break;
        }
      }
    }

    for(auto i = pieceBottomCoords.begin(); i != pieceBottomCoords.end(); i++) {
      auto x = i->first + activePiece->x;
      auto y = i->second + activePiece->y;
      if(y >= height - 1) {
        return true;
      }

      if(board[x][y + 1]) {
        return true;
      }
    }
    return false;
  }

  void mergePieceToBoard() {
    for(auto i = activePiece->canvasSize - 1; i > -1; i--) {
      for(auto j = activePiece->canvasSize - 1; j > -1; j--) {
        if(activePiece->shape[j][i]) {
          board[i + activePiece->x][j + activePiece->y] = activePiece->type;
        }
      }
    }
  }

  void update(float dt) {
    currentTime += dt;
    if(nextUpdate < currentTime){
      activePiece->y++;
      nextUpdate = currentTime + 40;
    }

    if(hasBottomCollided()) {
      mergePieceToBoard();

      // TODO: Random piece
      activePiece.reset(new TPiece{});
    }
  }

  void setBlockColor(BlockType type) {
    switch(type) {
      case BlockType::RED:
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        break;
      case BlockType::GREEN:
        SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
        break;
      case BlockType::BLUE:
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 255);
        break;
      case BlockType::PURPLE:
        SDL_SetRenderDrawColor(gRenderer, 128, 0, 128, 255);
        break;
      default:
        SDL_SetRenderDrawColor(gRenderer, 100, 100, 100, 100);
    }
  }

  void renderFallingPiece() {
    setBlockColor(activePiece->type);
    for(auto i = 0; i < 4; ++i) {
      for(auto j = 0; j < 4; ++j) {
        if(activePiece->shape[j][i]) {
          SDL_Rect rect{};
          rect.x = (squareSize * (i + activePiece->x)) + playFieldX;
          rect.y = squareSize * (j + activePiece->y);
          rect.w = squareSize;
          rect.h = squareSize;

          SDL_RenderFillRect(gRenderer, &rect);
        }
      }
    }
  }

  void renderSquares() {
    for(auto i = 0; i < width; i++) {
      for(auto j = 0; j < height; j++) {
        SDL_Rect rect{};
        rect.x = (squareSize * i) + playFieldX;
        rect.y = squareSize * j;
        rect.w = squareSize;
        rect.h = squareSize;

        auto block = board[i][j];
        setBlockColor(block);
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
    renderFallingPiece();

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
    uint32_t nextIterationTime = SDL_GetTicks() + SCREEN_TICKS_PER_FRAME;

    bool quit{false};
    while(!quit) {
      tetrisBoard.render();
      SDL_RenderPresent(gRenderer);

      while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
          quit = true;
        }
      }

      uint32_t now { SDL_GetTicks() };
      if(nextIterationTime > now) {
        SDL_Delay(nextIterationTime - now);
      } else {
        nextIterationTime += SCREEN_TICKS_PER_FRAME;
      }

      tetrisBoard.update(2);

    }
  } catch (std::runtime_error e) {
    std::cout<<e.what()<<std::endl;
  }

  return 0;
}
