#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <exception>
#include <cstdint>
#include <memory>
#include <random>

#include <./Pieces/Piece.hh>
#include <./Pieces/PieceFactory.hh>

const int SCREEN_WIDTH{ 640 };
const int SCREEN_HEIGHT{ 480 };
const int SCREEN_FPS{ 24 };
const int SCREEN_TICKS_PER_FRAME{ 1000 / SCREEN_FPS };

SDL_Window* gWindow{NULL};
SDL_Renderer* gRenderer{NULL};

void init();

using BlockType = PieceEnums::BlockType;

enum Direction {
  LEFT,
  RIGHT,
  UP,
  DOWN
};

class TetrisBoard {
  const int width{10};
  const int height{20};
  const int playFieldHeight{SCREEN_HEIGHT};
  const int squareSize{playFieldHeight / height};
  const int playFieldWidth{(width * squareSize) + 1};
  const int playFieldX{(SCREEN_WIDTH / 3) - 1};

  std::mt19937 rgen;
  std::uniform_int_distribution<int> randomPiece;

  uint32_t nextUpdate{0};
  uint32_t currentTime{0};

  std::vector<std::vector<BlockType>> board {};
  SDL_Rect playField{playFieldX, 0, playFieldWidth, playFieldHeight};

  std::unique_ptr<Piece> activePiece;

  public:
  TetrisBoard() {
    std::random_device rseed;
    rgen = std::mt19937(rseed());
    randomPiece = std::uniform_int_distribution<int>(0, pieceFactories.size() - 1);

    for(auto i = 0; i < width; i++) {
      board.emplace_back(std::vector<BlockType>{});
      for(auto j = 0; j < height; j++) {
        board[i].emplace_back(BlockType::EMPTY);
      }
    }

    setNextPiece();
  }
  ~TetrisBoard() {}

  bool hasBottomCollided() {
    auto pieceBottomCoords = activePiece->getPieceRelativeDirectionalCoords(PieceEnums::Direction::BOTTOM);
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

  bool willHaveXCollision(Direction direction) {
    int newX{ activePiece->x };
    switch(direction) {
      case Direction::LEFT:
        newX--;
        break;
      case Direction::RIGHT:
        newX++;
        break;
    }
    
    auto pieceCoords = activePiece->getPieceRelativeDirectionalCoords(PieceEnums::Direction::LEFT);
    for(auto i = pieceCoords.begin(); i != pieceCoords.end(); i++) {
      auto y = i->second + activePiece->y;
      auto x = i->first + newX;

      if(
        x < 0  // left bounds
        || x > width - 1 // right bounds
        || board.at(x).at(y) // piece bounds
      ) {
        return true;
      }
    }

    return false;
  }

  void move(SDL_Keycode key) {
    switch(key) {
      case SDLK_LEFT:
        if(!willHaveXCollision(Direction::LEFT)) {
          activePiece->x--;
        }
        break;
      case SDLK_RIGHT:
        if(!willHaveXCollision(Direction::RIGHT)) {
          activePiece->x++;
        }
        break;
      case SDLK_UP:
        // TODO Rotate
        break;
      case SDLK_DOWN:
        break;
    }
  }

  void setNextPiece() {
    activePiece.reset(pieceFactories[randomPiece(rgen)].create());
  }

  // bool isLineComplete(const std::vector<BlockType>& line) {
  //   std::cout<<"lineSize in isCOmplete: "<<line.size()<<std::endl;
  //   for(auto i = line.begin(); i < line.end(); i++) {
  //     std::cout<<*i<<" | "<<static_cast<BlockType>(*i)<<std::endl;
  //     if(static_cast<BlockType>(*i) == BlockType::EMTPY) {
  //       return false;
  //     }
  //   }

  //   return true;
  // }

  // void clearCompletedLines() {
  //   auto result = isLineComplete(board[0]);
  //   std::cout<<result<<std::endl;
  //   result = isLineComplete(board[12]);
  //   std::cout<<result<<std::endl;
  //   // auto i = height - 1;
  //   // while(i > 0) {
  //   //   auto lineIsCompleted = isLineComplete(board[i]);
  //   //   if(lineIsCompleted) std::cout<<"is complete"<<std::endl;
  //   //   i--;
  //   // }
  // }

  void update(float dt) {
    currentTime += dt;
    if(nextUpdate < currentTime){
      activePiece->y++;
      nextUpdate = currentTime + 20;
    }

    if(hasBottomCollided()) {
      mergePieceToBoard();
      setNextPiece();
      // clearCompletedLines();
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
      case BlockType::YELLOW:
        SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
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
        if(block == BlockType::EMPTY) {
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
        } else if (e.type == SDL_KEYDOWN) {
          tetrisBoard.move(e.key.keysym.sym);
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
