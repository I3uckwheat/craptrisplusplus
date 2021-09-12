#pragma once
#include <vector>
#include <memory>

namespace PieceEnums {
  enum Direction {
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
  };

  enum BlockType {
    EMPTY,
    RED,
    GREEN,
    BLUE,
    PURPLE,
    YELLOW
  };
} 

struct Piece {
  Piece(PieceEnums::BlockType type, std::vector<std::vector<bool>> shape);
  int row;
  int col;
  static const int canvasSize{4};
  static const int totalPieces{5};

  // todo make const
  PieceEnums::BlockType type;

  std::vector<std::vector<bool>> shape;
  std::vector<std::pair<int, int>> getPieceCoords();
  void rotatePiece();
};
