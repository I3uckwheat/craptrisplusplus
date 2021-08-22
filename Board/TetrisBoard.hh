#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <exception>
#include <cstdint>
#include <memory>
#include <random>

#include <../Pieces/Piece.hh>
#include <../Pieces/PieceFactory.hh>
#include <../constants.hh>

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
  const int playFieldHeight{constants::SCREEN_HEIGHT};
  const int squareSize{playFieldHeight / height};
  const int playFieldWidth{(width * squareSize) + 1};
  const int playFieldX{(constants::SCREEN_WIDTH / 3) - 1};

  SDL_Renderer* gRenderer;

  std::mt19937 rgen;
  std::uniform_int_distribution<int> pieceRandomizer;

  uint32_t nextUpdate{0};
  uint32_t currentTime{0};

  std::vector<std::vector<BlockType>> board {};
  SDL_Rect playField{playFieldX, 0, playFieldWidth, playFieldHeight};

  std::unique_ptr<Piece> activePiece;

  public:
  TetrisBoard(SDL_Renderer* grenderer) : gRenderer{grenderer} {
    std::random_device rseed;
    rgen = std::mt19937(rseed());
    pieceRandomizer = std::uniform_int_distribution<int>(0, pieceFactories.size() - 1);

    for(auto row = 0; row < height; row++) {
      board.emplace_back(std::vector<BlockType>{});
      for(auto col = 0; col < width; col++) {
        board[row].emplace_back(BlockType::EMPTY);
      }
    }

    setNextPiece();
  }
  ~TetrisBoard() {}

  bool willBottomCollide() {
    auto pieceCoords = activePiece->getPieceCoords();
    for(auto pieceCoord = pieceCoords.begin(); pieceCoord != pieceCoords.end(); pieceCoord++) {
      auto row = pieceCoord->first + activePiece->row;
      auto col = pieceCoord->second + activePiece->col;
      if(row >= height - 1) {
        return true;
      }

      if(board.at(row + 1).at(col)) {
        return true;
      }
    }
    return false;
  }

  void mergePieceToBoard() {
    for(auto row = activePiece->canvasSize - 1; row > -1; row--) {
      for(auto col = activePiece->canvasSize - 1; col > -1; col--) {
        if(activePiece->shape[row][col]) {
          board[row + activePiece->row][col + activePiece->col] = activePiece->type;
        }
      }
    }
  }

  bool willHaveXCollision(Direction direction) {
    int newColumn{ activePiece->col };
    switch(direction) {
      case Direction::LEFT:
        newColumn--;
        break;
      case Direction::RIGHT:
        newColumn++;
        break;
    }
    
    auto pieceCoords = activePiece->getPieceCoords();
    for(auto i = pieceCoords.begin(); i != pieceCoords.end(); i++) {
      auto row = i->first + activePiece->row;
      auto col = i->second + newColumn;

      if(
        col < 0  // left bounds
        || col >= width // right bounds
        || board.at(row).at(col) // piece bounds
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
          activePiece->col--;
        }
        break;
      case SDLK_RIGHT:
        if(!willHaveXCollision(Direction::RIGHT)) {
          activePiece->col++;
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
    activePiece.reset(pieceFactories[pieceRandomizer(rgen)].create());
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
      activePiece->row++;
      nextUpdate = currentTime + 20;
    }

    if(willBottomCollide()) {
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
    for(auto row = 0; row < Piece::canvasSize; ++row) {
      for(auto col = 0; col < Piece::canvasSize; ++col) {
        if(activePiece->shape[row][col]) {
          SDL_Rect rect{};
          rect.x = (squareSize * (col + activePiece->col)) + playFieldX;
          rect.y = squareSize * (row + activePiece->row);
          rect.w = squareSize;
          rect.h = squareSize;

          SDL_RenderFillRect(gRenderer, &rect);
        }
      }
    }
  }

  void renderSquares() {
    for(auto row = 0; row < height; row++) {
      for(auto col = 0; col < width; col++) {
        SDL_Rect rect{};
        rect.y = squareSize * row;
        rect.x = (squareSize * col) + playFieldX;
        rect.w = squareSize;
        rect.h = squareSize;

        auto block = board[row][col];
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