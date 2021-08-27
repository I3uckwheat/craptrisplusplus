#pragma once
#include <SDL2/SDL.h>
#include <vector>
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

  bool willBottomCollide();
  void mergePieceToBoard();
  bool willHaveXCollision(Direction direction);
  void setNextPiece();
  void setBlockColor(BlockType type);
  void renderFallingPiece();
  void renderSquares();


public:
  TetrisBoard(SDL_Renderer* grenderer); 
  ~TetrisBoard();
  void move(SDL_Keycode key);
  void update(float dt);
  void render() ;
};
